# **MPI简谈**
****

MPI是分布式内存系统，区别于OpenMP和Pthreads的共享内存系统。MPI是一种基于消息传递的并行编程技术，是如今最为广泛的并行程序开发方法。

## **MPI前世今生**

MPI(Message Passing Interface，简称MPI)是一种编程接口标准，不是一种具体的编程语言。

- 1992年开始起草
- 1994年发布第一个版本MPI-1
- 1997年发布第二个版本MPI-2
- 成为消息传递并行编程标准，也是最为流行的并行编程接口

### **MPI实现小记**

MPI是一个标准，不是语言。

1. MPICH
- [MPICH-1下载](https://www.mpich.org/)
- [MPICH-2下载](https://www.mpich.org/#download)
- 支持C/C++、Fortran 77 和 Fortran 90
2. Intel MPI
- [Intel MPI下载](https://www.intel.com/content/www/us/en/developer/articles/tool/oneapi-standalone-components.html#mpi)
- Intel MPI是符合MPI-2标准的MPI实现。
- Intel MPI在通信协议的选择上无需进行额外设置，可自动选择MPI进程间最快的传输协议。

### **MPI特点**

> 聚是一团火，散如满天星

- 基于消息传递的并行程序，也就是所谓的分布式内存心痛，其中的每个进程之间具有自己独立的堆栈和代码段，作为互不相关的多个程序执行，进程之间的信息交互完全通过显式的调用通信函数来实现。

- SPMD(Single Program Multiple Data)单程序多数据，使用一个程序来处理多个不同的数据集来达到并行的目的

- MPMD(Multiple Program Multiple Data)多程序多数据，使用不同的程序处理多个数据集，合作求解同一个问题

#### **SPMD**

本质上就是对于不同的数据集合都是一样的处理，在串行中，如果此时有一个数据集A，那么就是将该操作对数据集A全部执行过去。

那么在SPMD中，比如现在有三个独立的进程，那么就是将数据集A均等拆分成三份a1,a2,a3分给这三个进程(在SPMD中，每一个进程会有一个进程号**rank**，用来相互区别)，等到每个进程处理完任务后，再通过消息传递的办法来收集处理数据处理的结果。MPI标准正是为了实现最后的消息传递提供标准和实现方法。

#### **MPMD**

MPMD目前有三种典型的执行模型

1. **管理者(Master)/工人(Worker)类型**

- 由一个管理者程序来控制整个程序的执行
- 管理者程序负责将不同的任务分配给多个工人程序来完成工作

2. **联合数据分析类型**

- 不同的程序各自独立的完成自己的任务，在特定的时候交换数据
- 耦合性最少，通信少，较为容易获得更好的并行加速效果(相对于串行来说)

3. **流式类型**

- 和工厂中的流水线类似，假设有三个进程，那么对于一个任务来说，进程1完成后的输出作为进程2的输入，同理，进程2的输出作为进程3的输出，不同的线程之间构成的实际上是串行关系，但是当数据量足够大的时候，我们其实可以把这种关系当作并行关系，也就是任务1，2，3近似的同时开始，同时结束(不考虑是否为同一个任务)

> 编写并行程序是为了利用冗余硬件(例如多核，多处理器或多机)提高应用性能。

## **MPICH诞生之旅**

> 众里寻他千百度。蓦然回首，那人却在，灯火阑珊处。

笔者下载安装MPICH走了好多弯路，谨以此文纪念。

温馨提示：接下来的教程只适用于**windows**，**linux**笔者可不负责哦。

### **安装网址推荐**

[MPICH-2](https://www.mpich.org/static/downloads/1.4.1p1/)，点击去官网安装，或者复制下面的链接去`https://www.mpich.org/static/downloads/1.4.1p1/`安装。

进入到的界面后寻找**mpich2-1.4.1p1-win-x86-64.msi**，点击下载就可以了。下载后会得到一个文件，但是注意这时候将其移动到桌面，

### **开始安装**

注意，不要直接点击安装，这里的推荐是以管理员身份安装。右键进行管理员安装，但是win10没有这个操作，这里推荐是左下角windows图标，右键，然后找**Windows Powershell(管理员)**或者就是找到**管理员**就可以了。进入界面按照笔者的顺序输入命令即可(注意需要保证前面的msi文件已经放在桌面了，不然就需要自己cd去找了，这里熟悉dos系统的可以跳过)

```
cd..
cd..
cd users/username/desktop
msiexec /package mpich2-1.4.1p1-win-x86-64.msi
```

一共输入四个命令，注意第三个命令`cd users/username/desktop`中的`username`需要替换成自己的用户名，也就是开机进入的自己当初起的名字。第四个命令的` mpich2-1.4.1p1-win-x86-64.msi`是当初下载下来的文件名字，如果不是这个名字记得自己改下名字。

### **进入安装**

如果前面的操作没有问题，那么现在已经运行了安装程序。

注意两点：

- 注意不要点的太快，中间有一个**process manager setup**界面，那里需要输入自己的开机密码(针对笔记本电脑)，不是默认的**behappy**，笔者没有尝试过，但是听说直接**behappy**最后安装的结果也是蛮happy的。

- 之后会有一个`just for me`和`for everyone`的选项，默认是`just for me`，这里推荐改成`for everyone`，这里所有的修改，都是为了之后安装的顺利。

- 最后一直按next就行了，如果不想要在默认的文件夹，自己修改的话，那么注意自己找好想放的位置就可以了。这里默认路径为`C:/program files/mpich2`。

### **继续出发**

这里同时按住`win`+`R`，在里面输入`cmd`，按回车，唤出dos系统。

然后依次输入以下命令(本质上就是找到那个mpich2文件)

```
cd/.
cd program files
cd mpich2
cd bin
smpd -status
```

这里如果出现`smpd running on ...`的信息，就说明**smpd**是运行的，安装可以。不行的话，可能需要重新安装qaq。

### **设置mpiexec**

使用**mpiexec**我们需要先注册账户

接下来在文件夹中找到moich2文件夹，点进去找到**bin**，然后找到**wmpiregister**exe文件点击进行注册。当然如果之前的命令行窗口没有关闭，也可以输入`mpiexec -register`进行，是一样的，不过这个有图形化界面，推荐用图形化界面，好看一点。

同样是设置账户和密码，这里的账户就是锁屏之后电脑上显示的你的用户名字，密码就是锁屏密码。

注册完毕之后，可以在dos窗口下输入该命令：

`mpiexec -validate`

如果系统的反应是'SUCCESS'，那么就说明你注册成功了，否则需要重新开始注册。

### **环境变量的配置**

- 找到我的电脑，右键找到`属性`，点进去，然后找到`高级系统设置`，点击进去，里面的窗口中有`环境变量`点击进去，这里有自己的用户变量和系统变量，我们选择在系统变量加入。

- 在里面找到`Path`，点击，然后新建一个路径，将刚才的mpich2里面的bin文件添加进去，如果安装路径是前面所说的，那么就是

  `C:/program files/mpich2/bin`

  加入即可。
- 注意此时需要推出dos系统重新按照之前的策略，找到bin，或者输入下面的命令。

```
cd /.
cd program files
cd mpich2
cd examples
```

可以自行运行里面的cpi.exe文件看看是否正常。

## **MPI落地**

如果前面的方法都没有用，没有关系。我们这边直接来一个微型mpi环境的搭建，一切为了运行自己的第一个mpi程序。

通用操作：

[mpi下载](https://www.microsoft.com/en-us/download/details.aspx?id=57467)

进入官网以后点击**download**，将里面的`msmpisetup.exe`和`msmpisdk.msi`都下载下来。这里默认下载下来以后一直点的是确认，没有修改安装地址，如果有修改，根据笔者的代码自行修改地址那边的信息。

[mingw64-8.1.0](https://sourceforge.net/projects/mingw-w64/files/?source=navbar)

注意往下拉，找到`x86_64-posix-seh`，点击下载，最好是64位的，好像32位不是很匹配。以及环境变量的配置，一样找到我的电脑(此电脑)，右键选择**属性**，然后找到**高级系统设置**，然后找到环境变量，点进去。

在用户变量和系统变量找到path，然后刚才下载下来的`x86_64-posix-seh`，解压完成之后，找到里面的文件夹**bin**，比如笔者的就是`C:\Program Files\mingw64\bin`，也就是我将mingw64放在了C盘的program files下，然后在每个path下加入就可以了。

检验的过程就是仍然是同时按`win`+`R`，唤出dos系统，输入`gcc --version`，如果在一大堆英文中看到**8.1.0**就说明安装以及配置环境变量成功了。


### **vscode**

[vscode](https://code.visualstudio.com/)

#### **一进宫**

推荐去官网下载，点击download即可，里面的配置可以放心选择默认。如果下载完vscode，可以唤出dos系统，然后输入`code -v`，如果第一行是版本号，第三行是**x64**说明安装成功了。

#### **二进宫**

这里第一次打开vscode是英文界面，不熟悉的同学没有关系，往右边的悬浮窗口寻找，找到第五个扩展，或者直接`Ctrl+Shift+X`，在里面输入Chinese，然后下载应用就可以了，记得做完退出再进来就是中文界面了。

#### **三进宫**

接下来一样的操作，仍然是在拓展窗口，这时候我们搜索**code runner**，下载并且应用，点确认就可以了。然后依然是退出再进去。

#### **我不来啦**

最后一次进去，点击拓展界面，这时候应该能看到**code runner**在你的搜索栏下面，这时候右键，找到**拓展设置**点击，然后在里面找到这个**Code-runner:Executor Map**，如果觉得麻烦，直接`Ctrl+F`，然后再那个框中粘贴`Code-runner:Executor Map`，按个回车就能找到了，然后点击下面的`在setting.json中编辑`，找到里面的：

```
"c":"...."
"cpp":"...."
```

我们要修改的就是这个。

#### **造宫殿**

将原先的替换成下面的：

```
"c": "cd $dir && gcc $fileName -o $fileNameWithoutExt -fopenmp -l msmpi -L \"C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Lib\\x64\" -I \"C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Include\" && mpiexec -n 4 $fileNameWithoutExt",
"cpp": "cd $dir && g++ $fileName -o $fileNameWithoutExt -fopenmp -l msmpi -L \"C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Lib\\x64\" -I \"C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Include\" && mpiexec -n 4 $fileNameWithoutExt",

```
注意这里面的**MS-MPI**库的位置(`\"C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Lib\\x64\"`和`\"C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Include\"`)，尤其注意`"`和`\`之前都要加上转义符号`\`，否则vscode后面运行的时候会出现`no this file or directionary`，这边也可以按照自己之前下载的位置找到**x64**和**Include**对应替换就可以了

#### **运行自己的第一个mpi程序**

拷贝下面程序，然后在vscode上运行，记着点击右上角的三角，或者直接`Ctrl+Alt+N`，就可以测试自己的mpi环境搭建的如何了。

```
#include <iostream>
#include <mpi.h>
#include <stdint.h>
using namespace std;

int main(int argc, char* argv[]) {
  cout << "hello" << endl;
  int myid, numprocs;
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Get_processor_name(processor_name, &namelen);
  cout << "Hello World! Process" << myid << "of" << numprocs << " on " << processor_name << endl;
  MPI_Finalize();
  return 0;
}
```

如果输出为
```
hello
hello
hello
hello
Hello World! Process0of4 on ...
Hello World! Process1of4 on ...
Hello World! Process2of4 on ...
Hello World! Process3of4 on ...

```

这里的`...`每个人可能不一样，应该是硬件的一种编号，大致类似就可以了。接下来就可以mpi的探索之旅啦。

### **DevCPP**

又是我们梦开始的dev，不过因为目前dev的mingw64版本是5.11，编译mpi程序的时候会有错误，所以我们要给自己的dev升级一下。

#### **告别往昔**

点击**Tools**，在点击**Compiler Options**，进入编译器选项界面后，右上角从左到右点击第三个**Add a compiler set by folder**，然后找到前面下载的mingw64文件夹，点击确定。然后再点从左到右第四个(向右的箭头)**Rename the selected compiler set**，为自己新搭建的编译器起个名字。注意，如果以后想要换回以前的编译环境，可以往上面的菜单栏中找到**help**，往下一行最右边，就会看到刚才自己为编译器起的名字，这时候点击，然后选择里面的第一个就是以前自己的编译器环境，以后切换可以在这边实现。或者就是在刚才的**Compiler Options**里面的第一栏点击选择就可以了。

#### **浴火重生**
在**Add the following commands when calling the compiler**里面添加下列代码：

`-L "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -I "C:\Program Files (x86)\Microsoft SDKs\MPI\Include"`

注意这里面的**MS-MPI**库的位置(`"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64"`和`"C:\Program Files ，这边也可以按照自己之前下载的位置找到**x64**和**Include**对应替换就可以了

注意修改之前先在之前打勾，不然没法选择。

接着就是在下面的**Add the following commands when calling the linker**中改成下面的语句：

`-static-libgcc -fopenmp -l msmpi`

这里的`-fopenmp`是为了运行omp，而`-l msmpi`是为了运行mpi，根据自己的需要自行选择。

#### **偷懒是一辈子的事情**

这时候理论上可以开始运行自己的第一个mpi程序了，但是还不够方便，因为此时编译运行是没有并行的感觉，所以我们还要进行一步操作。

点击**Tools**，再点击**Configure Tools**，点击里面的**Add**，下面的内容copy即可

|标题|需要填写的内容|
|:--:|:-----------:|
|Title|`MPI RUN FOR 4`|
|Program|`C:\Windows\System32\cmd.exe`|
|Working Directory|`C:\Windows\System32\`|
|Parameters|`/c cd/d <PROJECTPATH> & mpiexec -n 4 <EXENAME> & "<EXECPATH>ConsolePauser.exe"`|

这边的parameters里面的数字4就是我们同时运行的线程有多少，这里可以根据自己的需要自行修改

#### **运行自己的第一个mpi程序**

拷贝下面程序，然后在dev运行，注意先编译，然后点击**Tools**，然后选择里面的**Package Manager**，点击自己刚刚搭建的**MPI RUN FOR 4**，就可以测试自己的mpi环境搭建的如何了。

```
#include <iostream>
#include <mpi.h>
#include <stdint.h>
using namespace std;

int main(int argc, char* argv[]) {
  cout << "hello" << endl;
  int myid, numprocs;
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Get_processor_name(processor_name, &namelen);
  cout << "Hello World! Process" << myid << "of" << numprocs << " on " << processor_name << endl;
  MPI_Finalize();
  return 0;
}
```

如果输出为
```
hello
hello
hello
hello
Hello World! Process0of4 on ...
Hello World! Process1of4 on ...
Hello World! Process2of4 on ...
Hello World! Process3of4 on ...

```

这里的`...`每个人可能不一样，应该是硬件的一种编号，大致类似就可以了。接下来就可以mpi的探索之旅啦。

## **MPI编程基础**

开始MPI语法的学习苦旅

### **关于`int main(int argc, char* argv[])`的解释**

注意main函数本质上只是一个程序执行的入口而已，平常我们使用scanf函数之类的，都是在执行的时候传入参数，那么有没有方法在程序启动的时候就传递参数呢，这里我们就要用到`int main(int argc, char* argv)`

#### **argc参数和argv参数**

```
#include<stdio.h>

int main(int argc, char* argv[]) {
  printf("argc = %d\n", argc);
  printf("%s\n", *argv);
}
```

运行上面的程序，我们会发现这边的结果为

```
argc = 1
C:/.../Untitled1.exe
```

argc代表了我们的命令行有`1`个字符串，而这个字符串就是`C:/.../Untitled1.exe`

所有我们就可以通过argc和argv这样的关系来进行命令行的输入

#### **传递参数的方法**

通过下面的格式传递：

`程序名.exe 字符串1 字符串2 ...`

```
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[]) {
  printf("argc = %d\n", argc);

  argv++;
  while(*argv) {
  	if(strcmp(*argv, "a") == 0) {
  		argv++;
  		printf("a\n");
	}else{
	    argv++;
	    printf("wrong\n");
	}
  }
  return 0;
}
```

输入以上程序，编译，假设此时产生的可执行文件叫做`a.exe`，那么注意此时是唤出cmd窗口，找到这个文件所在的位置，然后输入

`a.exe a A w`

就会得到一下的输出

```
argc = 4
a
wrong
wrong
```

所以如果需要程序带参数地启动的时候，就是用`int main(int argc, char* argv[])`，仅此而已。

### **MPI程序引入**

仍然是**Hello World**

```
#include<stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
  int rank;
  int size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Hello World from process %d of %d\n", rank, size);
  MPI_Finalize();
  return 0;
}
```

这里不使用argc，argv也是可以的

```
#include<stdio.h>
#include "mpi.h"

int main() {
  int rank;
  int size;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Hello World from process %d of %d\n", rank, size);
  MPI_Finalize();
  return 0;
}
```

这里运行的结果是：

```
Hello World from process 2 of 4
Hello World from process 1 of 4
Hello World from process 0 of 4
Hello World from process 3 of 4
```

这里笔者是4个进程，分别打印他们各自的编号，注意这边的顺序有很多种，并没有固定的顺序，因为他们是并行的，谁快，谁就先占用打印设备，仅此而已。

### **MPI四大护法**

首先，想要运行mpi，很明显
`#include"mpi.h"`是显然必要的。

- #### **MPI_Init和MPI_Finalize**

|函数|作用|
|:--:|:--:|
|`MPI_Init`|用来初始化MPI执行环境，建立多个MPI之间的联系，为后续通信做准备|
|`MPI_Finalize`|结束MPI执行环境|

MPI_Init和MPI_Finalize配套使用，用来定义mpi程序的并行区。一般只有在这两个定义的区域之内调用mpi函数，同时配套使用。

如果在并行区域之外有其他的行为执行，那么不同于OpenMP，大部分MPI实现 会在各个并行进程之间独立地执行相应地代码。

```
#include"mpi.h"
#include<stdio.h>

int main() {
  MPI_Init(NULL, NULL);
  MPI_Finalize();
  printf("Hello World\n");
  return 0;
}

/*
output:
Hello World
Hello World
Hello World
Hello World
*/
```

- C语言中的MPI_Init需要提供argc和argv参数，如果没有，写成NULL就可以了，二MPI_Finalize函数不需要提供参数。二者的返回值都是int类型，标识函数是否调用成功。

- 总的来说就是一下的调用形式
  - `MPI_Init(&argc, &argv);`
  - `MPI_Init(NULL, NULL);`
  - `MPI_Finalize();`


- #### **MPI_Comm_rank**

MPI_Comm_rank就是表示各个MPI进程的，使用的时候需要提供两个函数参数：

  - MPI_Comm类型的通信域，标识参与计算的MPI进程组。**MPI_COMM_WORLD**是MPI实现预先定义好的进程组，指的是所有MPI进程所在的进程组，如果想要申请自己的进程组，则需要通过MPI_Comm定义并通过其他MPI函数生成。
  - 整型指针，返回进程在相应进程组中的进程号。即需要将rank存放的地址了，本质上可以认为同scanf的参数类似
  - MPI还会预先定义一个进程组MPI_COMM_SELF，只包含自己的进程组，因此里面的编号都是0

```
#include<stdio.h>
#include"mpi.h"

int main() {
  int r1, r2;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &r1);
    MPI_Comm_rank(MPI_COMM_SELF, &r2);
    printf("%d %d\n", r1, r2);
  MPI_Finalize();
  return 0;
}
```

- #### **MPI_Comm_size**

本函数表示相应进程组之间有多少个进程。其返回的也是整型值，同样需要两个参数：

- MPI_Comm类型的通信域，标识参与计算的MPI进程组，与上面类似，这里就是MPI_COMM_WORLD

- 整型指针，返回相应进程组中的进程数

```
#include<stdio.h>
#include"mpi.h"

int main() {
  int r1, r2, s1, s2;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &r1);
    MPI_Comm_rank(MPI_COMM_SELF, &r2);
    MPI_Comm_size(MPI_COMM_WORLD, &s1);
    MPI_Comm_size(MPI_COMM_SELF, &s2);
    printf("world %d of %d, self %d of %d\n", r1, s1, r2, s2);
  MPI_Finalize();
  return 0;
}
```

### **MPI的点对点通信**

点对点通信时MPI编程的基础。接下来将引入两个重要的MPI函数`MPI_Send`和`MPI_Recv`

先给代码，注意这边的如果格式化(printf)的`%d %s`之类的漏掉的话，会发生通信错误。

```
#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main(int argc, char* argv[]) {
  int myid, numprocs, next, namelen;
  char buffer[BUFLEN], processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    printf("Process %d on %s\n", myid, processor_name);
    printf("Process %d of %d\n", myid, numprocs);
    memset(buffer, 0, BUFLEN*sizeof(char));
    if(myid == numprocs-1)
      next = 0;
    else
      next = myid+1;
    if(myid == 0)
    {
        strcpy(buffer, "hello there");
        printf("%d sending '%s'\n", myid, buffer);
        fflush(stdout);
        MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);
        printf("%d reveiving\n", myid);
        fflush(stdout);
        MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        printf("%d received '%s'\n", myid, buffer);
        fflush(stdout);
    }else{
        printf("%d receiving\n", myid);
        fflush(stdout);
        MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        printf("%d received '%s'\n", myid, buffer);
        fflush(stdout);
        MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);
        printf("%d sent '%s'\n", myid, buffer);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}
```

这里笔者的输出为

```
Process 2 on ...
Process 2 of 4
2 receiving
Process 0 on ...
Process 0 of 4
0 sending 'hello there'
Process 1 on ...
Process 1 of 4
1 receiving
Process 3 on ...
Process 3 of 4
3 receiving
0 reveiving
1 received 'hello there'
1 sent 'hello there'
2 received 'hello there'
2 sent 'hello there'
3 received 'hello there'
3 sent 'hello there'
0 received 'hello there'
```

接下来逐步拆解上面的程序

#### **四剑客**

```
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
MPI_Comm_rank(MPI_COMM_WORLD, &myid);
MPI_Get_processor_name(processor_name, &namelen);
```

这四个语句所执行的都是初始化操作，其中一个新成员`MPI_Get_processor_name`是用来取得运行本进程的机器名称，该名称放在processor_name中，其长度为namelen，同时`MPI_MAX_PROCESSOR_NAME`是记录机器名的最大长度的。

##### **MPI_Get_processor_name**

- 注意`MPI_Get_processor_name`的用法。

- `MPI_Get_processor_name(processor_name, &namelen)`

这里后面的代码

```
if(myid == numprocs - 1)
    next = 0;
else
    next = myid + 1;
```

目的是为了告诉进程号他们下一个进程号是多少，注意这是一个循环，最后一个进程号的下一个进程号是0。所以这里的代码也可以是`next = (myid + 1) % numprocs;`，至于写哪一种就看各自的选择了。

##### **fflush**

如今windows下的stdout变成及时输出，所以一般来说适用不适用fflush也看不出太大的区别了。

注意，平时使用的printf函数并不是直接打印到屏幕上，而是先发送到stdout(此时的stdout类似缓冲区)中，再由stdout发送到屏幕上。

那么假设现在stdout直到遇到`\n`才会进行打印输出，那么假设进程1发送`hello`给到stdout，然后这时候切换到进程2，进程2发送`hello world\n`给stdout，此时打印到屏幕上的就是

`hellohello world`

很明显第一个明明是进程1的，但是在我们看来是执行进程2打印出来的，为了解决这个问题，我们就要使用`fflush(stdout)`，它的作用就是立即将所有内容发送到指定输出设备上(清空缓冲区)。一般在多线程的输出中使用。

接下来主角登场

#### **MPI_Send**

- `MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);`
- MPI_Send函数的标准形式是
`int MPI_SEND(buf, count, datatype, dest, tag, comm)`

其中，输入参数包括：

|输入参数|作用|
|:-----:|:--:|
|`buf`|发送缓冲区的起始地址，可以是各种数组或结构的**指针**|
|`count`|整型，发送的数据个数，应为非负整数(感觉类似指针的偏移量)|
|`datatype`|发送数据的数据类型|
|`dest`|应该为整数，表示目的进程号，即destination|
|`tag`|应该为整数，消息标志|
|`comm`|MPI进程组所在的通信域(应该是发送的哪个进程号所在的通信域)|

- 该函数的作用就是向通信域comm中的dest进程发送数据。消息数据存放在buf中，类型是datatype，个数是count个。这个消息的标志是tag，用以和本进程向同意目的进程发送的其他消息区别开来。

对于具体的`MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD)`的解释

在通信域MPI_COMM_WORLD内，向进程号next发送信息。发送的是buffer里面的所有数据，数据类型就是MPI_CHAR(因为buffer存储的是char类型的数据，MPI_CHAR是MPI的预定义数据类型，和char一一对应)，MPI_Send的参数都是输入参数，没有输出参数

#### **MPI_Recv**

- `MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);`
- MPI_Recv的标准形式就是：`int MPI_Recv(buf, count, datatype, source, tag, comm, status);`
- MPI_Recv中的buffer和status是输出参数，其他的都是输入参数

其中的参数包括：

|参数类型|作用|
|:------:|:--:|
|`buf`|接收缓冲区的起始地址，可以是各种数组或结构的**指针**，为输出参数|
|`status`|MPI_Status结构指针，返回状态信息，为输出参数|
|`count`|整数，最多可接收的数据个数|
|`datatype`|接收数据的数据类型|
|`source`|整型，接受数据的来源即发送数据进程号|
|`tag`|整数，消息标识，应与相应的发soon给操作消息标识相同。|
|`comm`|本进程(消息接收进程)和消息发送进程所在的通信域|

对于`MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);`的解释：

在通讯域MPI_COMM_WORLD中，0号进程(假设是0)从任意进程(MPI_ANY_SOURCE表示接受任意进程发来的消息)，接收的标签号是99，而且不超过512个MPI_CHAR类型数据，保存到buffer中。

注意缓冲区buf的大小，不能小于发送过来的有效消息长度，否则可能由于数组越界导致程序错误(段错误)

##### **MPI_Status**
- MPI_Status是MPI中一个特殊的，也是比较有用的结构。MPI_Status的结构定义如下：

```
typedef struct MPI_Status {
  int count;
  int cancelled;
  int MPI_SOURCE;
  int MPI_TAG;
  int MPI_ERROR;
} MPI_Status;
```

- status主要显示接收函数的各种错误状态，我们通过访问status.MPI_SOURCE，status.MPI_TAG和status.MPI_ERROR就可以得到发送数据进程号，发送数据使用的tag以及本接收操作返回的错误代码。当然如果想要获取数据项数，笔者尝试了一下，好像通过`status.count`无法获取，需要通过MPI函数`MPI_Get_count`获得。

#### **MPI_Get_count**

其标准定义为：

`int MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count);`

其中前两个参数为输入参数，status是MPI_Recv返回的状态结构的指针，datatype指定数据类型，最后一个参数是输出参数，是实际接收到的给顶数据类型的数据项数。

笔者测试的程序如下，确实获得了实际收到的个数。

```
#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define MAXLEN 512

int main(int argc, char* argv[]) {
  int myid, namelen, numprocs;
  char buffer[MAXLEN], pro_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(pro_name, &namelen);
    printf("myid %d of %d running on %s\n", myid, numprocs, pro_name);
    if(myid == 0) {
      strcpy(buffer, "hello world");
      printf("processor 0 sending message: %s\n", buffer);
      fflush(stdout);
      MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, 3, 110, MPI_COMM_WORLD);
      printf("send %d data\n", strlen(buffer)+1);
      fflush(stdout);
    }
    if(myid == 3) {
      MPI_Recv(buffer, MAXLEN, MPI_CHAR, 0, 110, MPI_COMM_WORLD, &status);
      printf("processor 3 received message: %s\n", buffer);
      fflush(stdout);
      int count;
      MPI_Get_count(&status, MPI_CHAR, &count);
      printf("the data num is %d\n", count);
    }
  MPI_Finalize();
  return 0;
}
```
这里的count其实本质上是需要根据数据类型变化的，MPI_DOUBLE,MPI_INT,MPI_CHAR对于同一长度的数据所能存储的数据个数是不一样的，这与C是一样的。

上面的点对点通信的例子，对应上面MPMD中的流式模型，即进程i等待进程i-1传递过来的字符串，并将其传递给进程i+1，直到最后一个进程传递给进程0。

## **消息管理7要素**

mpi最重要的功能就是消息传递，MPI_Send和MPI_Recv负责在两个进程之间接收信息和发送信息。主要由以下7个参数构成。

- 发送或者接收缓冲区buf
- 数据数量count
- 数据类型datatype
- 目标进程或者源进程destination/source
- 消息标签tag
- 通信域comm
- 消息状态status，只在接收的函数中出现

**消息信封**
MPI程序中的消息传递和我们日常的邮件发送和传递有类似之处，其中buf，coutn，datatype是信件的内容，而source/destination，tag，comm是信件的信封，因此我们称之为消息信封。

### **消息数据类型**

消息数据类型，就是之前所说的datatype

#### **作用**
- 方便将非连续内存中的数据，以及具有不同数据类型的内容组成消息
- 其类型匹配非常严格，一是宿主语言(如C)数据类型和通信操作的数据类型匹配，同时发送方和接收方的数据类型匹配

#### **基本数据类型**

以下给出了MPI预定义数据类型与C数据类型的对应关系

|MPI预定义数据类型|相应的C数据类型|
|:--------------:|:------------:|
|`MPI_CHAR`|`signed char`|
|`MPI_SHORT`|`signed short int`|
|`MPI_INT`|`signed int`|
|`MPI_LONG`|`signed long int`|
|`MPI_UNSIGNED_CHAR`|`unsigned char`|
|`MPI_UNSIGNED_SHORT`|`unsigned short int`|
|`MPI_UNSIGNED`|`unsigned int`|
|`MPI_UNSIGNED_LONG`|`unsigned long int`|
|`MPI_FLOAT`|`float`|
|`MPI_DOUBLE`|`double`|
|`MPI_LONG_DOUBLE`|`long double`|
|`MPI_BYTE`|无对应类型|
|`MPI_PACKED`|无对应类型|

基本上就是`MPI+datatype`的结构

一开始的时候建议尽可能地保证发送和接收地数据类型完全一致。

这里面的多出来的`MPI_BYTE`和`MPI_PACKED`，可以与任意以字节为单位的消息相匹配。MPI_BYTE是将消息不加修改的通过二进制字节流来传递的一种方式，而MPI_PACKED是为了将非连续的数据进行打包发送而提出的。经常与函数`MPI_Pack_size`和`MPI_Pack`联合使用。

下面是MPI_PACKED的使用代码：

```
#include"mpi.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXLEN 512

int main() {
  int myid, namelen;
  MPI_Status status;
  char name[MPI_MAX_PROCESSOR_NAME], buf[MAXLEN];
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(name, &namelen);
    printf("processor %d is started on %s\n", myid, name);
    if(myid == 0) {
        double A[100];
        int buffersize;
        MPI_Pack_size(50, MPI_DOUBLE, MPI_COMM_WORLD, &buffersize);
        void* tempbuffer = malloc(buffersize);
        int j = sizeof(MPI_DOUBLE);
        int position = 0;
        for(int i = 0; i < 100; i++) A[i] = i * 1.1;
        printf("position : %d\n", position);
        for(int i = 0; i < 50; i++)
          MPI_Pack(A+i*2, 1, MPI_DOUBLE, tempbuffer, buffersize, &position, MPI_COMM_WORLD);
        MPI_Send(tempbuffer, position, MPI_PACKED, 1, 101, MPI_COMM_WORLD);
        free(tempbuffer);
    }
    if(myid == 1) {
      void* B = malloc(MAXLEN);
      MPI_Recv(B, MAXLEN, MPI_PACKED, 0, 101, MPI_COMM_WORLD,&status);
      int num;
      MPI_Get_count(&status, MPI_PACKED, &num);
      printf("%d\n", num);
      double* C = (double*)B;
      for(int i = 0; i < 50; i++) {
        printf("%lf\n", C[i]);
      }
      free(B);
    }
  MPI_Finalize();
  return 0;
}
```

- `MPI_Pack_size`
  - 决定需要多大的缓冲区来存放数据
  - `MPI_Pack_size(num, datatype, comm, buffersize)`
  - 这里是通过MPI_Pack_size来计算num个datatype数据所需要的内存，其结果存放在buffersize，注意buffersize给的是整型指针，comm就是通信域

- `MPI_Pack`
  - `MPI_Pack(buf, sum, datatype, tempbuffer, buffersize, &position, comm)`
  - buf是所要打包的数据的起始位置(指针or地址)，第二个参数是打包几个数据，第三个参数是说这回的数据的种类，第四个参数tempbuffer是要打包的地方，buffersize是缓冲区大小，第五个参数用于跟踪已经有多少个数据被打包(同时也作为地址偏移量，本质上也是第一个数据开始存放的地方)，第六个就是通信域

##### **导出数据类型**
 MPI还允许通过导出数据类型，将不连续的，甚至是不同类型的数据元素组合在一起形成新的数据类型。我们称这种由用户定义的数据类型为到此处数据类型。这需要由MPI提供的构造函数来构造。

 总之类型匹配规则如下：

 - 有类型数据的通信，发送方和接收方均使用相同的数据类型
 - 无类型数据的通信，发送方和接收方均以MPI_BYTE作为数据类型
 - 打包数据的通信，发送方和接收方均使用MPI_PACKED


 ### **消息标签TAG**

TAG是消息信封中的一项，是程序在同一接收者的情况下，用于标识不同类型消息的一个整数。

```
#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define MAXN 512

int main() {
  int myid, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);
    printf("processor %d running on %s\n", myid, processor_name);
    if(myid == 0) {
      char message[MAXN];
      strcpy(message, "hello, I'm processor 0\n");
      printf("processor 0 sending message: %s", message);
      fflush(stdout);
      MPI_Send(message, strlen(message)+1, MPI_CHAR, 2, 101, MPI_COMM_WORLD);
      strcpy(message, "goodbye, I'm processor 0\n");
      printf("processor 0 sending message: %s", message);
      fflush(stdout);
      MPI_Send(message, strlen(message)+1, MPI_CHAR, 2, 110, MPI_COMM_WORLD);
    }
    if(myid == 2) {
      char message[MAXN];
      MPI_Recv(message, MAXN, MPI_CHAR, 0, 101, MPI_COMM_WORLD, &status);
      printf("processor 2 received message: %s", message);
      fflush(stdout);
      MPI_Recv(message, MAXN, MPI_CHAR, 0, 110, MPI_COMM_WORLD, &status);
      printf("processor 2 received message: %s", message);
      fflush(stdout);
    }
  MPI_Finalize();
  return 0;
}
```

如果上述的例子假设没有标签的化，那么有可能进程0发送的第二个信息如果比第一个信息块，那么进程2接收的就是第二个信息，如果此时存储的地方不一样，就会导致消息沟通的错误，所以我们需要消息标签来进行区别。

### **通信域**

消息的发送和接收必须使用相同的消息标签才能实施通信。维护TAG来匹配消息是比较繁琐的事情，因此我们同时提出了另一项通信域。

一个通信域包含一个进程组及其上下文。进程组是进程的有限有序集。有限是说进程的数量是有限的，有序是编号是从0~n-1。

通信域限定了消息传递的进程范围。

一个进程在一个通信组中，用它的编号进行标识，组的大小和进程号可以用前面所说的`MPI_Comm_size`和`MPI_Comm_rank`获得。

MPI预先定义了两个进程组：MPI_COMM_SELF(只包含自己的通信域)和MPI_COMM_WORLD(包含所有MPI进程的进程组)，同时，MPI对于通信子(通信组)提供了各种管理函数。

- `int MPI_Comm_compare(comm1, comm2, result)`

其中result是整型指针的传递，这里比较comm1和comm2，如果comm1和comm2是相同的句柄，则result为MPI_Ident(感觉上是一个整型，但是实测的时候没法打印，反正该函数通过result值得不同来表示结果)，如果仅仅是个进程组得成员和序列号都相同，则result为MPI_Congruent，如果两者得组成员相同但序列号不同则结果为MPI_Similar，否则结果就为MPI_Unequal

- `int MPI_Comm_dup(comm, newcomm)`

对comm进行复制得到新的通信域newcomm，注意这边得newcomm是通过指针传递的，类型为MPI_Comm*

- `int MPI_Comm_solit(comm, color, key, newcomm)`

通信域分裂，本函数要求comm进程组中的每个进程都要执行，每个进程指定一个color(整型)，如果具有相同的color值的进程形成一个新的进程组，新产生的通信域与这些进程组一一对应。

```
#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define MAXN 512

int main() {
  MPI_Comm a;
  MPI_Status status;
  int myid, numprocs;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("MPI_COMM_WORLD:%d\n", myid);
    MPI_Comm_split(MPI_COMM_WORLD, myid%2, myid, &a);
    MPI_Comm_size(a, &numprocs);
    printf("%d\n", numprocs);
    MPI_Comm_rank(a, &myid);
    printf("a:%d\n", myid);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    if(myid == 0) {
       char buf[MAXN];
       strcpy(buf, "hello world from 0\n");
       printf("processor 0 sending : %s", buf);
       MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 1, 110, a);
    }
    if(myid == 2) { // 这里发现0和2是一组，0和3不是一组
      char buf[MAXN];
      MPI_Recv(buf, MAXN, MPI_CHAR, 0, 110, a, &status);
      printf("%s", buf);
    }
  MPI_Finalize();
  return 0;
}
```

注意新产生的通信域包含旧的所有进程，只是不同的进程可能在不同的组别之中。新的进程组中，各个进程的顺序编号根据key(整型)的大小决定，如果key越小，则相应进程在新通信域中的顺序编号也越小，如果key值相同，则根据这两个进程在原来通信域中顺序号决定新的进程号。一个进程可能提供color值为MPI_Undefined，此时，newcomm返回MPI_COMM_NULL(分裂失败)

- `int MPI_Comm_free(comm)`

释放给定的通信域，注意这里传递的是指针

### **状态字(status)**

状态字的主要功能就是保存接收到的消息的状态。

```
while(true) {
  MPI_Recv(..., ..., ..., MPI_ANY_SOURCE, MPI_ANY_TAG,...,...);
  switch(status.MPI_TAG) {
    case 0: ...;
    case 1: ...;
    case 2: ...;
  }
}
```

这里的MPI_Recv没有指定从哪里接收信息，可以接收任意来源的信息，任意标签的信息(MPI_ANY_TAG)，我们可以通过检查status中的MPI_TAG可以有效把消息区分开来。当一个接收者能从不同进程接收不同大小和标签的消息时，比如服务器进程，查阅状态信息就会很有用。我们可以利用状态字的标签可以进行更多的有意思的操作。

### **通信匹配圣经**

 - 通信数据类型匹配
 - 消息标签，通信域匹配
 - 发送进程与接收进程号对应
 - 接收消息的缓冲区大于发送过来的消息的大小

 现在考虑如果当初的信息大家都是先接收然后再发送，程序会怎么样呢？运行后会发现，程序进入了停滞状态，此时0，1，2，3都是在receiving状态，而这时候没有进程可以发送消息来结束这个状态，**这种大家都在等待的状态，称为“死锁”**，死锁现象在多进程，多线程编程中是经常发生的现象。 因为MPI_Send或MPI_Recv正确返回的前提是该通信操作已经完成。对于发送操作来说就是缓冲区可以被其他的操作更新，对于接收操作来说就是该缓冲区中的数据已经可以被完整的使用。我们称这样的形式为阻塞通信，如果没有完成之前，其不会结束该次通信操作。当然反过来，先发送再接收是可以执行下去的，因为发送操作不需要等待其他的先行操作，因此阻塞可以是有限的。阻塞通信中点对点消息的匹配也对正确通信有着至关重要的影响。


 ### **统计时间**

 编写并行程序的目的是为了提高程序运行性能。为了检验并行化的效果，我们经常会用到统计时间的函数。MPI提供两个时间函数`MPI_Wtime`和`MPI_Wtick`

 - `MPI_Wtime`返回一个双精度数，标识从过去的某点时间到当前时间所消耗的时间秒数

 - `MPI_Wtick`返回`MPI_Wtime`结果的精度

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<string.h>

 #define BUFLEN 512

 int main(int argc, char* argv[]) {
   int myid, numprocs, next, namelen;
   char buffer[BUFLEN], processor_name[MPI_MAX_PROCESSOR_NAME];
   MPI_Status status;
   double t1, t2, t3, tick;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Get_processor_name(processor_name, &namelen);

   t1 = MPI_Wtime();

   printf("Processor %d on %s\n", myid, processor_name);
   printf("Processor %d of %d\n", myid, numprocs);
   memset(buffer, 0, BUFLEN*sizeof(char));
   if(myid == numprocs-1)
     next = 0;
   else
     next = myid + 1;

   if(myid == 0) {
     strcpy(buffer, "hello there");
     printf("%d sending '%s'\n", myid, buffer); fflush(stdout);
     MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);
     printf("%d receiving\n", myid); fflush(stdout);
     MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
     printf("%d received '%s'\n", myid, buffer); fflush(stdout);
   }else{
     printf("%d receiving\n", myid); fflush(stdout);
     MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
     printf("%d received '%s'\n", myid, buffer); fflush(stdout);
     MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);
     printf("%d sent '%s'\n", myid, buffer); fflush(stdout);
   }

   t2 = MPI_Wtime();
   t3 = t2 - t1;
   tick = MPI_Wtick();
   printf("%d process time is '%.10f'\n", myid, t3);
   printf("%d process tick is '%.10f'\n", myid, tick);
   MPI_Finalize();
   return 0;
 }
 ```

 其实本质上和前面的时钟打点函数的用法差不多，这里MPI_Wtime就是获得程序当前运行了多少时间，而MPI_Wtick就是获得计时的精度。


 ### **错误管理**

 - 通过`status.MPI_ERROR`来获取错误码

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<string.h>

 #define BUFLEN 512

 int main() {
   int myid;
   MPI_Status status;
   char buf[BUFLEN];
   MPI_Init(NULL, NULL);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     printf("processor %d running\n", myid);
     if(myid == 0) {
       strcpy(buf, "hello, processor 1 from processor 0");
       printf("processor %d sending %s\n", myid, buf); fflush(stdout);
       MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 1, 101, MPI_COMM_WORLD);
     }
     if(myid == 1) {
       MPI_Recv(buf, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
       printf("processor %d received %s\n", myid, buf); fflush(stdout);
       printf("tag %d source %d\n", status.MPI_TAG, status.MPI_SOURCE);
       printf("error code %d\n", status.MPI_ERROR);
     }
   MPI_Finalize();
   return 0;
 }
 ```

 - MPI终止MPI程序执行的函数`MPI_Abort`

 `int MPI_Abort(MPI_Comm, int errorcode)`

 该函数的作用使通信域comm的所有进程退出，返回errorcode给调用的环境。通信域comm中的任意进程调用此函数都能使该通信域内所有的进程结束运行。这里只要执行到这个代码，那么所有的进程都会结束，类似于抛出异常的处理机制。

 接下来进入本章的最后一个环节啦，加油。


 ## **MPI群集通信**

 除了之前介绍的点对点通信，MPI还有群集通信。群集通信，说白了就是包含一对多，多对一，多对多的进程通信模式(就是不带一对一玩，但其实本质上就是多对多，因为一对多和多对一不过是多对多的特例)。此时的通信方式变成了多个进程参与通信。

 ### **同步**

 `int MPI_Barrier(MPI_Comm comm)`

 如下面这段代码，如果没有MPI_Barrier，那么进程运行快的会直接执行下面的代码，而有的进程还没有执行第一行的输出。

 ```
 #include"mpi.h"
 #include<stdio.h>

 int main() {
   int myid;
   MPI_Init(NULL, NULL);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     printf("processor %d running\n", myid); fflush(stdout);
     MPI_Barrier(MPI_COMM_WORLD);
     printf("hello world %d\n", myid); fflush(stdout);
   MPI_Finalize();
   return 0;
 }
 ```

 这个函数就像是一道路障。使得通信子comm中的所有进程相互同步，知道所有的进程都执行了他们各自的MPI_Barrier函数，然后各自开始执行后面的代码。同步函数是并行程序中控制执行顺序的常用手段。(本质上就是强迫所有在通信子comm中的进程，重新在Barrier那一行一起进行，让某些线程达到同步，此时有点串行的味道)

 ### **广播**

 广播就是一对多的传送消息，从一个root进程向组内所有其他的进程发送一条消息。

 `int MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root,MPI_Comm)`

 相比于之前的MPI_Send，MPI_Bcast就是少了目标进程，此时的目标进程扩大为组内的所有进程。

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<string.h>

 #define BUFLEN 512

 int main() {
   int myid, numprocs, namelen;
   char buf[BUFLEN], Buf[BUFLEN], name[MPI_MAX_PROCESSOR_NAME];
   MPI_Init(NULL, NULL);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     MPI_Get_processor_name(name, &namelen);
     printf("%d of %d running on %s\n", myid, numprocs, name); fflush(stdout);
     memset(buf, 0, sizeof(buf));
     memset(Buf, 0, sizeof(Buf));
     if(myid == 0) {
       strcpy(buf, "hello, I\'m processor 0\n");
     }
     printf("processor %d\'s buf : %s", myid, buf); fflush(stdout);
     printf("\nMPI_Bcast is started\n"); fflush(stdout);
     if(myid == 0) MPI_Bcast(buf, strlen(buf)+1, MPI_CHAR, 0, MPI_COMM_WORLD);
     MPI_Bcast(Buf, BUFLEN, MPI_CHAR, 0, MPI_COMM_WORLD);
     printf("processor %d\'s now buf : %s", myid, Buf); fflush(stdout);
   MPI_Finalize();
   return 0;
 }
 ```

 用法如上，本质上和Recv和Send很相似，不过没有了tag，同时MPI_Bcast广播本身可以做发送和接收，如果当前进程号等于root，那就是发送，否则就是接收。

 ### **聚集**

 `int MPI_Gather(void* sendbuf, int sendcnt, MPI_Datatype sendtype, void* recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm)`

 该函数的作用就是root进程接收该通信组每一个成员进程(包括root自己)发送的信息。这n个消息的连接按进程号排列存放在root进程的接收缓冲中。每个缓冲由三元组(sendbuf, sendcnt, sendtype)标识。所有非root进程忽略接收缓冲。跟多的是接收的作用，只不过此时接收的是其他进程中发送过来的信息。

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<string.h>

 #define BUFLEN 512

 int main() {
   int myid, numprocs, namelen;
   char name[MPI_MAX_PROCESSOR_NAME], buf[BUFLEN], BUF[BUFLEN];
   MPI_Init(NULL, NULL);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     MPI_Get_processor_name(name, &namelen);
     printf("%d of %d running on %s\n", myid, numprocs, name); fflush(stdout);
     sprintf(buf, "hello, I\'m processor %d.", myid);
     printf("%s\n", buf); fflush(stdout);
     int len = strlen(buf);
     MPI_Gather(buf, len, MPI_CHAR, BUF, len, MPI_CHAR, 1, MPI_COMM_WORLD);
     //MPI_Barrier(MPI_COMM_WORLD);
     printf("processor %d\'BUF is %s\n", myid, BUF); fflush(stdout);
   MPI_Finalize();
   return 0;
 }
 ```

 MPI_Gather注意这边的函数sendcnt和recvcnt要匹配。如果不相等可能会造成通信错误，其实质就是运行这些函数的进程开始相互通讯。注意该函数自带有barrier的功能。

 ### **播撒**

 `int MPI_Scatter(void* sendbuf, int sendcnt, MPI_Datatype sendtype, void* recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm)`

 MPI_scatter是一对多传递消息。和广播不同的是，root进程向各个进程传递的消息可以是不同的。Scatter实际上执行的是与Gather相反的操作。

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<string.h>

 #define BUFLEN 512

 int main() {
   int myid, numprocs, namelen;
   char processor_name[MPI_MAX_PROCESSOR_NAME], buf[BUFLEN], BUF[BUFLEN];
   MPI_Status status;
   MPI_Init(NULL, NULL);
     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     MPI_Get_processor_name(processor_name, &namelen);
     printf("%d of %d running on %s\n", myid, numprocs, processor_name);
     memset(buf, 0, sizeof(buf));
     if(myid == 0) strcpy(buf, "hello, I\'m processor 0");
     printf("processor %d buf %s\n", myid, buf); fflush(stdout);
     int len = strlen(buf), next = (myid + 1) % numprocs;
     MPI_Barrier(MPI_COMM_WORLD);
     if(myid == 0) {
       MPI_Send(&len, 1, MPI_INT, next, 101,  MPI_COMM_WORLD);
       MPI_Recv(&len, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
     }else{
       MPI_Recv(&len, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
       MPI_Send(&len, 1, MPI_INT, next, 101,  MPI_COMM_WORLD);
     }
     MPI_Barrier(MPI_COMM_WORLD);
     printf("processor %d len %d\n", myid, len); fflush(stdout);
     MPI_Scatter(buf, len/4, MPI_CHAR, BUF, len/4, MPI_CHAR, 0, MPI_COMM_WORLD);
     printf("processor %d BUF %s\n", myid, BUF); fflush(stdout);
   MPI_Finalize();
   return 0;
 }
 ```

 注意方便起见，建议这里的sendcnt和recvcnt保持一直，同时注意这里的recvcnt是表示每个进程接收的数量，而不是发送的总数量，注意这个区别，类似于一种分配块中任务的数量。当然root进程可以给自己发送信息。

 ### **扩展的聚集和播撒操作**

 > `MPI_Allgather`的作用是每一个进程都收集到其他所有进程的消息，它相当于每一个进程都执行了MPI_Gather执行完了MPI_Gather之后，所有的进程的接收缓冲区的内容都是相同的，也就是说每个进程给所有进程都发送了一个相同的消息，所以名为`allgather`。本函数的接口是：

 `int MPI_Allgather(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)`

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<string.h>

 #define BUFLEN 512

 int main() {
   int myid, numprocs, namelen;
   char buf[BUFLEN], BUF[BUFLEN], name[MPI_MAX_PROCESSOR_NAME];
   MPI_Init(NULL, NULL);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     MPI_Get_processor_name(name, &namelen);
     printf("processor %d of %d running on %s\n", myid, numprocs, name);
     memset(buf, 0, sizeof(buf));
     memset(BUF, 0, sizeof(BUF));
     sprintf(buf, "hello, I'm processor %d", myid);
     MPI_Allgather(buf, strlen(buf), MPI_CHAR, BUF, strlen(buf), MPI_CHAR, MPI_COMM_WORLD);
     printf("processor %d get message : %s\n", myid, BUF); fflush(stdout);
   MPI_Finalize();
   return 0;
 }
 ```

 ### **全局交换**

 > `MPI_Allgather`每个进程发送一个相同的消息给所有的进程，而`MPI_Alltoall`散发给不同进程的消息是不同的。因此，它的发送缓冲区也是一个数组。`MPI_Alltoall`的每个进程可以向每个接收者发送数目不同的数据，第i个进程发送的第j块数据将被第j 个进程接收并存放在其他消息缓冲区recvbuf的第i块，每个进程的sendcount和sendtype的类型必须和所有其他进程的recvcount和recvtype相同，这也意味着在每个进程和根进程之间发送的数据量必须和接收的数据量相等。函数接口为：

 `int MPI_Alltoall(void* sendbug, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype, MPI_Comm comm)`

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<string.h>

 #define BUFLEN 512

 int main() {
   int myid, numprocs, namelen;
   char processor_name[MPI_MAX_PROCESSOR_NAME], buf[BUFLEN], BUF[BUFLEN];
   MPI_Init(NULL, NULL);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     MPI_Get_processor_name(processor_name, &namelen);
     printf("%d of %d running on %s\n", myid, numprocs, processor_name); fflush(stdout);
     sprintf(buf, "I\'m processor %d, hello!", myid);
     printf("processor %d : %s\n", myid, buf); fflush(stdout);
     memset(BUF, 0, sizeof(BUF));
     int len = strlen(buf);
     MPI_Alltoall(buf, len/numprocs, MPI_CHAR, BUF, len/numprocs, MPI_CHAR, MPI_COMM_WORLD);
     printf("processor %d get message: %s\n", myid, BUF);
   MPI_Finalize();
   return 0;
 }
 ```

 ### **规约与扫描**

 MPI提供了两种类型的聚合操作

 #### **规约**

 `int MPI_Reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)`

 这里的每个进程的待处理数据存放在sendbuf中，可以是标量也可以是向量。所有进程将这些值通过输入的操作子op计算为最终结果并将它存入root进程的recvbuf中。具体的规约操作包括：

 |操作子|功能|
 |:---:|:--:|
 |`MPI_MAX`|求最大值|
 |`MPI_MIN`|求最小值|
 |`MPI_SUM`|求和|
 |`MPI_PROD`|求积|
 |`MPI_LAND`|逻辑与|
 |`MPI_BAND`|按位与|
 |`MPI_LOR`|逻辑或|
 |`MPI_BOR`|按位或|
 |`MPI_LXOR`|逻辑异或|
 |`MPI_BXOR`|按位异或|
 |`MPI_MAXLOC`|最大值且对应的位置|
 |`MPI_MINLOC`|最小值且相应的位置|

 规约操作的数据类型与C中的整数类型对应。

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<time.h>
 #include<stdlib.h>
 #include<string.h>

 #define LEN 10
 #define BASE 1000

 int main() {
   int myid, numprocs, num[LEN], out[LEN];
   srand(time(NULL));
   MPI_Init(NULL, NULL);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     for(int i = 0; i < LEN; i++) num[i] = 10*myid + i;
     printf("processor %d array: ", myid);
     for(int i = 0; i < LEN; i++) printf("%d ", num[i]);
     printf("\n");fflush(stdout);
     memset(out, 0, sizeof(out));
     MPI_Barrier(MPI_COMM_WORLD);
     MPI_Reduce(&num, &out, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
     printf("processor %d array:", myid);
     if(out[0] == 0) printf("no\n");
     else{
       for(int i = 0; i < 10; i++) printf("%d ", out[i]);
       printf("\n");
     }
     fflush(stdout);
   MPI_Finalize();
   return 0;
 }
 ```

 注意这里的数据量count指的是几个数据参加，而这边的操作其实是对所有线程的第i个数据进行的，所以传递的时候接收的就是经历这些操作过后留下来的数据。化多线程为一个线程上的数据，归一。

 #### **扫描**

 `int MPI_Scan(void* sendbuf, void* recvbuf, int count, MPI_Datatype, MPI_Op op, MPI_Comm comm)`

 MPI_Scan常用于对分布于族中的数据做前置规约操作。此操作将序列号为0,...,i(包括i)的进程发送缓冲区的规约结果存入序列号为i的进程接收消息缓冲区中。这种操作支持的数据类型，操作以及对发送及接收缓冲区的限制和规约相同。与规约相比，扫描操作设过去了root域，因为扫描是将部分值组合成n个最终值，并存放在n个进程的recvbuf中。具体的扫描操作有Op域定义。

 MPI的规约和扫描操作允许每个进程贡献向量值，而不只是标量值。向量的长度由Count定义。

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<time.h>
 #include<stdlib.h>
 #include<string.h>

 #define LEN 10
 #define BASE 1000

 int main() {
   int myid, numprocs, num[LEN], out[LEN];
   srand(time(NULL));
   MPI_Init(NULL, NULL);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     for(int i = 0; i < LEN; i++) num[i] = 10*myid + i+1;
     printf("processor %d array: ", myid);
     for(int i = 0; i < LEN; i++) printf("%d ", num[i]);
     printf("\n");fflush(stdout);
     memset(out, 0, sizeof(out));
     MPI_Barrier(MPI_COMM_WORLD);
     MPI_Scan(&num, &out, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
     printf("processor %d array:", myid);
     if(out[0] == 0) printf("no\n");
     else{
       for(int i = 0; i < 10; i++) printf("%d ", out[i]);
       printf("\n");
     }
     fflush(stdout);
   MPI_Finalize();
   return 0;
 }
 ```

 与规约是类似的不过就是这里的最终结果一定存放在最后一个进程中，同时注意随着进程号的迭代，里面的进程中的最后一个存放当前的结果。比如对于四进程来说，第二个进程存放一二进程中op操作子过后的值。

 ### **简单示例**

 相关代码如下：

 ```
 #include"mpi.h"
 #include<stdio.h>
 #include<math.h>

 double f(double);

 double f(double a) {
     return (4.0 / (1.0 + a*a));
 }

 int main(int argc, char* argv[]) {
     int n, myid, numprocs, i;
     double PI25DT = 3.141592653589793238462643;
     double mypi, pi, h, sum, x;
     double starttime = 0.0, endwtime;
     int namelen;
     char processor_name[MPI_MAX_PROCESSOR_NAME];

     MPI_Init(&argc, &argv);
     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     MPI_Comm_rank(MPI_COMM_WORLD, &myid);
     MPI_Get_processor_name(processor_name, &namelen);

     fprintf(stdout, "Process %d of %d is on %s\n", myid, numprocs, processor_name);
     fflush(stdout);
     n = 10000;
     if(myid == 0)
       starttime = MPI_Wtime();
     MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
     h = 1.0/(double)n;
     sum = 0.0;
     for(i = myid+1; i <= n; i += numprocs){
       x = h * ((double)i - 0.5);
       sum += f(x);
     }
     mypi = h * sum;
     MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
     if(myid == 0) {
       endwtime = MPI_Wtime();
       printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi-PI25DT));
       printf("wall clock time = %f\n", endwtime-starttime);
       fflush(stdout);
     }
     MPI_Finalize();
     return 0;
 }
 ```

 这里本质上利用的是积分求pi，1/(1+x^2)的积分是arctanx，通过这种方式来实现。最后通过规约操作中的求和将四个线程的内容相加就可以了，也就是将其中的操作基本上分成四等分，然后来求解。

 ### **小结**

 - 通信子中的所有进程必须调用群集通信历程。如果有意个进程没有调用，会产生奇奇怪怪的错误。
 - 一个进程一旦结束了群集操作就从群集例程中返回。
 - 每个群集历程，也就是前面的群集函数都有阻塞的功能

 ****
 MPI入门到此ending。完结撒花，感谢陪伴。

 > 何当共剪西窗烛，却话巴山夜雨时。

 江湖再会，哈哈哈。
