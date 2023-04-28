# 在Windows平台安装Parmetis库

## 下载最新版本parmetis-4.0.3

可以去官网下载最新的Parmetis库发布版，这里面包含了metis库等除MPI外的一系列依赖，但这个网站经常会down掉。所以可以去GitHub的镜像站进行下载。

```html
http://glaros.dtc.umn.edu/gkhome/metis/parmetis/download

https://github.com/wildmeshing/ParMETIS	//镜像站
```

## 添加CMAKE编译选项

1. 打开cmake-gui，指定parmetis根目录位置和build文件生成位置，例如

> C:\parmetis-4.0.3
> C:\parmetis-4.0.3\build

2. 点击add Entry，添加以下条目。这里需要事先安装好Windows中的MPI SDK，可以去官网进行下载。安装好后默认位置在 `C:\Program Files (x86)\Microsoft SDKs\MPI` 中，把include和lib文件夹添加到下面编译条件中就行。

| Name                 | Type   | Value                |
| -------------------- | ------ | -------------------- |
| MPI_C_INCLUDE_PATH   | PATH   | MPI头文件目录        |
| MPI_C_LIBRARIES      | STRING | 库文件（带绝对路径） |
| CMAKE_INSTALL_PREFIX | PATH   | 安装目录             |

3. 打开根目录下的CMakeLists.txt文件，在Project(ParMETIS)之后添加一行

```cmake
find_package(MPI)
```

4. 依次点击configure和generate进行配置



## 修改metis中冲突的源代码

1. 更改整型和浮点型精度，本框架其他库均已长整型进行编译，因此此处整型和浮点型精度均设置位64位

   > 打开根目录下metis/include/metis.h文件，查找IDXTYPEWIDTH和REALTYPEWIDTH关键字，设置为64

2. 打开根目录下的metis/GKlib/gk_arch.h文件，查找下面语句的第一个出现位置

   ```c
   #ifdef __MSC__
   ```

   替换为

   ```c
   #if _MSC_VER < 1700
   ```

   并将以下两行注释

   ```c
   #ifndef SUNOS
     #include <stdint.h>
   #endif
     #include <inttypes.h>
     #include <sys/types.h>
     //#include <sys/resource.h>
     //#include <sys/time.h>
   #endif
   ```

3. 如果出现`Error C2059: syntax error : '('` 在 `CRTIMP double __cdecl rint(_In_ double _X);` ，这是和windows中的函数数学库函数冲突了，需要将metis/GKlib/gk_arch.h文件中的 `#define rint(x) ((idx_t)((x)+0.5))` 注释掉

   ```c
    // #define rint(x) ((idx_t)((x)+0.5))
   ```
   
   如果不够的话，可以将 metisbin.h 和 metislib.h 中的也一起注释掉。



然后再打开编译，一遍就能通过了。