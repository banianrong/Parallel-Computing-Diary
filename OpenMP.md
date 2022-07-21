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
|`omp_get_thread_num`||
