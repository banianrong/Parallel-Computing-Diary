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

拷贝下面程序，然后在dev运行，记着是点击**Tools**，然后选择里面的**Package Manager**，点击自己刚刚搭建的**MPI RUN FOR 4**，就可以测试自己的mpi环境搭建的如何了。

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
