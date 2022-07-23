# **OpenMP入门**
前情提要：**并行**(parallel):需要多个运算核心同时完成

其中有多处理器和单处理器多核两种实现方式，其中差异如下：
- 同一芯片上的多核通信速度更快
- 同一芯片上的多核能耗更低

## **OpenMP初见**

### **OpenMP环境配置**
笔者当初刚进入的时候，走了许多弯路，这里给出Dev-Cpp版本的OpenMP的环境配置

点击**Tools**，再点击里面的**Complier Options**，在里面的**General**下的**Add the following commands when calling the compiler:**加入这段控制语句**-fopenmp**就可以在Dev-Cpp中编写omp程序


### **OpenMP作用**
- 编译器指令(#pragma)、库函数、环境变量
- 极大地简化了C/C++/Fortran多线程编程
- 并不是全自动并行编程语言(其并行行为仍需由用户定义及控制)

### **\#pragma**
预处理指令
- 设定编译器状态或指定编译器完成特定动作
  - 需要编译器支持相应功能，否则将被忽略

` #pragma once `

该段代码实现的功能是头文件只被编译一次

```
#ifdef HEADER_H

#define HEADER_H

...

#endif
```

上述代码实现的功能也是头文件只被编译一次

| \#pragma once | #ifndef |
|:-------------:|:-------:|
|需要编译器支持|不需要特定编译器|
|针对物理文件|不针对物理文件|
|需要用户保证头文件没有多份拷贝|需要用户保证不同文件的宏名不重复|

`#pragma GCC poison printf`

禁止使用printf函数，该段语句本质上就是申明printf被污染了，紧跟在该条语句后面的代码段不能使用printf。

**注意：对于形如\#pragma的语句，其作用域一般只包含于该语句的下一个代码段，也就是一对大括号之内。**

对于\#pragma还有许多语法，类似

`#pragma startup`

`#pragma exit`

`#pragma warning`

这些语句对于gcc编译器来说，不能进行正常的编译，直接会被忽视，并不会执行，因此这里不表

总之，对于编译器来说如果不支持\#pragma指令则会直接将其忽视，并不会执行

**查看OpenMP版本**

- 使用 **_OPENMP** 宏定义

**使用OpenMP编写第一个程序**

```
#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel
  {
    printf("Hello World\n");
  }
  return 0;
}
```

我们一般通过 **\#pragma omp parallel** 指明并行部分

线程的编号获取是通过下列语句

` omp_get_thread_num(); `

总共的线程总数获取是通过下列语句

`omp_get_max_threads();`

**同一线程的多个语句一般情况下并不是连续执行，其中可能有其他线程加入，导致该线程阻塞。**

**OpenMP运行机制**

- 使用分叉(fork)与交汇(join)模型

**fork** : 由主线程(master thread)创建一组从线程(slave threads)
- 主线程编号永远为0(thread 0)
- 不保证执行顺序

**join** : 同步终止所有线程并将控制权转移回至主线程

> fork是各奔东西，join是百川入海

### **OpenMP语法**

#### **编译器指令**

`#pragma omp parallel [clause[clause]...]{stuctured block}`

- 指明并行区域及并行方式
- 这里的clause是指明详细的并行参数，其大致分为
  - 控制变量在线程间的作用域
  - 显式指明线程数目
  - 条件并行

**num_threads(int)**
- 用于指明线程数目
- 当没有指明时，将默认使用OMP_NUM_THREADS环境变量
  - 环境变量的值为系统运算核心数目(或超线程数目)
  - 可以使用 `omp_set_num_threads(int)` 修改全局默认线程数，注意这需要在外面修改，在线程中修改笔者尝试后没有作用
  - 可以使用omp_get_num_threads()获取当前设定的默认线程数
  - num_threads(int)优先级高于环境变量，不保证创建指定数目的线程，会后到系统资源的限制

这里做个区分

|并行控制指令|功能|
|:---------:|:--:|
|`omp_get_num_threads`|返回当前并行区域中的活动线程个数，如果在并行区域外部调用，返回1|
|`omp_get_thread_num`|返回当前的线程号，注意不要和之前的omp_get_num_threads混肴|
|`omp_set_num_threads`|设置进入并行区域时，将要创建的线程个数|
|`omp_get_max_threads`|用于获得最大的线程数量，这个最大数量是指在不使用num_threads的情况下，OpenMP可以创建的最大线程数量。需要注意这个值是确定的，与它是否在并行区域调用没有关系|

### **并行for循环**

```
#pragma omp parallel
{
  int n;
  for(n = 0; n < 4; n++) {
    int thread = omp_get_thread_num();
    printf("thread %d\n", thread);
  }
}
```

该段语句是每个线程都执行了一次该循环体中的内容，也就是最后的结果会发现每个线程都执行了四次`printf("thread %d\n", thread)`语句。

如果想要将循环中的迭代分配到多个线程并行，有两种方式，

**方式1**
```
#pragma omp parallel
{
  int n;
  #pragma omp for
  for(n = 0; n < 4; n++) {
    int thread = omp_get_thread_num();
    printf("thread %d]n", thread);
  }
}
```
在并行区域内加入`#pragma omp for`
注意：在并行区域内，for循环外还可以加入其他并行代码

**方式2**
```
int n;
#pragma omp parallel for
for(n = 0; n < 4; n++) {
  int thread = omp_get_thread_num();
  printf("thread %d\n", thread);
}
```
合并为`#pragma omp parallel for`，这样子写法更加简洁

### **嵌套并行**

OpenMP中的每个线程同样可以被并行化为一组线程
- OpenMP默认关闭嵌套，即默认情况下一个线程不能并行化为一组线程，需要使用`omp_set_nested(1)`打开
- 嵌套并行仍然是fork-join模型，注意无论什么时候，主线程的编号一定是**0**

### **语法限制**
当循环分配并行的时候，有下列限制：

- 不能使用!=作为判断条件(接受<,<=,>,>=)
- 循环必须为单入口单出口(也就是不能使用break，goto等跳转语句)

### **数据依赖性**
- 循环迭代相关(loop-carried dependence)
  - 依赖性与循环相关，去除循环则依赖性不存在
- 非循环迭代相关(loop-independent dependence)
  - 依赖性与循环无关，去除循环依赖性仍然存在

  如果数据与循环无关，那么普通的循环并行是可以的，但是如果与循环有关，那么并行就会出现问题

  ```
  #include<stdio.h>
  #include<omp.h>

  int main() {
    int a[4] = {1, 2, 3, 4};
    #pragma omp parallel for
    for(int i = 1; i < 4; i++) {
      a[i] = a[i-1] + 2;
    }//这里运行出来的结果不一定是1,3,5,7，这就是循环迭代相关
    return 0;
  }
  ```

## **线程互动与同步**

- OpenMP是多线程共享地址架构
  - 线程可通过共享变量通信
- 线程及其语句执行具有不确定性
  - 共享数据可能造成竞争条件(race condition)
  - 竞争条件:程序运行的结果依赖于不可控的执行顺序(此时结果是不可测的，即不同的线程执行顺序会有不同的结果)
- 必须使用同步机制避免竞争条件的出现
  - 同步机制将带来巨大开销
  - 尽可能改变数据的访问方式减少必须的同步次数

### **高级语言的语句并非原子操作**
比如count++并不是只有一个子操作，而是返回count值后再进行++操作。

对于count++其所包含的操作如下
```
LOAD Reg, count
ADD #1
STORE Reg, count
```

包含三个子操作，如果一个线程负责count++，另一个线程负责count--，那么就有可能出现下面的情况(竞争条件)

```
        thread 1        thread 2
          Reg    count    Reg
  LOAD     10     10    
  ADD      11     10  
           11     10       10    LOAD
           11     10        9    SUB
           11      9        9    STORE
  STORE    11     11
```

同样如果上述的模板中thread 1执行的是count--，而thread 2执行的是count++，那么最终count存储的是9，这样子就会因为执行顺序不同而造成不同的结果

### **临界区(critical section)**

`#pragma omp critical`
- 指的是一个访问共用资源(例如：共用设备或是共用存储器)的程序片段，而这些共用资源又无法同时被多个线程访问的特性
  - 同一时间内只有一个线程能执行临界区内代码
  - 其他线程必须等待临界区内线程执行完毕后才能进入临界区
  - 常用来保证对共享数据的访问之间互斥

  如果具有竞争条件，那么就可以使用临界区来保证结果的正确，此时被`#pragma omp critical`修饰的代码段对于每个线程来说此时里面的资源是它独占的，其他线程无权访问修改，因此可以消除竞争条件带来的不确定性

  ```
  #pragma omp parrallel for
  for(int i = 0; i < 1000; i++) {
    int value = rand()%20;
    histogram[value]++;
  }
  int total = 0;
  for(int i = 0; i < 20; i++) {
    total +== histogram[i];
    cout << histogram[i] << endl;
  }
  cout << "total:" << total << endl;
  ```

  本段代码的输出未必是1000，原因可以参考前面的count++，++并非原子操作，因此多线程使用的时候会形成竞争条件，导致最后的存储除了问题

****
  解决方案（添加临界区）：

  ```
  #pragma omp parallel for
  for(int i = 0; i < 1000; i++) {
    int value = rand()%20;
    #pragma omp critical
    {
      histogram[value]++;
    }
  }
  int total = 0;
  for(int i = 0; i < 20; i++) {
    total += histogram[i];
    cout << histogram[i] << endl;
  }
  cout << "total:" << total << endl;
  ```

### **原子(atomic)操作**
`#pragma omp atomic`
- 保证对内存的读写更新等操作在同一时间只能被一个线程执行
  - 常用来做计数器、求和等
- 原子操作通常比临界区执行更快
  - 不需要阻塞其他线程
- 临界区的作用范围更广，能够实现的功能更复杂

代码演示如下：
```
#pragma omp parallel for
for(int i = 0; i < 1000; i++) {
  int value = rand()%20;
  #pragma omp atomic
  histogram[value]++;
}
```

关于原子操作和临界区的一些总结

|指令|功能|
|:--:|:--:|
|临界区|访问临界资源的代码，一个线程进入了临界区，如果第二个线程不会进入临界区那么第二个线程也可以运行|
|原子操作|原子操作独占处理器，其他线程必须等原子操作完了才可以运行|

> 原子操作是山大王，临界区是此路是我开，此树是我栽

总的来说就是，原子操作时不允许其他线程抢占资，也就是不能中断，但是临界区没有这个硬性规定，只是相关的代码区段此时只能有一个线程执行。

### **栅障(barrier)**

`#pragma omp barrier`
- 在栅障点处同步所有线程
  - 先运行至栅障点处的线程必须等待其他线程
  - 常用来等待某部分任务完成再开始下一部分任务
  - 每个并行区域的结束点默认自动同步线程

```
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<omp.h>

int main() {
  srand(time(NULL));
  int total = 0;
  int histogram[20];
  memset(histogram, 0, sizeof(histogram));
  #pragma omp parallel num_threads(20)
  {
    for(int i = 0; i < 50; i++) {
      int value = rand()%20;
      #pragma omp atomic
      histogram[value]++;
    }

    int thread = omp_get_thread_num();
    #pragma omp atomic
    total += histogram[thread];
  }
  return 0;
}
```

这里如果没有加入栅障指令，那么最后统计的total一般不是1000，造成数据丢失，因为不能保证执行到`total += histogram[thread]`对应的数字已经全部统计完毕，其他线程可能仍然在统计

修改方案如下：

```
int total = 0;

#pragma omp parallel num_threads(20)
{
  for(int i = 0; i < 50; i++) {
    int value = rand()%20;
    #pragma omp atomic
    histogram[value]++;
  }
  #pragma omp barrier//保证执行到下一个语句之前，所有的线程都已经统计完毕了随机数的次数
  int thread = omp_get_thread_num();
  #pragma omp atomic
  total += histogram[thread];
}
```

这里给出`#pragma omp for`的一些相似之处

```
int total = 0;

#pragma omp parallel num_threads(20)
{
  #pragma omp for
  for(int i = 0; i < 1000; i++) {
    int value = rand()%20;
    #pragma omp atomic
    histogram[value]++;
  }
  int thread = omp_get_thread_num();
  #pragma omp atomic
  total += histogram[thread];
}
```
这里的for分配出来的结果仍然是1000，也就是说`#pragma omp for`也是等到最后的线程执行完循环才进行下一步任务，前面先完成的线程会进入等待状态。注意如果修改成`#pragma omp for nowait`，那么结果就会不确定，因为此时先完成的线程不会再等待了(产生了一个隐式栅障，但是nowait可以去除)。

### **single & master**

`#pragma omp single{}`
- 用于保证{}内的代码由一个线程完成
- 常用于输入输出或初始化
- 由第一个执行至此处的线程执行
- 同样会产生一个隐式栅障
  - 可由`#pragma omp single nowait`去除

`#pragma omp master{}`
- 与single相似，但指明由主线程执行
- 与使用if的条件并行等价
  - `#pragma omp parallel IF(omp_get_thread_num() == 0) nowait`，这边的话，笔者暂时没有找到相关的资料，不过可以理解为只有主线程执行该代码块，而其他线程直接跳过，可以执行后面的任务。
  - 默认不产生隐式栅障

`#pragma omp master`使用如下
```
int total = 0;
#pragma omp parrallel
{
  #pragma omp for
  for(int i = 0; i < 1000; i++) {
    int value = rand()%20;
    #pragma omp atomic
    histogram[value]++;
  }  
  #pragma omp master
  {
    for(int i = 0; i < 20; i++) {
      total += histogram[i];
    }
  }
}
```

注意single会产生栅障，但是master不会

> single独乐乐，master众乐乐

### **并行Reduction**
指明如何将线程局部结果汇总

- `#pragma omp for reduction(+:total)`
- 支持的操作，八大金刚
  1. `+`
  2. `-`
  3. `*`
  4. `&`
  5. `|`
  6. `&&`
  7. `||`
  8. `^`

```
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

int main() {
  int total = 0;
  int histogram[20];
  memset(histogram, 0, sizeof(histogram));
  #pragma omp parallel
  {
    #pragma omp for
    for(int i = 0; i < 1000; i++) {
      int value = rand()%20;
      #pragma omp atomic
      histogram[value]++;
    }
    #pragma omp for reduction(+:total)
    for(int i = 0; i < 20; i++) {
      total += histogram[i];
    }
  }
  printf("%d\n", total);
  return 0;
}
```

reduction指令就是指明如何将线程局部结果汇总，原先没有reduction限制的时候，total是每个线程共享的，也就是所有的线程都可以访问修改total的值。但是加入reduction限制后，每个线程的total都是独属于该线程的全新total，线程1和线程2的total没有关系，也就是线程1的total的值改变了，不会影响到线程2的total的值。这样每个线程都有自己独特的total，等到所有线程结束完成之后，再根据reduction后面所给的操作符，将所有线程的total经过该运算储存到原来的total中。

## **变量作用域**

### **OpenMP与串行程序的作用域不同**
- OpenMP中必须指明变量为**shared**或**private**

|Shared|Private|
|:----:|:-----:|
|变量为所有线程所共享|变量为线程私有，其他线程无法访问|
|并行区域外定义的变量默认为shared|并行区域内定义的变量默认为private|
||循环计数器默认为private|

```
int histogram[20];//histogram是shared
memset(histogram, 0, sizeof(histogram));
int total = 0;//total是shared

int i,j;
#pragma omp parallel for
for(i = 0; i < 1000; i++) {//循环计数器i是private，虽然定义在并行区域外部
  int value = rand()%20;
  #pragma omp atomic
  histogram[value]++;
  for(j = 0; j < 1000; j++) {//循环计数器j也是private
    ...
  }
}
```

### **显式作用域定义**
- 显式指明变量的作用域
- `shared(var)`
  - 指明变量**var**为**shared**
- `default(none/shared/private)`
  - 指明变量的默认作用域
  - 如果为**none**则必须指明并行区域内每一变量的作用域
  ```
  int a, b, c;
  ...//init a, b, c
  #pragma omp parallel default(shared) shared(a)
  {
    b += a;
  }
  ```
  这里会报错，因为b并没有指定是private还是shared，注意只要default中选择的是none，那么并行体中出现的变量就都要说明是shared还是private类型。
  - 注意在C/C++中只存在none和shared两种选择，对于Fortran，可以多一个private参数选择
  ```
  int sum = 0;
  #pragma omp parallel for
  for(int i = 0; i < 20; i++) {
    sum += i;
  }
  ```
  这里连续和的答案不确定，也是因为sum是shared，shared本质上就是说所有线程的sum指向的都是同一块内存空间
  - **注意：循环计数器变量无论前面是shared还是private都是private，否则会造成逻辑上的错误**
- `private(var)`
  - 指明变量var为private
  ```
  int i = 10;
  #pragma omp parallel for
  for(int j = 0; j < 4; j++) {
    printf("Thread %d: i = %d\n", omp_get_thread_num(), i);
  }
  printf("i = %d\n", i);
  ```
  这里线程中输出的i的值都是未定义的，因为此时i是private，也就是说，对于每个线程来说，他们的i都是相互独立的，拥有自己的空间。所以对于每个线程，他们的i都需要重新初始化。
