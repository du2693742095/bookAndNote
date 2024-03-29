# CGNS简介

## 概述

CFD通用标记系统(CFD General Notation System) ，既是一种文件格式规范，也表示用于读写这种文件格式的相应的开源软件库。使用CGNS格式，可方便CFD数据交换，存储及应用，避免现有不同CFD软件交换数据时可能会出现的歧义。CGNS格式描述了一个CFD算例，其内容包括：

>  网格、流场、边界条件；
>
>  拓扑链接信息；
>
>  辅助数据（如无量纲参数，参考状态）

通过任意的CFD软件工具，如解算器、网格生成器、流场显示、后处理，可存储上述格式文件对应部分，编辑相应数据。

CGNS的重要特点是数据可包含解释性注释或描述信息（单从文件本身就可以知道文件内数据的意义）。此外，数据是分层结构化存储的，而且是基于二进制存储的，普通的文件打开对于CGNS来说没有意义。

- [SIDS](#SIDS) 给出了这种文件格式的抽象定义。
- [MLL](#MLL) 是对 SIDS 的一种具体实现，它提供了读写（以 ADF 或 HDF5 作为底层数据格式的）CGNS 文件的 Fortran 及 C 语言 API。
- 文档中常用缩写含义如下：

| 术语 |                             含义                             |
| :--: | :----------------------------------------------------------: |
| ADF  | [*A*dvanced *D*ata *F*ormat](https://cgns.github.io/CGNS_docs_current/adf) |
| CFD  |               *C*omputational Fluid *D*ynamics               |
| CGNS |    [CFD General Notation System](https://cgns.github.io/)    |
| CPEX |               *C*GNS *P*roposal for Extension                |
| FMM  |                  *F*ile *M*apping *M*anual                   |
| HDF  | [*H*ierarchical *D*ata *F*ormat](https://cgns.github.io/CGNS_docs_current/hdf5) |
| MLL  | [中级库, 官方提供的访问CGNS文件的库](https://cgns.github.io/CGNS_docs_current/midlevel/) |
| SIDS | [*S*tandard *I*nterface *D*ata *S*tructures](https://cgns.github.io/CGNS_docs_current/sids/) |

## 存储文件格式

CGNS只是一个规范，它使用ADF、HDF5或Python存储数据，数据结构由SIDS（Standard Interface Data Structures）来规定。

- `ADF`(高级数据格式)文件由node组成，每个node可以带有数据，也可以不带数据。node组成一种tree结构，这样非常便于检索数据。标准接口数据结构SIDS定义了节点类型，给出文件组成结构上的定义。也就是说，SIDS定义了ADF文件的节点代表的意义、格式。

- `HDF5`比ADF多了并行I/O（MPI-IO）和压缩的功能，而且受到科学界更广泛的支持。但是HDF5目前的缺点在于节点数目较多时文件读写较慢，文件大小一般比ADF大。因此，CGNS委员会不会强制使用HDF5，并且提供了两种格式的自由转换。



# SIDS文件格式的抽象定义

## 官方文档

- 入门指南：《[Overview of the SIDS](https://cgns.github.io/CGNS_docs_current/user/sids.html)》
- 完整定义：《[CGNS Standard Interface Data Structures](https://cgns.github.io/CGNS_docs_current/sids/index.html)》

## 文件读写

符合 SIDS 规范的 CGNS 文件（数据库）是按 ADF 或 HDF5 编码的，因此无法用普通的文本编辑器读写，但可以用 [CGNSview](https://cgns.github.io/CGNS_docs_current/cgnstools/cgnsview/)、[HDFView](https://www.hdfgroup.org/downloads/hdfview/) 等工具安全地读写，其操作类似于在操作系统中访问文件树。

如果要构建上述工具，则需安装以下依赖库，并在 CMake 中将 `CGNS_BUILD_CGNSTOOLS` 设为 `ON`：

```shell
sudo apt install libx11-dev libxmu-dev
sudo apt install libopengl-dev libglu1-mesa-dev libgl1-mesa-dev 
sudo apt install libtcl8.6 tk8.6-dev
```

 Gmsh、VTK 只能打开比它们所依赖的 CGNS MLL库更旧的 CGNS 文件。

## 示例文件

- [CGNS官方.case文件](http://cgns.github.io/CGNSFiles.html)
- [AIAA High Lift Workshop (HiLiftPW)](https://hiliftpw.larc.nasa.gov)
  - [NASA High Lift Common Research Model (HL-CRM) Grids](https://hiliftpw-ftp.larc.nasa.gov/HiLiftPW3/HL-CRM_Grids/)
  - [JAXA Standard Model (JSM) Grids](https://hiliftpw-ftp.larc.nasa.gov/HiLiftPW3/JSM_Grids/)
- [AIAA Drag Prediction Workshop (DPW)](https://aiaa-dpw.larc.nasa.gov) 
  - [Download Grids](https://dpw.larc.nasa.gov/DPW6/)

## 树状结构

每个 CGNS 文件（数据库）在逻辑上是一棵由若干**结点 (node)** 相互链接而成的**树 (tree)**。每个结点都含有以下数据：

- `Label` 表示其类型，通常是以 `_t` 为后缀的预定义类型, 例如Element_t。
- `Name` 描述节点及其数据的附加字段，通常是由用户自定义的字符串，但有时需符合命名规范, 例如不能和已有的规定结构重名。
- `Data` 是实际数据，可以为空（用 `MT` 表示）。
- `子节点ID` 指向其**父结点 (parent node)** 或**子结点 (child node)** 的链接。

为避免混淆，称树上的结点为**对象 (object)**，而将网格中的点称为**顶点 (vertex)** 或**网格点 (mesh/grid point)**。

其基本结构为:

[![pSXOLkR.png](https://s1.ax1x.com/2023/02/21/pSXOLkR.png)](https://imgse.com/i/pSXOLkR)



# MLL读写中级库

## 官方文档

- 入门指南：《[A User's Guide to CGNS](https://cgns.github.io/CGNS_docs_current/user/)》
- 完整定义：《[Mid-Level Library](https://cgns.github.io/CGNS_docs_current/midlevel/)》
- 并行版本：《[Parallel CGNS Routines](https://cgns.github.io/CGNS_docs_current/pcgns/)》

## 编译选项

这些编译选项的选择会导致后面cmake调用生成脚本时生成不同的代码, 体现在cgnstypes.h文件中.

### 64位版本

64位的CGNS版本可以使得计算机处理超过**20亿坐标或元素**问题, 如果只有32位版本, 则早期的文件最大只能有2/4GB, 超过则无法寻址导致读取失败. 但64位版本不兼容以前的网格与代码, 只能读取以64位程序构建的CGNS文件.

- 64位版本从CGNS3.1开始支持, 需要使用cmake参数 `--enable-64bit` 或者修改cmake代码 `CG_BUILD_64BIT=ON` , 设置后对应的 `CG_BUILD_LEGACY` 会置为OFF.

- 如果想兼容32位版本, 则需要使用编译选项 `CG_BUILD_LEGACY=ON` , 对应的 `CG_BUILD_64BIT` 和 `CG_BUILD_SCOPE` 都需要置为OFF, 编译出来的就只是以前32位的版本.

### 并行版本

并行版的 CGNS/MLL 依赖于并行版的 HDF5，后者依赖于 MPI。安装好MPI后，即可在构建 CGNS/MLL 时，cmake参数如下. 或修改cmake代码将 `CGNS_ENABLE_HDF5=ON` , 然后将 `HDF5_NEED_MPI=ON` . 编译出来的CGNS并行库被称为PCGNS, 一起存在于cgns.lib中, 对应头文件是pcgns.lib.

```shell
cmake -D CMAKE_BUILD_TYPE=Debug \
      -D CGNS_ENABLE_HDF5=ON \ # 并行版本必须启用 HDF5
      -D HDF5_NEED_MPI=ON -D CGNS_ENABLE_PARALLEL=ON \ # 启用并行版本
      -G Ninja -B ${CGNS_BUILD_DIR} -S ${CGNS_SOURCE_DIR}
```

### 枚举值范围

当启用 `CG_BUILD_SCOPE=ON` 时, 将为所有枚举添加前缀`CG_` , 这个选项一般用于32位程序, 用于不同版本间做区分.

## CGNS的版本及兼容性问题

- 3.1.0版本: 这个版本新增了64位支持, 可以实现超大文件读取, 需要HDF5也是64位支持. 构建 64 位代码时，ADF 文件格式会发生变化, 所有维度都存储为 64 位整数，HDF5 接口也会受到维度大小的影响。3.1 版的 32 位编译也能够读取 64 位 CGNS 文件，但如果大小超过 32 位最大尺度则失败。采用32位模式编译，ADF版本和HDF5接口不变，无论 3.1 版软件如何编译，它都向后兼容早期版本的 CGNS 文件。
- 3.3.0版本: 为Fortran调用做了可移植性适配, 需要使用Fortran的必须编译3.3.0以上版本. 不支持混合单元的读取

- 4.3.0版本: 解决了4.2.0版本中stat类型与Windows中变量冲突的问题

C解决可移植性问题, 在头文件中声明, 根据CGNS版本将数据类型的长度设置为指定位数

```c
#if CGNS_VERSION < 3100
#define cgsize_t int
#endif
```

## 数组索引

入门指南主要用Fortran语言进行演示 Fortran-API，C-API需要在完整定义里面查询, 所有后面带有_f的函数都是Fortran语言, 剩余函数就是C语言接口 。

在CGNS提供的官方库中, 所有的数组下标都是从1开始计算, 这是Fortran的习惯, 和C程序习惯有所不同.

- C 的多维数组*按行 (row major)* 存储，Fortran 的多维数组*按列 (column major)* 存储，因此 *row in C* 对应于 *column in Fortran*。
- Fortran 的数组索引从 `1` 开始（与 SIDS 一致），而 C 则从 `0` 开始（故调用 C-API 时可能需要转换, 循环都需要从1开始）。

## 演示代码

克隆 [CGNS 代码库](https://github.com/CGNS/CGNS)后，可在 `${CGNS_SOURCE_DIR}/src/Test_UserGuideCode/` 中找到所有示例的源文件。

- 源文件头部的注释给出了在Linux下独立构建各示例的方法.
- 若要批量构建所有示例，可在 CMake 中开启 `CGNS_ENABLE_TESTS` 选项，这样生成的可执行文件位于 `${CGNS_BUILD_DIR}/src/Test_UserGuideCode/` 中。
- 源文件中上面的文件夹存储了示例代码应该有的输出, 用于test检验.



# 网格的结构组成

## 结构网格

网格由其顶点 *(vertices)* 定义, 主要有4中基本元素: *体 ( volume )* , *单元 ( cell )* , *面 ( face )* , 边 *( edge)* . 一个体是由单元组成, 其中每个单元是由八个最近的相邻顶点所定义的六面体区域组成, 这八个相邻顶点组成六个面, 每个面都是4条边组成, 每条边都是由两个顶点组成, 最终形成一个完成的体.

结构多区域网格 *(multizone grid)* 由多个区域 *(zone)* 组成, 这些区域包括构成该区域中网格的所有顶点, 单元, 面和边. 3D情况下cell就是由面组成的多面体, 2D情况下cell是面或者边, 在1D情况下, 链接两个顶点的边就是一个cell. 用来表述维度坐标的索引是有序的 ( *i* , *j* , *k* )；( *i* , *j* ) 用于二维，( *i* ) 用于一维.

这些用于2D单元中心或3D面中心的索引**由1开始计数**, 一个面的索引已知, 便可以通过计算其最近的相邻顶点索引来获取构成这个面的顶点, 即在结构化网格中, **已知点可以求得面, 已知面也可以获得点**. 

![带有索引的二维网格部分](http://cgns.github.io/CGNS_docs_current/sids/conv.figs/gridnot_fig1.gif)

## 非结构网格

结构网格与非结构网格最大的区别, 就是在于网格顶点的定义上: 
>  - 结构网格的点之间的邻接是有序、规则的，单元是二维的四边形、三维的六面体, 不用存储元素间坐标关系
> - 非结构网格的点之间的邻接是无序的、不规则的，每个网格点可以有不同的邻接网格数，单元有二维的三角形、四边形，三维的四面体、六面体、三棱柱和金字塔等多种形状, 需要存储单元间的关系. 单元间的关系通常包括单元类型或者形状, 以及每个单元的顶点列表

在非结构网格中, 顶点从1到 *N* 进行编号组成索引，其中*N*是区域中的节点数。元素由一个或多个节点组成，其中每个顶点由其索引表示。区域 ( Zone ) 由单元cell组成, 每个区域也由其索引进行表示。即非结构网格不像结构网格可以由坐标得出其几何关系，各个顶点间的关系需要专门的表示方式



# 基本模块

## `root` 文件信息

CGNS文件最开始的root节点, 用于存储文件的CGNS版本, 文件类型, 精度等信息, 由库文件自行处理, 一般不需要读出。

### **层级结构**

```c++
root
├── CGNSLibraryVersion_t
│   ├── Name: CGNSLibraryVersion
│   └── Data: 版本号
└── CGNSBase_t  // 可有多个，各表示一个算例
```

### **对应库函数**

用于新建对象的函数 `open()` 或 `write()` 总是以 `(int) id` 作为返回值。此 `id` 可以被后续代码用来访问该对象, 相当于操作系统中的文件描述符。

- 函数`cg_close`必须始终是最后调用的函数。它关闭由索引号`fn`指定的 CGNS 文件并释放保存 CGNS 数据的内存。如果省略，则 CGNS 文件不会被正确写入。
- 为了减少内存占用和提高执行速度，网格坐标或流解等大型数组实际上并不存储在内存中，只保留有关节点的基本信息，而数据的读写直接在应用程序的内存中进行。

```c++
/* open/close a CGNS file */
ier = cg_open(char *file_name,
    int mode/* CG_MODE_WRITE | CG_MODE_READ | CG_MODE_MODIFY */,
    /* output: */int *i_file);
ier = cg_close(int i_file);

/* parallel version (need MPI) */
ier = cgp_open(char *file_name, int mode,
    /* output: */int *i_file);
ier = cgp_close(int i_file);

/* CGNS file info */
ier = cg_version(int fn, /* output: */float *version);
ier = cg_precision(int fn, /* output: */int *precision);
ier = cg_is_cgns(const char *filename, /* output: */int *file_type);
ier = cg_set_file_type(int file_type);
ier = cg_get_file_type(int fn, /* output: */int *file_type);
```

| 变量名       | 功能                                                         |
| ------------ | ------------------------------------------------------------ |
| file_name    | CGNS 文件的名称，必要时包括路径名。这个字符变量的长度没有限制。 |
| mode         | 用于打开文件的模式。当前支持的模式是`CG_MODE_READ`（只读）、 `CG_MODE_WRITE`（只写）和`CG_MODE_MODIFY`（读写）。 |
| i_file       | CGNS 文件索引号。                                            |
| version      | CGNS 版本号。                                                |
| precision    | 指明CGNS 文件的精度。返回值将是 32（32 位）、64（64 位）之一，如果未知则为 0。 |
| file_type    | CGNS 文件的类型。这通常是`CG_FILE_ADF` 或`CG_FILE_HDF5`，具体取决于基础文件格式。 |
| follow_links | 此标志确定在保存 CGNS 文件时链接是否保持不变。如果非零，则链接将被删除，与链接文件相关的数据将复制到新文件。 |



## `CGNSBase_t` 根节点

CGNS的根节点标识为CGNSBase_t，名称由用户定义。一个文件中可含有多个CGNSBase_t，这些Base可以用于并行计算时候交给不同的节点进行计算，不同的Base间属于程序间的并行。

`CGNSBase_t`包含计算网格的单元维数(CellDimension) 和物理维数(PhysicalDimension), 还附带有全局使用的信息, 包含参考状态, 一组流动方程, 尺寸单位等。除此之外还有一些由用户自定义的注释信息等。其中CellDimension和PhysicalDimension是必填字段, 读取时需要检测。

### **层级结构**

```c++
CGNSBase_t
├── Name: 文件名
├── Data: cell_dim  // int
│         phys_dim  // int   
├── nbases: base的个数
└── Zone_t  // 可有多个，各表示一块网格
```

### **参数解析**

1. 三个维度: IndexDimension, CellDimension, PhysicalDimension, 这三个信息会传递给zone_t用于解析
   - `IndexDimension`: 表示引用一个节点所需要的不同索引的数量, 在结构网格 中, indexDimension始终等于cellDimension; 在非结构网格中, indexDimension始终等于1. 所以这个值在每个zone中都是自动生成的.
   - `cellDimension`: 网格中单元的维数(3 for a volume cell, 2 for a face cell)
   - `PhysicalDimension`: 定义节点位置所需要的坐标数. (1 for 1-D, 2 for 2-D, 3 for 3-D)
2. `DataClass`: 描述了 CGNS 数据库中包含的数据类的全局默认值.

### **对应库函数**

```c
/* write/read CGNSBase_t */
ier = cg_base_write(int i_file, char *base_name, int cell_dim, int phys_dim,
    /* output: */int *i_base);
ier = cg_base_read(int i_file, int i_base,
    /* output: */char *base_name, int *cell_dim, int *phys_dim);
ier = cg_nbases(int fn, /* output: */int *nbases);
```



## `Zone_t` 单块网格信息

Zone_t包含在CGNSBase_t中。Zone_t包含单个区域相关的所有信息, 包括区域类型、构成该区域网格的单元和顶点的数量、网格顶点的物理坐标、网格运动信息、族、流动解、区域界面连通性、边界条件和区域收敛历史数据等. 其中ZoneType、VertexSize和 CellSize 是必要的字段, 读取时需要检查. 

Zone属于分区后的单块网格, 可以交给不同的进程进行计算, 属于进程间的并行.

### **层级结构**

```c++
Zone_t
├── ZoneType_t
│   ├── Name: ZoneType
│   └── Data: Structured | Unstructured | UserDefined | Null
├── zone_size[][]
│   ├── VertexSize[IndexDimension] 
│   ├── CellSize[IndexDimension] 
│   └── VertexSizeBoundary[IndexDimension] 	
├── GridCoordinates_t       // 顶点坐标
├── Elements_t              // 单元顶点列表（非结构网格特有）
├── FlowSolution_t          // 物理量在顶点或单元上的值
├── ZoneBC_t                // 边界条件
└── ZoneGridConnectivity_t  // 多区网格的衔接方式
```

### **参数解析**

1.  Zone_size\[3][IndexDimension], 即结构网格中列数等于维度数, 非结构网格中列数等于1
    1.  `VertexSize`是每个索引方向上的顶点数. 在非结构网格中顶点数与维度无关, 所以只有一列, 即`VertexSize` 是一个数; 在结构化网格中每个方向上的顶点都需要存储, 所以列数与维度有关, 即`VertexSize` 是一个长度等于维度数的数组
    2.  `CellSize`是每个方向上的单元数, 即domain内部的单元数。这两个网格大小的数组会被传递到`grid-coordinate` `flow-solution`  `discrete-data`。(在3D结构化网格, CellSize = VertexSize - [1,1,1] ; 在3D非结构化网格, CellSize就是 3D 单元的总数)
    3.  `VertexSizeBoundary` 是边界节点的数量, 只有当节点在内部节点和边界节点之间排序时才有效。默认情况下该参数为0, 意味着未排序。此参数仅对非结构化网格有用, 结构网格中始终为0。
2.  `GridCoordinates_t` 用来定义网格,包含了网格顶点的物理坐标, 并且可以包含rind 或ghost points 的物理坐标. 在涉及网格节点排序时, GridCoordinates_t 中的DataArray_t先列出边界节点的数据, 然后再是内部节点的数据. 
3.  `Element_t` 包含非结构网格元素数据, 例如父元素、元素类型等.
4.  `FlowSolution` 包含流动解的数据. 
5.  `ZoneBC_t` 与区域相关的所有边界条件信息.

### **对应库函数**

```c
/* write/read Zone_t */
ier = cg_zone_write(int i_file, int i_base, char *zone_name, cgsize_t *zone_size,
    ZoneType_t zone_type/* CGNS_ENUMV(Structured) | CGNS_ENUMV(Unstructured) */,
    /* output: */int *i_zone);
ier = cg_zone_read(int i_file, int i_base, int i_zone,
    /* output: */char *zone_name, cgsize_t *zone_size);

```

| 变量名    | 功能                                                         |
| --------- | ------------------------------------------------------------ |
| i_file    | CGNS 文件索引号。                                            |
| i_base    | 基本索引号，其中 1 ≤ `B` ≤ `nbases`。                        |
| i_zone    | 区域索引号，其中 1 ≤ `Z` ≤ `nzones`。                        |
| nzones    | base`B`中存在的区域数。                                      |
| zone_name | 区域的名称。                                                 |
| zone_size | 二维数组， 其行数为 `3` (即zone_size\[3][IndexDimension])，各行分别表示*顶点数*、*单元数*、*边界单元数* 。对于**结构网格 **：*列数* 至少等于*空间维数*，每列分别对应一个（逻辑）方向。各方向的*单元数* 总是比*顶点数* 少 `1`。*边界单元* 没有意义，因此最后一行全部为 `0`。对于**非结构网格**：*列数* 至少为 `1`。若没有对单元排序（边界单元在前、内部单元在后），则*边界单元数*为 `0` |



## 定位操作

根据索引或节点名找到对应数据的位置, 实质上是在CGNS库中移动当前的文件指针, 使得下次访问数据的时候文件存取从goto的位置开始.

### **对应库函数**

```c
/* Access a node via label-index or name-0 pairs. */
/* return Value: 
 * 0 = CG_OK, 1 = CG_ERROR, 2 = CG_NODE_NOT_FOUND, 3 = CG_INCORRECT_PATH	 */

ier = cg_goto(int i_file, int i_base, ..., "end");
ier = cg_gorel( int fn , ... , "end" );
ier = cg_gopath(int i_file, const char *path);
/* Delete a child of current node. */
ier = cg_delete_node(char *node_name);

/* e.g. */
cg_goto(i_file, i_base, "Zone_t", 1, "UserDefinedData_t", 1, NULL);//table
cg_goto(i_file, i_base "Zone1", 0, "UserDefinedData_t", 1, NULL);//name
cg_goto(i_file, i_base, "Zone_t", 1, "User", 0, NULL);
cg_goto(i_file, i_base, "Zone1", 0, "User", 0, NULL);
cg_gopath(i_file, "/Base/Zone1/User");
```

| 变量名 | 功能                                                         |
| ------ | ------------------------------------------------------------ |
| fn     | CGNS 文件索引号。                                            |
| i_base | 基本索引号，其中 1 ≤ `B` ≤ `nbases`。                        |
| ...    | 用于指定节点路径的变量参数列表, 格式为"CGNS_NodeLable/Name", NodeIndex。节点可以通过它们的标签lable或名称name来标识。用名字标识时NodeIndex为0。 |
| "end"  | 用于表示参数列表结束, 在c中可以用NULL表示                    |



## 错误处理

当发生错误时, errno会置为非0, 调用cg_get_error获取错误信息。

### **对应库函数**

```c
error_message = const char *cg_get_error();
void cg_error_exit();
void cg_error_print();
```



## 并行IO杂项

使用并行IO前需要先将MPI初始化, 底层调用的是HDF5的并行IO函数, 所以也需要HDF5支持并行模式

 调用CGNS的并行函数后, 其自动使所有启动的Node并行的读取文件, 读取的数据存在各个节点中, 调用并行获取数据函数时, 由MPI通信自动汇总, 不需要用户再去编写MPI通信流程

### **对应库函数**

```c
/*Set the MPI communicator*/
ier = cgp_mpi_comm(int mpicomm);
/*Set the MPI info object*/
ier = cgp_mpi_info(MPI_Info info);
/*Set the parallel IO mode*/
ier = cgp_pio_mode(PIOmode_t mode);
void cgp_error_exit();
```

| 变量名  | 功能                                                         |
| ------- | ------------------------------------------------------------ |
| mpicomm | MPI的communicator, 可以通过这个指定使用不同的mpi通讯组       |
| info    | MPI的信息, 可传递文件分片和内部缓冲区大小等, 用于优化mpi运行流程 |
| mode    | 设置并行数据读取和写入的模式, 默认为CGP_COLLECTIVE, 允许任意数量的进程访问数据, 且所有进程都必须访问数据 |



# 单区网格坐标

##  `GridCoordinates_t` (网格坐标)

用于描述与特定Zone相关联的网格坐标, 需要读取Zone中ZoneType的Zone_size[0]中的顶点数信息来申请xyz坐标的大小. xyz坐标的有无与维数有关

#### **层级结构**

其中`DataArray_t` 是存储数据的主要结构, 内部描述了数据的类型, 维数和每个维度大小的多维数组, 数据可以是有量纲或无量纲量(压力等)

```c++
GridCoordinates_t
├── Name: GridCoordinates 
└── DataArray_t  // 个数 == 所属 CGNSBase_t 对 象的 phys_dim
    ├── Name: CoordinateX | CoordinateY | CoordinateZ |
    │         CoordinateR | CoordinateTheta | CoordinatePhi
    └── Data: 一维数组，长度 = 顶点数 + 外表层数(ring)  // 沿当前方向
```
#### **对应库函数**

```c
/* write/read (GridCoordinates_t) "GridCoordinates" */
ier = cg_grid_write(int i_file, int i_base, int i_base,
    char *grid_name/* GridCoordinates */,
    /* output: */int *i_grid);
ier = cg_grid_read(int i_file, int i_base, int i_zone, int i_grid,
    /* output: */char *grid_name);
ier = cg_ncoords( int i_file , int i_base , int i_base , 
    /* output: */int *ncoords );
```

## CoordinateX / Y / Z

实际存储的坐标信息, Z坐标只有在3D情况下才会存在

#### **对应库函数**

```c
/* write/read (DataArray_t) "Coordinate[XYZ]" */
ier = cg_coord_write(int i_file, int i_base, int i_zone,
    DataType_t data_type/* CGNS_ENUMV(RealDouble) */,
    char *coord_name, void *coord_array,
    /* output: */int *i_coord);
ier = cg_coord_read(int i_file, int i_base, int i_zone,
    char *coord_name, DataType_t data_type,
    cgsize_t *range_min, cgsize_t *range_max,  // 1-based inclusive
    /* output: */void *coord_array);

/* parallel version (need MPI) */
ier = cgp_coord_write(int i_file, int i_base, int i_zone,
    DataType_t data_type/* CGNS_ENUMV(RealDouble) */, char *coord_name,
    /* output: */int *i_coord);
ier = cgp_coord_read();  /* undefined */
ier = cgp_coord_write_data(int i_file, int i_base, int i_zone, int i_coord,
    cgsize_t *range_min, cgsize_t *range_max,  // 1-based inclusive
    void *coord_array);
ier = cgp_coord_read_data(int i_file, int i_base, int i_zone, int i_coord,
    cgsize_t *range_min, cgsize_t *range_max,  // 1-based inclusive
    /* output: */void *coord_array);
```

- `range_min` 和 `range_max` 分别表示坐标的边界, 在c语言中`range_min` 全部初始化为1, `range_max` 根据`zone_size[0]` 顶点数算出来
  - 在非结构化网格中, `range_max` = `zone_size[0][0]` 
  -  在结构化网格中, `range_max` = `zone_size[0][0]` +  `zone_size[0][1]` + ... 
- `coord_array` 是一个数组, 长度由(即顶点数) 得出, 大小等同于`range_max` 
- 坐标名 `coord_name` 必须取自《[SIDS-standard names](https://cgns.github.io/CGNS_docs_current/sids/dataname.html)》，即
  - 直角坐标：`CoordinateX | CoordinateY | CoordinateZ`
  - 球坐标：`CoordinateR | CoordinateTheta | CoordinatePhi`
- `data_type` 应当与 `coord_array` 的类型匹配, 只能是 `RealSingle` 或者 `RealDouble` 
  - `CGNS_ENUMV(RealSingle)` 对应 `float`。
  - `CGNS_ENUMV(RealDouble)` 对应 `double`。
- 并行版本的写分两步：
  - 先用 `cgp_write_coord()` 创建空 `DataArray_t` 对象；
  - 再用 `cgp_write_coord_data()` 写入当前进程所负责的部分。



# 单区网格拓扑信息

## 单元cell

在CGNS中，**单元**是由网格单元的类型、节点坐标和节点之间的连接关系组成。解算器可以使用这些信息来构建数值模型，求解流场等问题。

CGNS可以存储各种类型的网格，如结构化网格、非结构化网格、混合网格等。对于每种类型的网格，CGNS都有相应的单元类型定义。例如，对于结构化网格，CGNS定义了矩形单元、立方体单元等，而对于非结构化网格，CGNS定义了三角形单元、四面体单元等。

在CGNS中，单元信息可以通过两种方式构建。一种是直接指定每个单元的节点编号，即节点之间的连接关系；另一种是指定每个单元的节点坐标，由CGNS根据节点坐标自动推导出节点之间的连接关系。对于非结构化网格，通常采用前一种方式，而对于结构化网格，通常采用后一种方式。



## `Elements_t` (非结构网格单元信息)

结构网格的*顶点信息* 已经隐含了*单元信息*，因此不需要显式创建单元。与之相反，非结构网格的单元信息需要显式给出, 这里 Element 的含义就是组成一个单元cell的基本类型, 可以是点, 边或面，轴对称和旋转坐标的对应内容没有写出, 对应网站的[轴对称和旋转坐标](http://cgns.github.io/CGNS_docs_current/midlevel/grid.html#axisymmetry)。

在非结构化区域中，节点是从1到n排序的，其中n是区域中的节点的数量。 一个元素由一个或多个点组成，其中每个点由其索引表示。 元素在一个zone内标记为1到m作为其索引，其中m是该区域定义的元素总数。所以 `ElementRange` 中存储的就是排序后每个元素的索引，也就是element的个数。`ElementConnectivity` 中存储的是对应的点索引，可以找到对应的顶点。

CGNS支持八个元素形状：点，线，三角形，四边形，四面体，五面体，金字塔和六面体。 描述体的元素称为3-D元素。 描述面的是二维元素。 线和点元素分别称为1-D和0D元素。在3-D非结构化网格中，单元由3-D元素表示，而边界的贴面由2-D元素来描述。 完整的元素定义不仅仅包括单元。每个元素形状可能具有不同数量的节点，具体取决于是否使用了线性，二次或立方插值。 因此，每种元素的名称由两个部分组成：第一部分描述元素形状，第二部分存储节点的数量。

### **层级结构**

```c++
Elements_t
├── IndexRange_t	// ElementConnectivity中定义的第一个到最后一个元素的索引
│   ├── Name: ElementRange
│   └── Data: int[2] = {first, last}  // 单个Elements_t内的单元需连续编号
├── ElementType_t: // 元素类型
│   └── Data: NODE = 2 | BAR_2 = 3 | TRI_3 = 5 | QUAD_4 = 7 |
│             TETRA_4 = 10 | PYRA_5 = 12 | PENTA_6 = 14 | HEXA_8 = 17 |
│             NGON_n = 22 | NFACE_n = 23 | MIXED = 20
└── DataArray_t  // 存储数据
│   ├── Name: ElementConnectivity  // 链接信息
│   └── Data: int[ElementDataSize]  
│    	// ElementDataSize :=
│       // 同种单元 ElementSize * NPE(ElementType)
│       // 多种单元 Sum(NPE(ElementType[n]) + 1)
│       // NPE := 特定ElementType所包含的node数, 例如NPE[HEXA_8]=8
└── DataArray_t  
    ├── Name: ElementStartOffset 	// 含多种单元时使用
    └── Data: int[ElementSize + 1]
```

### 对应库函数

```c
// read section number
ier = cg_nsections(int i_file, int i_base, int i_zone,
    /* output: */int *n_sections);
// read Element node Size
ier = cg_ElementDataSize(int fn, int B, int Z, int S,
    /* output: */cgsize_t *ElementDataSize);
// read the size of node for an element of type
ier = cg_npe(ElementType_t type,  /* output: */int *npe);
    
// for mixed cell and face elements info:
ier = cg_section_write(int i_file, int i_base, int i_zone,
    char *section_name, ElementType_t element_type,
    cgsize_t first, cgsize_t last, int n_boundary,
    cgsize_t *connectivity, /* output: */int *i_sect);
ier = cg_section_read(int i_file, int i_base, int i_zone, int i_sect,
    /* output: */char *section_name, ElementType_t *element_type,
    cgsize_t *first, cgsize_t *last, int *n_boundary, int *parent_flag);

// read data（所有图形，侧重于单元信息）
ier = cg_elements_read(int i_file, int i_base, int i_zone, int i_sect,
    /* output: */cgsize_t *connectivity, cgsize_t *parent_data);
ier = cg_elements_partial_write(int i_file, int i_base, int i_zone, int i_sect,
    cgsize_t first, cgsize_t last, cgsize_t *connectivity);
ier = cg_elements_partial_read(int i_file, int i_base, int i_zone, int i_sect,
    cgsize_t first, cgsize_t last,
    /* output: */cgsize_t *connectivity, cgsize_t *parent_data);

// for MIXED | NGON_n | NFACE_n type(多边形或多面体，侧重于面 ):
ier = cg_poly_section_write(int i_file, int i_base, int i_zone,
    char *section_name, ElementType_t element_type,
    cgsize_t first, cgsize_t last, int n_boundary,
    cgsize_t *connectivity, cgsize_t *offset,
    /* output: */int *i_sect);
ier = cg_poly_elements_read(int i_file, int i_base, int i_zone, int i_sect,
    /* output: */cgsize_t *connectivity, cgsize_t *offset, cgsize_t *parent_data);
ier = cg_poly_elements_partial_write(int i_file, int i_base, int i_zone, int i_sect,
    cgsize_t first, cgsize_t last, cgsize_t *connectivity, cgsize_t *offset);
ier = cg_poly_elements_partial_read(int i_file, int i_base, int i_zone, int i_sect,
    cgsize_t first, cgsize_t last,
    /* output: */cgsize_t *connectivity, cgsize_t *offset, cgsize_t *parent_data);

/* parent数据读取*/

/* parallel version (need MPI) */
ier = cgp_section_write(int i_file, int i_base, int i_zone,
    char *section_name, ElementType_t element_type,
    cgsize_t first, cgsize_t last, int n_boundary,
    /* output: */int *i_sect);
ier = cgp_elements_write_data(int i_file, int i_base, int i_zone, int i_sect,
    cgsize_t first, cgsize_t last, cgsize_t *connectivity);
ier = cgp_elements_read_data(int i_file, int i_base, int i_zone, int i_sect,
    cgsize_t first, cgsize_t last,
    /* output: */cgsize_t *connectivity);
```

| 变量名          | 功能                                                         |
| --------------- | ------------------------------------------------------------ |
| n_sections      | 对应区域中的区域Element个数                                  |
| ElementDataSize | 元素的连接数                                                 |
| npe             | 某种特定类型的单元的数量                                     |
| type            | 单元的类型, 即下面的ElemtType                                |
| first           | 区域中的第一个单元的索引, 即Element No.                      |
| last            | 最后一个单元的索引                                           |
| n_boundary      | 区域中最后一个边界单元的索引 (如果单元未排序则置为0)         |
| parent_flag     | 是否存在父节点                                               |
| connectivity    | 单元内节点的关系数据, 针对所有的形状单元(即除了Mix混合类以外) |
| parent_data     | 对于边界单元, 数组内包含单元或单元面的信息(可以设置为NULL)   |
| offset          | 元素链接的偏移数据, `NGON_n`、`NFACE_n`和`MIXED`类需要       |

- `cg_section_write()` 在给定的 `Zone_t` 对象下新建一个**单元片段 (element section)**，即 `Elements_t` 对象。
- 一个 `Zone_t` 对象下可以有多个 `Elements_t` 对象：
  - 同一个 `Elements_t` 对象下的所有单元必须具有同一种 `element_type`，并且必须是枚举类型 [`ElementType_t`](file:///Users/master/code/mesh/cgns/doc/midlevel/general.html#typedefs) 的有效值之一。
  - 同一个  `Zone_t` 下的所有单元（含所有维数）都必须有*连续*且*互异*的编号。
- `first`、`last`
  - 在串行版本及 `cgp_section_write()` 中为当前 `Elements_t` 对象内的首、末单元的编号。
  - 在 `cgp_elements_write_data()` 及 `cgp_elements_write_data()` 中为当前进程所读写的首、末单元的编号。
- `n_boundary` 为当前 `Elements_t` 对象的*边界单元数*：若 `n_boundary > 0`，则单元已被排序，且前  `n_boundary` 个单元为边界单元。
- `parent_flag` 用于判断 parent data 是否存在。



## Element Type (单元类型)

Element数据可以分为两大类: **普通形状单元** 和 **混合单元**(MIXED). 普通形状单元就是固定的形状, 例如2D中有四边形和三角形; 对于混合单元, 使用 `NGON_n` 和 `NFACE_n` 可以定义任意多面体单元.

### 普通形状单元

CGNS 支持八种单元形状: 点、[线](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_line)、 [三角形](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_tri)、[四边形](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_quad)、 [四面体](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_tetra)、[五面体](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_penta)、 [六棱柱](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_pyramid)和[六面体](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_hexa)。描述体积的单元称为[3D Elements](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_3d)。描述面的是[2D Elements](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_2d)。线和点元素分别称为[1D](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_line)和0D Elements。线性插值指两个点组成一条线, 二次插值即两个点组成的线中间还会多一个点, 三次及四次插值同上. 一般来说只会用到线性插值和二次插值, 下面是单元类型及对应的表: 

| 类型 | 形状                                                         | 线性插值  | 二次插值             | 三次插值                       | 四次插值                         |
| ---- | ------------------------------------------------------------ | --------- | -------------------- | ------------------------------ | -------------------------------- |
| 0D   | Point（点）                                                  | `NODE`    | `NODE`               | `NODE`                         | `NODE`                           |
| 1D   | [Line](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_line)（线） | `BAR_2`   | `BAR_3`              | `BAR_4`                        | `BAR_5`                          |
| 2D   | [Triangle](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_tri)（三角形） | `TRI_3`   | `TRI_6`              | `TRI_9, TRI_10`                | `TRI_12, TRI_15`                 |
|      | [Quadrangle](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_quad)（四边形） | `QUAD_4`  | `QUAD_8, QUAD_9`     | `QUAD_12, QUAD_16`             | `QUAD_P4_16, QUAD_25`            |
| 3D   | [Tetrahedron](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_tetra)（四面体） | `TETRA_4` | `TETRA_10`           | `TETRA_16, TETRA_20`           | `TETRA_22, TETRA_34, TETRA_35`   |
|      | [Pyramid](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_pyramid)（金字塔形） | `PYRA_5`  | `PYRA_13, PYRA_14`   | `PYRA_21, PYRA_29, PYRA_30`    | `PYRA_P4_29, PYRA_50, PYRA_55`   |
|      | [Pentahedron](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_penta)（五面体） | `PENTA_6` | `PENTA_15, PENTA_18` | `PENTA_24, PENTA_38, PENTA_40` | `PENTA_33, PENTA_66, PENTA_75`、 |
|      | [Hexahedron](http://cgns.github.io/CGNS_docs_current/sids/conv.html#unst_hexa)（六面体） | `HEXA_8`  | `HEXA_20, HEXA_27`   | `HEXA_32, HEXA_56, HEXA_64`    | `HEXA_44, HEXA_98, HEXA_125`     |

### 混合单元

对于混合单元, 使用 `NGON_n` 和 `NFACE_n` 可以定义任意多面体单元, `NGON_n` 用于指定网格中的所有面, 然后用 `NFACE_n` 将多面体单元定义为这些面的集合. 除边界面外，多面体单元的每个面都必须由另一个多面体单元共享. `ElementConnectivity` 用于描述网格中每个面的节点列表, 而 `ElementStartOffset` 提供`ElementConnectivity` 数组中每个面的起始位置.



## 非结构化网格示例

以一个立方体形状的非结构网格为例, 该zone的每个边缘有3个节点, 生成非结构化网格共27个节点, 分解图如下:

![3 x 3 x 3 非结构化网格，带有编号节点](http://cgns.github.io/CGNS_docs_current/sids/conv.figs/unst_example.gif)

这个区域包含6个六面体单元, 分别编号1-8, 点到单元的连接(connectivity)为:

![pp1VAmt.png](https://s1.ax1x.com/2023/03/14/pp1VAmt.png)

也可以划分为点到面的连接, 这个可以直接由connectivity计算得出. 这个模型中有24个边界面, 对应单元编号9-32, 每个边界面的类型均为 `QUAD_4` , 下表显示了每个边界面的单元连通性，以及其父单元的单元编号和面编号:

![](https://s1.ax1x.com/2023/03/14/pp1Vwc9.png)





# 边界条件BocoData

边界条件BC（Boundary Condition），是指在求解区域边界上所求解的变量或其导数随时间和地点的变化规律。通常有固体壁面条件，来流、出流条件，周期性条件，对称条件等。边界条件通常依赖于控制方程，如在固体壁面上，欧拉方程要求采用不可穿透条件，而纳维-斯托克斯方程则要求满足无滑移条件。

## 结构网格

尽管本节标题为*结构网格*，但上述方法也可以用于*非结构网格*，只是后者可以更简单的用Element_t来附加边界条件信息。

**层级结构**

```c
Zone
├── ZoneBC_t
│    └── BC_t
│     	├── IndexRange_t
│    	└── IndexRange_t 
└── ZoneType // 表示边界条件的类型
```

- 每个 `BC_t` 对象都含有一个 `IndexRange_t` 或 `IndexRange_t` 子对象。
- 所有 `BC_t` 对象都是同一个 `ZoneBC_t` 对象的子对象。
- 这个唯一的 `ZoneBC_t` 对象是某个 `Zone_t` 对象的子对象，因此是 `GridCoordinates_t` 对象及 `FlowSolution_t` 对象的同辈对象。

两种 BC 表示方法：

- `PointRange` 是在一个逻辑矩形区域的面上确定最大点和最小点，适用于**结构网格的矩形边界**。

- `PointList` 适用于**所有边界**, 存储的是所有的点列表 , 除 `PointRange` 外的点都应该用其补充表示。

**对应库函数**

```c
// Write boundary condition type and data:
ier = cg_boco_write(int i_file, int i_base, int i_zone,
    char *boco_name, BCType_t boco_type/* CGNS_ENUMV(BCType_t) */,
    PointSetType_t ptset_type/* CGNS_ENUMV(PointRange) | CGNS_ENUMV(PointList) */, cgsize_t npnts, cgsize_t *point_set,
    /* output: */int *i_boco);

// Get number of boundary condition in zone:
ier = cg_nbocos(int i_file, int i_base, int i_zone,
    /* output: */int *n_boco);

// Get boundary condition info:
ier = cg_boco_info(int i_file, int i_base, int i_zone, int i_boco,
    /* output: */char *boco_name, BCType_t *boco_type,
    PointSetType_t *ptset_type, cgsize_t *npnts,
    int *NormalIndex,
    cgsize_t *NormalListSize,
    DataType_t *NormalDataType,
    int *ndataset);

// Read boundary condition data and normals:
ier = cg_boco_read(int i_file, int i_base, int i_zone, int i_boco,
    /* output: */cgsize_t *point_set, void *normal_list);
// read
ier = cg_boco_gridlocation_read( int fn , int B , int Z , int BC , 
    /* output: */GridLocation_t *location );

// for type of FamilySpecified
ier = cg_family_read(int fn, int B, int Fam, 
    /* output: */char *FamilyName, int *nFamBC, int *nGeo)
```

| 变量名         | 功能                                                         |
| -------------- | ------------------------------------------------------------ |
| bocotype       | 边界条件的类型. 特殊的,如果是FamilySpecified, 就要为边界所属的族规定边界条件的类型 |
| ptset_type     | 可利用点或单元的范围, 分别有`PointRange` （表示采用点或单元的范围）和 `PointList`（表示采用点或单元的离散列表） |
| npnts          | 定义边界条件区域的顶点或单元数。对于`PointRange`来说 `npnts`始终为两个。而对于`PointList` 为列表中顶点的数量。当要在点以外的任何地方应用边界条件时，必须使用`BC_t`节点下的`GridLocation_t`来指示这一点。`GridLocation_t`的值可以由`cg_boco_gridlocation_read`和`cg_boco_gridlocation_write`读取或写入 。 |
| NormalIndex    | 边界条件补块法向量的计算坐标方向的索引矢量                   |
| NormalListSize | 如果法线在`NormalList`中定义， 则`NormalListSize`是面片中的点数乘以`phys_dim`，即在场中定义矢量所需的坐标数。如果法线未在`NormalList`中定义，则 `NormalListSize`为 0 |
| NormalDataType | 定义法向量的数据精度类型。分为单精度和双精度                 |
| ndataset       | 本边界条件数据组的数据的个数                                 |
| point_set      | 定义边界条件范围的点数据, 即PointList或PointRange的数据，长度应该等于npnts |
| normal_list    | 指向块内部的边界条件补块的矢量列表法向量                     |
| location       | 点列表使用的网格位置                                         |

其中

- `cg_boco_write()` 用于创建一个表示具体边界条件的 `BC_t` 对象。
- `boco_type` 的取值必须是枚举类型 `BCType_t` 的有效值, 用来表示边界条件的类型，例如 `BCWallInviscid | BCInflowSupersonic | BCOutflowSubsonic`，完整列表参见《[Boundary Condition Type Structure Definition](https://cgns.github.io/CGNS_docs_current/sids/bc.html#BCType)》。
- 二维数组 `point_set` 用于指定顶点编号，其行数（至少）为 `n_point`。
  - 对于结构网格，`point_set` 的列数为*空间维数*，而 `n_point`
    - 为 `2`，若 `point_set_type` 为 `CGNS_ENUMV(PointRange)`。此时 `point_set` 的第一、二行分别表示编号的下界、上界。
    - 为 此边界的顶点总数，若 `point_set_type` 为 `CGNS_ENUMV(PointList)`。
  - 对于非结构网格，`point_set` 的列数为 `1`，而 `n_point`
    - 为 此边界的顶点总数，且 `point_set_type` 只能为 `CGNS_ENUMV(PointList)`。

## 非结构网格

尽管*非结构网格*可以像*结构网格*那样，通过指定边界上的*顶点*来施加边界条件，但利用读写单元时创建的 `Elements_t` 对象来指定边界上的*单元*通常会更加方便。换言之，非结构网格的边界条件既可以用BC来表示，也可以用Element表示。用BC表示的话只会存储点的信息，需要根据别的新信息如gridlocation等来表示其拓扑关系，构建出面单元；用Element表示则直接表示为面。

主要的 API 如下：

```c
// Write boundary condition type and data:
ier = cg_boco_write(int i_file, int i_base, int i_zone, char *boco_name,
    BCType_t boco_type/* CGNS_ENUMV(BCType_t) */,
    PointSetType_t point_set_type/* CGNS_ENUMV(PointRange) | CGNS_ENUMV(PointList) */,
    cgsize_t n_cell, cgsize_t *cell_set,
    /* output: */int *i_boco);

// Write grid location:
ier = cg_gridlocation_write(GridLocation_t grid_location/*
    CGNS_ENUMV(CellCenter) | CGNS_ENUMV(FaceCenter) |
    CGNS_ENUMV(EdgeCenter) | CGNS_ENUMV(Vertex) */);
// Read grid location:
ier = cg_gridlocation_read(GridLocation_t *grid_location);
```

其中

- 这里的 `cg_boco_write()` 在形式上与*结构网格*版本一样，只是将 `n_point`、`point_set` 替换成了 `n_cell`、`cell_set`。
- `grid_location == CGNS_ENUMV(FaceCenter)` 表示当前边界条件定义在*面单元*上，即 `cell_set` 是存储*面单元*编号的数组。
- 调用 `cg_gridlocation_write()` 之前必须先用 `cg_goto()` 定位到所需的 `BC_t` 对象。



# 流场数据

对于流场数据有三种表示方式，**格心型**、**格点型**和**格心型加外层单元**

- **格心型**：未知量位于单元中心，控制体取为网格单元本身，数值通量需要在网格单元的面上进行计算，因此格心型离散对应着基于面的数据结构
- **格点型**：未知量位于单元节点，控制体通常通过连接节点周围的单元中心和边的中点来构建，数值通量需要在由原网格的边构建的面上进行计算，因此格点型离散对应着基于边的数据结构。
- **格心型加外层单元**: 未知量位于单元中心, 但还有一层外部的ring节点也会参与计算, 这个只在结构网格中存在, 非结构网格中外层单元无效

## `FlowSolution_t`

**层级结构**

在层级结构上, `FlowSolutiont` 是在 `Zone_t` 对象下创建的一个表示一组物理量的对象, 一个 `Zone_t` 下可以有很多个 `FlowSolution_t` 物理量。一个 `FlowSolution_t` 下可以包含很多个物理量, 可以是点数据, 单元数据, 边数据和面数据, 对应的物理量也有很多种类, 例如压力, 密度, 速度, 动量等, 统一存储在 `DataArray_t` 中

```c++
FlowSolution_t
├── GridLocation_t
│   └── Data: Vertex | CellCenter | EdgeCenter | FaceCenter
├── IndexRange_t  // 与 IndexList_t 二选一，对 Vertex | CellCenter 非必需
│   ├── Name: PointRange
│   └── Data: int[2] = {first, last}
├── IndexList_t  // 与 IndexRange_t 二选一，对 Vertex | CellCenter 非必需
│   ├── Name: PointList
│   └── Data: int[] = {...}
└── DataArray_t
    ├── Name: Pressure | Density | VelocityX | MomentumX | ...
    └── Data: DataType[DataSize]  
    /* 编号与相应的 Elements_t 一致
        if (有 PointRange | PointList):
            DataSize = Size(PointRange | PointList)
        else:
            DataSize = VertexSize | CellSize
            if (有 Rind):
                DataSize += RindSize  */
```

## `Vertex` 格点型

顶点处的流场数据与网格点处在相同的位置. 读取和写入的类似单区网格坐标, 需要先用 `cg_sol_*` 读取 `FlowSolution_t` 节点的数据, 获得流场个数和名字等信息, 然后再用 `cg_field_*` 等函数读取具体的流场数据

<img src="../Picture/image-20230316100310486.png" alt="image-20230316100310486" style="zoom:50%;" />

**对应库函数**

```c
/* write/read FlowSolution_t */
ier = cg_sol_write(int i_file, int i_base, int i_zone, char *sol_name,
    GridLocation_t location/* CGNS_ENUMV(Vertex) */,
    /* output: */int *i_soln);
ier = cg_sol_read();  /* undefined */
ier = cg_sol_info(int i_file, int i_base, int i_zone, int i_soln,
    /* output: */char *sol_name, GridLocation_t *location);

/* Read field info */
ier = cg_nfields(int i_file, int i_base, int i_zone, int i_soln,
    /* output: */int *n_fields);
ier = cg_field_info(int i_file, int i_base, int i_zone, int i_soln, int i_field,
    /* output: */DataType_t *data_type, char *field_name);

/* read DataArray_t */
ier = cg_field_read(int i_file, int i_base, int i_zone, int i_soln,
    char *field_name, DataType_t data_type,
    cgsize_t *range_min, cgsize_t *range_max,
    /* output: */void *sol_array);

/* parallel version (need MPI) */
ier = cgp_field_write(int i_file, int i_base, int i_zone, int i_soln,
    DataType_t datatype, char *field_name,
    /* output: */int *i_field);
ier = cgp_field_write_data(int i_file, int i_base, int i_zone, int i_soln, int i_field,
    cgsize_t *range_min, cgsize_t *range_max,  // 1-based inclusive
    void *sol_array);
ier = cgp_field_read();  /* undefined, use cg_field_read() */
ier = cgp_field_read_data(int i_file, int i_base, int i_zone, int i_soln, int i_field,
    cgsize_t *range_min, cgsize_t *range_max,  // 1-based inclusive
    /* output: */void *sol_array);
ier = cgp_field_general_read_data(int i_file, int i_base, int i_zone, int i_soln, int i_field,
    cgsize_t *range_min, cgsize_t *range_max,  // range in file
    DataType_t mem_datatype/* data type of the array in memory */,
    int mem_rank/* number of dimensions of array in memory */,
    cgsize_t *mem_dimensions/* dimensions of array in memory */,
    cgsize_t *mem_range_min, cgsize_t *mem_range_max,  // range in memory
    /* output: */void *sol_array);
```

其中

- `cg_sol_info()` 读取 `FlowSolution_t` 的信息, `cg_field_info()` 读取其中一组物理量的信息
- `cg_field_read()` 读取一组物理量中的数据, 存储在 `sol_array` 中, `sol_array` 尺寸是根据前文 (`range_max` - `range_min` )计算得来：对于结构网格，通常声明为多维数组；对于非结构网格，通常声明为一位数组。

- `cg_sol_write()` 用于在 `Zone_t` 对象下创建一个表示*一组物理量*的 `FlowSolution_t` 对象。
  - 同一个  `Zone_t` 对象下可以有多个 `FlowSolution_t` 对象。
  - 所有 `FlowSolution_t` 对象都平行于 `GridCoordinates_t` 对象。
- `cg_field_write()` 用于在 `FlowSolution_t` 对象下创建一个表示*单个物理量*的对象，例如  `DataArray_t` 对象、`Rind_t` 对象。
  - `field_name` 应当取自《[SIDS-standard names](https://cgns.github.io/CGNS_docs_current/sids/dataname.html)》，例如 `Density | Pressure`。

## `CellCenter` 格心形

格心形流场数位于单元中心, 2D情况下由4个环绕的点确定网格的单元中心; 3D情况下通过8个环绕的网格点确. `CellCenter`所用 API 与`Vertex` 的几乎完全相同，只需注意：

- 在调用 `cg_sol_write()` 时，将 `location` 的值由 `CGNS_ENUMV(Vertex)` 改为 `CGNS_ENUMV(CellCenter)`。

- 在结构网格的各逻辑方向上，用于存放数据的多维数组的长度必须与单元数量协调。

  <img src="../Picture/image-20230316100404536.png" alt="image-20230316100404536" style="zoom:50%;" />

## `rind data` 格心型加外层单元

**表皮数据 (rind data)** 是指存储在网格表面的一层或多层**影子单元 (ghost cells)** 上的数据, 在CGNS中, 外层单元处的流场数据并不是单独的实体, 而是将流场的范围延申到包括外层单元的位置, 这时候访问对应的外层单元, 需要用goto函数定位到对应位置再去读取

```
┌───╔═══╦═══╦═══╗───┬───┐      ═══ 网格单元
│ o ║ o ║ o ║ o ║ o │ o │
└───╚═══╩═══╩═══╝───┴───┘      ─── 影子单元
```

新增的 API 如下：

```c
/* API in `write_flowcentrind_str.c` and `read_flowcentrind_str.c` */

// Access a node via [label|name]-index pairs:
ier = cg_goto(int i_file, int i_base, ..., "end");
// e.g.
ier = cg_goto(i_file, i_base,
    "Zone_t", i_zone,
    "FlowSolution_t", i_soln,
    "end");

// Number of rind layers for each direction (structured grid):
int rind_data[6] = {
  1/* i_low */, 1/* i_high */,
  1/* j_low */, 1/* j_high */,
  0/* k_low */, 0/* k_high */};
// Write number of rind layers:
ier = cg_rind_write(int *rind_data);
// Read number of rind layers:
ier = cg_rind_read(int *rind_data);
```

其中

- `cg_goto()` 用于定位将要创建 `Rind_t` 对象的那个 `FlowSolution_t` 对象。
- 外表数据存储在（根据影子单元层数）扩充的流场数组中，因此在结构网格的各逻辑方向上，用于存放数据的多维数组的长度必须与*扩充后的*单元数量协调。





# 量纲信息

## `DataClass_t`

```c
/* Write data class: */
ier = cg_goto(i_file, i_base, "end");
ier = cg_dataclass_write(CGNS_ENUMV(Dimensional));
ier = cg_dataclass_write(CGNS_ENUMV(NondimensionalParameter));
```

## `DimensionalUnits_t`

```c
/* Write first five dimensional units: */
ier = cg_units_write(
    CGNS_ENUMV(Kilogram), CGNS_ENUMV(Meter),
    CGNS_ENUMV(Second), CGNS_ENUMV(Kelvin),
    CGNS_ENUMV(Degree));
```

## `DimensionalExponents_t` 

```c
/* Write first five dimensional exponents of coordinates: */
float dimensional_exponents[5] = {0., 1., 0., 0., 0.};
ier = cg_goto(i_file, i_base, "Zone_t", i_zone, "GridCoordinates_t", 1,
    "DataArray_t", i_coord, "end");
ier = cg_exponents_write(CGNS_ENUMV(RealSingle), dimensional_exponents);
/* Write first five dimensional exponents of pressure: */
dimensional_exponents[0] = +1.0;
dimensional_exponents[1] = -1.0;
dimensional_exponents[2] = -2.0;
ier = cg_goto(i_file, i_base, "Zone_t", i_zone, "FlowSolution_t", i_soln,
    "DataArray_t", i_field, "end");
ier = cg_exponents_write(CGNS_ENUMV(RealSingle), dimensional_exponents);
```





# 多区网格





# 动态数据

## [迭代数据结构](https://cgns.github.io/CGNS_docs_current/sids/timedep.html#IterativeData)

SIDS 定义了两种迭代数据结构，以管理多个时间（或迭代）步的数据：

`BaseIterativeData_t` 对象位于 `CGNSBase_t` 对象之下，一般用于存储*时间步总数*及*各步的时间值*，有时（如[网格拓扑发生改变](#网格拓扑发生改变)）也用来存储*指向各步的指针*：

```c++
BaseIterativeData_t := {
  int NumberOfSteps                                                  (r)

  DataArray_t<real, 1, NumberOfSteps> TimeValues ;                   (o/r)
  DataArray_t<int,  1, NumberOfSteps> IterationValues ;              (r/o)

  DataArray_t<int,  1, NumberOfSteps> NumberOfZones ;                (o)
  DataArray_t<int,  1, NumberOfSteps> NumberOfFamilies ;             (o)
  DataArray_t<char, 3, [65, MaxNumberOfZones, NumberOfSteps]>
    ZonePointers ;                                                   (o)
  DataArray_t<char, 3, [65, MaxNumberOfFamilies, NumberOfSteps]>
    FamilyPointers ;                                                 (o)

  List( DataArray_t<> DataArray1 ... DataArrayN ) ;                  (o)

  List( Descriptor_t Descriptor1 ... DescriptorN ) ;                 (o)

  DataClass_t DataClass ;                                            (o)

  DimensionalUnits_t DimensionalUnits ;                              (o)

  List( UserDefinedData_t UserDefinedData1 ... UserDefinedDataN ) ;  (o)
}
```

`ZoneIterativeData_t` 对象位于 `Zone_t` 对象之下，一般用于存储*指向各步的指针*：

```c++
ZoneIterativeData_t< int NumberOfSteps > := {
  DataArray_t<char, 2, [32, NumberOfSteps]>
    RigidGridMotionPointers ;                                       (o)
  DataArray_t<char, 2, [32, NumberOfSteps]>
    ArbitraryGridMotionPointers ;                                   (o)
  DataArray_t<char, 2, [32, NumberOfSteps]>
    GridCoordinatesPointers ;                                       (o)
  DataArray_t<char, 2, [32, NumberOfSteps]>
    FlowSolutionPointers ;                                          (o)
  DataArray_t<char, 2, [32, NumberOfSteps]>
    ZoneGridConnectivityPointers ;                                  (o)
  DataArray_t<char, 2, [32, NumberOfSteps]>
    ZoneSubRegionPointers ;                                         (o)

  List( DataArray_t<> DataArray1 ... DataArrayN ) ;                 (o)

  List( Descriptor_t Descriptor1 ... DescriptorN ) ;                (o)

  DataClass_t DataClass ;                                           (o)

  DimensionalUnits_t DimensionalUnits ;                             (o)

  List( UserDefinedData_t UserDefinedData1 ... UserDefinedDataN ) ; (o)
}
```

⚠️ 上述*指针*目前由*字符串*实现。

## 网格固定不变
**网格固定不变**意味着 `GridCoordinates_t` 及 `Elements_t`(s) 可复用，故只需将各时间步上的 `FlowSolution_t`(s) 记录在 `ZoneIterativeData_t` 内的 `(DataArray_t) FlowSolutionPointers` 中。

官方教程中的 `write_timevert_str.c` 与 `read_timevert_str.c` 演示了这种方法。
主要 API 用法如下：

```c
/* API in `write_timevert_str.c` and `read_timevert_str.c` */

// Base-level
ier = cg_simulation_type_write(i_file, i_base, CGNS_ENUMV(TimeAccurate));
ier = cg_biter_write(i_file, i_base, "TimeIterValues", n_steps);
ier = cg_goto(ii_file, i_base, "BaseIterativeData_t", 1, "end");
int n_steps = 3;
double times[3] = {1.0, 2.0, 3.0};
ier = cg_array_write("TimeValues", CGNS_ENUMV(RealDouble), 1, &n_steps, &times);
// Zone-level
ier = cg_ziter_write(i_file, i_base, i_zone, "ZoneIterativeData");
ier = cg_goto(i_file, i_base, "Zone_t", i_zone, "ZoneIterativeData_t", 1, "end");
char names[97];  /* need an extra byte for the terminating '\0' */
strcpy(names   , "FlowSolution1");
strcpy(names+32, "FlowSolution2");
strcpy(names+64, "FlowSolution3");
cgsize_t n_dims[2] = {32, 3};
ier = cg_array_write("FlowSolutionPointers", CGNS_ENUMV(Character),
    2, n_dims, names/* the head of a 2d array, whose type is char[3][32] */);
```

⚠️ 一个 `FlowSolution_t` 有且仅有一个 `GridLocation_t`，这意味着

- 在同一个 `FlowSolution_t` 内，不能既[顶点数据](#顶点数据)、又有[单元数据](#单元数据)。
- 在同一个 `FlowSolutionPointers` 内，所有 `FlowSolution_t`(s) 必须有相同的  `GridLocation_t` 值。
- 若要同时记录[顶点数据](#顶点数据)与[单元数据](#单元数据)，则应[平行于 `FlowSolutionPointers` 再创建一个 `FlowSolutionXXXPointers`](https://gitlab.kitware.com/paraview/paraview/-/issues/19838#note_732151)，这样两类数据都能被 [ParaView](../vtk/README.md#ParaView) 识别。示例文件 [`write_fixed_grid.cpp`](./write_fixed_grid.cpp) 演示了这种方法。

## [网格作刚体运动](https://cgns.github.io/CGNS_docs_current/sids/timedep.html#RigidGridMotion)

**网格作刚体运动**意味着 `Elements_t`(s) 可复用，而顶点坐标可以由*初始位置*（记录在当前 `Zone_t` 下唯一的 `GridCoordinates_t` 中）与*刚体运动信息*（随体坐标系的原点位置、原点速度、坐标架转角、坐标架转速等）快速地算出，后者记录在 `RigidGridMotion_t` 中（一个时间步对应一个这样的  `RigidGridMotion_t`，对应关系由 `ZoneIterativeData_t` 中的 `(DataArray_t) RigidGridMotionPointers` 管理）。

```c++
RigidGridMotion_t := {
  List( Descriptor_t Descriptor1 ... DescriptorN ) ;                 (o)

  RigidGridMotionType_t RigidGridMotionType ;                        (r)

  DataArray_t<real, 2, [PhysicalDimension, 2]> OriginLocation ;      (r)
  DataArray_t<real, 1,  PhysicalDimension>     RigidRotationAngle ;  (o/d)
  DataArray_t<real, 1,  PhysicalDimension>     RigidVelocity ;       (o)
  DataArray_t<real, 1,  PhysicalDimension>     RigidRotationRate ;   (o)

  List( DataArray_t DataArray1 ... DataArrayN ) ;                    (o)

  DataClass_t DataClass ;                                            (o)

  DimensionalUnits_t DimensionalUnits ;                              (o)

  List( UserDefinedData_t UserDefinedData1 ... UserDefinedDataN ) ;  (o)
}
```

## [顶点作任意运动](https://cgns.github.io/CGNS_docs_current/sids/timedep.html#ArbitraryGridMotion)

**顶点作任意运动**意味着 `Elements_t`(s) 仍可复用，但顶点坐标不再能快速算出，故需为每个时间步分别创建

- 一个 `GridCoordinates_t`，用于记录该时间步的*顶点坐标*，并将其*名称*记录在 `ZoneIterativeData_t` 下的 `(DataArray_t) GridCoordinatesPointers` 中。
- 一个 `ArbitraryGridMotion_t`，用于记录其所属 `Zone_t` 的*顶点速度*，并将其*名称*记录在 `ZoneIterativeData_t` 下的 `(DataArray_t) ArbitraryGridMotionPointers` 中。

```c++
ArbitraryGridMotion_t< int IndexDimension, 
                       int VertexSize[IndexDimension], 
                       int CellSize[IndexDimension] > := {
  ArbitraryGridMotionType_t ArbitraryGridMotionType ;                (r)

  List( DataArray_t<real, IndexDimension, DataSize[]>
        GridVelocityX GridVelocityY ... ) ;                          (o)

  List( Descriptor_t Descriptor1 ... DescriptorN ) ;                 (o)

  GridLocation_t GridLocation ;                                      (o/d)

  Rind_t<IndexDimension> Rind ;                                      (o/d)

  DataClass_t DataClass ;                                            (o)

  DimensionalUnits_t DimensionalUnits ;                              (o)

  List( UserDefinedData_t UserDefinedData1 ... UserDefinedDataN ) ;  (o)
}
```

## [网格拓扑发生改变](https://cgns.github.io/CGNS_docs_current/sids/timedep.html#ex:adaptedunstructuredmesh)

**网格拓扑发生改变**意味着 `Elements_t`(s) 不再能复用，故需创建新的 `Zone_t` 以对应*网格拓扑发生变化*的各时间步，对应关系由其所属 `CGNSBase_t` 中的 `(DataArray_t) ZonePointers` 管理。示例文件 [`write_adaptive_grid.cpp`](./write_adaptive_grid.cpp) 演示了这种方法。

```c++
CGNSBase_t "RemeshingCase" {
  BaseIterativeData_t "Steps" {
    int "NumberOfSteps" = 4
    DataArray_t<double> "TimeValues" = {0, 5, 10, 15, 20}
    DataArray_t<int> "NumberOfZones" = {1, 1, 1, 1, 1}
    DataArray_t<char*> "ZonePointers" = {
      "Zone1", "Zone1", "Zone1", "Zone2", "Zone2"
    }
  }
  Zone_t "Zone1" {
    GridCoordinates_t "GridCoordinates"
    Elements_t "Elements"
    ZoneBC_t "ZoneBC"
    FlowSolution_t "Solution0"
    FlowSolution_t "Solution5"
    FlowSolution_t "Solution10"
    ZoneIterativeData_t {
      DataArray_t<char*> "FlowSolutionPointers" = {
        "Solution0", "Solution5", "Solution10", "Null", "Null"
      }
    }
  }
  Zone_t "Zone2" = {
    GridCoordinates_t "GridCoordinates"
    Elements_t "Elements"
    ZoneBC_t "ZoneBC"
    FlowSolution_t "Solution15"
    FlowSolution_t "Solution20"
    ZoneIterativeData_t {
      DataArray_t<char*> "FlowSolutionPointers" = {
        "Null", "Null", "Null", "Solution15", "Solution20"
      }
    }
  }
}
```

另一种更通用的方法是创建  CGNS 文件序列，即各时间步分别对应一个 CGNS 文件。示例文件 [`write_file_series.cpp`](./write_file_series.cpp) 演示了这种方法。在 [ParaView](../vtk/README.md#ParaView) 中加载 CGNS 文件序列时，需勾选 `Ignore FlowSolutionPointers` 及 `Ignore Reader Time`，否则所有时间步会叠在一起显示。