# **用MPI进行分布式内存编程**

大部分并行多指令多数据流计算机，都分为**分布式内存系统**和**共享内存系统**两种。

消息传递程序中，运行在一个核-内存对上的程序通常称为一个进程。两个进程可以通过调用函数来进行通信。我们使用的消息传递的实现称为消息传递接口(Message-Passing Interface, MPI)

## **Hello_World -> Hello_thread**

```
#include<stdio.h>
#include<string.h>
#include"mpi.h"

const int MAX_STRING = 100;

int main() {
  char greeting[MAX_STRING];
  int comm_sz, my_rank;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(my_rank != 0) {
    sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
    MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }else{
    printf("Greeting from process %d of %d!\n", my_rank, comm_sz);
    for(int q = 1; q < comm_sz; q++) {
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", greeting);
    }
  }
  MPI_Finalize();
  return 0;
}
```

注意这里的函数`MPI_Recv`中的status参数可以设置为`MPI_STATUS_IGNORE`

## MPI_Init和MPI_Finalize

- 从经验上来看，使用MPI_Init初始化之前，不应调用其他MPI函数，调用该函数是为了让系统为消息缓冲区分配存储空间，为进程指定进程号。
- MPI_Finalize是为了告知MPI系统MPI已经使用完毕，为MPI而分配的任何资源都可以释放了。同样，一般而言，使用完MPI_Finalize函数之后，就不应该调用MPI函数了。

总之一个典型的MPI程序的框架如下：

```
#include"mpi.h"

int main(int argc, char* argv[]) {
  ...
  MPI_Init(&argc, &argv);
  ...
  MPI_Finalize();
  ...
  return 0;
}
```

同时MPI_Init和MPI_Finalize未必需要在main函数里面调用，如下所示

```
#include<stdio.h>
#include<string.h>
#include"mpi.h"

const int MAX_STRING = 100;

void foo() {
  MPI_Init(NULL, NULL);
}

void bar() {
  MPI_Finalize();
}

int main() {
  char greeting[MAX_STRING];
  int comm_sz, my_rank;
  foo();
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(my_rank != 0) {
    sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
    MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }else{
    printf("Greeting from process %d of %d!\n", my_rank, comm_sz);
    for(int q = 1; q < comm_sz; q++) {
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", greeting);
    }
  }
  bar();
  return 0;
}
```

### **通信子**

通信子指的是一组可以互相发送消息的进程集合。MPI为通信子定义的特殊类型为**MPI_Comm**，MPI_Init一开始就定义由用户启动的所有进程组成的通信子**MPI_COMM_WORLD**以及只包括自己本身的通信子**MPI_COMM_SELF**

### **SPMD**

我们编写的是单个程序，让不同进程产生不同动作。主要通过不同的进程号，来分配不同的任务分支，这一方法称为但程序多数据流(Single Program, Multiple Data, SPMD)

注意，我们的程序原则上可以运行任意数量的进程。

同时一个通信子中的进程发送的消息不能被另一个通信子中的进程所接收。

### **消息匹配**

- recv_type = send_type
- recv_buf_sz >= send_buf_sz
- recv_comm = send_comm
- recv_tag = send_tag

详细的参考MPI简谈

如果一个进程接收多个进程的信息但是不知道其他进程发送消息得顺序。为了提高运行效率，我们这里引入一个特殊常量**MPI_ANY_SOURCE**，这样就可以按照多个进程发送消息得顺序来动态得接收，而不是静态顺序得接收。

同样对于tag来说也有一个特殊常量**MPI_ANY_TAG**

注意上面的两个特殊常量是MPI_Recv可以使用，MPI_Send不可以使用。

### **MPI_Status参数**

```
#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid;
  char buf[BUFLEN];
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("processor %d running\n", myid); fflush(stdout);
    memset(buf, 0, sizeof(buf));
    if(myid != 0) {
      sprintf(buf, "hello, I\'m processor %d", myid);
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 0, 101, MPI_COMM_WORLD);
    }
    if(myid == 0) {
      for(int i = 0; i < 3; i++) {
        MPI_Recv(buf, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("get message : %s\n", buf); fflush(stdout);
        int cnt;
        MPI_Get_count(&status, MPI_CHAR, &cnt);
        printf("count %d from processor %d tag %d\n", cnt, status.MPI_SOURCE, status.MPI_TAG);
      } 
    }
    
  MPI_Finalize();
  return 0;
}
```

- 发送者MPI_SOURCE
- 标签MPI_TAG
- 接收的数据量，注意count的值会根据不同的数据类型而变化，所以如果没有特殊需求，一般不会为了获得这个值而去特意做一次运算

消息的传递其实有消息包装的环节，也就是所谓的添加信封。注意发送进程可以缓冲消息，也可以阻塞。如果发生阻塞，就会一直等待，直到可以发送消息为止。事实上，MPI_Send返回，只是说明发送缓冲区可以被再次使用，实际上并不知道消息是否已经成功传递出去。

MPI_Recv总是阻塞的，因此，当MPI_Recv返回的时候，我们就知道进程已经成功接收到信息了。

对于同一进程的两条消息来说，前面的消息一定要被先接受(不可超越的)。但是对于不同进程的两条消息来说，消息的接收上面没有硬性的规定。

如果使用MPI_Send没有接收，消息将会丢失i。如果使用MPI_Recv没有发送，那么线程就会被挂起，最后导致死锁现象的出现。

## **用MPI来实现梯形积分法**

设计一个并行程序的一般想法
- 将问题的解决方案划分成多个任务
- 将任务间之别处需要的通信信道
- 将任务聚合成复合任务
- 在核上分配复合任务

对于多线程程序中，局部变量是指只在使用它们的进程内有效，而全局变量是所有的进程都有效。

### **I/O处理**

现有的MPI标准中，允许MPI_COMM_WORLD中的所有进程访问标准输出stdout核标准错误输出stderr。但是，MPI不提供对于I/O设备访问的自动调度。也就是说多个进程使用I/O设备的时候，他们的顺序是不太确定的。同时，容易出现一个进程还在打印，另一个进程终端该进程，开始打印该进程中的内容。

MPI进程都在相互竞争，以取得共享输出设备、标准输出stdout的访问。如果我们不希望线程的输出以随机顺序出现，那么此时要转换为串行的思想。比如，让除了0号进程之外的进程向0号进程发送它的输出，然后0号进程根据进程号的顺序打印输出结果。

但是对于输入，一般来说大部分的MPI只允许MPI_COMM_WORLD中的0号进程访问标准输入stdin。

```
#include"mpi.h"
#include<stdio.h>

int main() {
  int myid, num = 0;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("processor %d running\n", myid); fflush(stdout);
    if (myid == 0 ) scanf("%d", &num); //其他进程没法访问，会导致死锁
    printf("%d %d\n", myid, num);
  MPI_Finalize();
  return 0;
}
```

一般来说读取函数的格式如下

```
void Get_input() {
  int my_rank;
  int comm_sz;
  double* a_p;
  double* b_p;
  int* n_p;
  int dest;
  if(my_rank == 0) {
    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", a_p, b_p, n_p);
    for(dest = 1; dest < comm_sz; dest++) {
      MPI_Send(a_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
      MPI_Send(b_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
      MPI_Send(n_p, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    }else{
      MPI_Recv(a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
}
```
