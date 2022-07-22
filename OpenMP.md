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
