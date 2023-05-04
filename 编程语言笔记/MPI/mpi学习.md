# mpi学习

## 简介

一些简单的名词解释：

*通讯器*（communicator）：通讯器定义了一组能够互相发消息的进程。在这组进程中，每个进程会被分配一个序号，称作*秩*（rank），进程间显性地通过指定秩来进行通信。

通信的基础建立在不同进程间发送和接收操作。一个进程可以通过指定另一个进程的秩以及一个独一无二的消息*标签*（*tag*）来发送消息给另一个进程。接受者可以发送一个接收特定标签标记的消息的请求（或者也可以完全不管标签，接收任何消息），然后依次处理接收到的数据。类似这样的涉及一个发送者以及一个接受者的通信被称作*点对点*（point-to-point）通信。

当然在很多情况下，某个进程可能需要跟所有其他进程通信。比如主进程想发一个广播给所有的从进程，这些操作统称为进程间的*集合通信*（collective）。

### 发送模式

MPI为**发送操作**提供了四种模式：标准（standard），同步（synchronous），就绪（ready）和缓冲（buffered）。不同的模式下，MPI实现决定是将消息的内容复制到自己的存储空间，还是一直阻塞到一个相匹配的接收操作被提交。MPI标准允许以两种不同的方式来实现 :

1. 简单地将**消息复制到MPI设置的缓冲区并返回**。
2. 直到**对应的MPI_Recv出现前都阻塞**。

此外，许多MPI函数都设置了从缓冲模式到阻塞模式间切换的阀值，即相对较小的消息就交由MPI_Send缓冲，但对于大型数据就选择阻塞模式。

### 集群模式

虚拟集群的搭建需要依赖于三个配置：

1. 需要确认所有节点之间能通过 SSH 节点间通信，并且需要设置无密码的 SSH 访问0。因为MPICH在进行计算时需要在各个节点机交换数据和文件，集群内的各个节点应该为互相信任的节点。
2. 利用NFS服务来共享制定的文件夹。例如共享的安装文件夹、数据文件夹和程序的文件夹，这样就可以避免在所有节点上重复安装或者复制数据或分发程序.
3. 对MPICH的配置、编译以及安装的过程。例如配置host文件等。

#### 主机命名

为了能够清晰地分辨不同的主机，可以对MPI的主机的名称进行修改，防止命名上出现冲突。采用命令:

```shell
hostnamectl set-hostname 主机名
```

#### 修改hosts文件

hosts文件是用来对主机名或域名做IP地址解析，如果修改此文件将主机名和IP地址关联，则在后面遇到关于输入IP地址的操作都可以用主机名代替。当然，也选择始终输入IP地址不修改此文件。

编辑`/etc/hosts`文件，在末尾加上`IP地址 主机名称`。需要注意的是，所有节点都要修改hosts文件。

```
172.27.80.248 node1 
172.27.80.247 node2            
```

作为验证，可以用ping主机名验证是否成功，`ping node1` 。

#### 设置SSH

在每个计算节点都生成ssh key，使用 `ssh-copy-id` 指令在将自身的公钥上传到其它计算节点，这一步需要输入远程计算节点的用户密码。

```shell
[mpi@node1 ~]$ ssh-copy-id user01@node1	
#没有hosts的话可能是
[mpi@node1 ~]$ ssh-copy-id user01@192.168.1.104	
```

在输入密码后，SSH公钥就会自动上传，保存在远程节点的 .ssh/authorized_keys文件中。之后使用scp传输文件也不需要密码了。

#### 配置host文件

这个host文件不是用与ip解析的/etc/hosts文件，而是MPI用来配置自身服务器集群的。



## 初始化与结束

### MPI_Init

必须作为MPI中第一个调用的函数，让MPI系统进行基本的初始化，例如为消息缓冲区分配空间，为进程指定进程号等。初始化结束后，一个通信子communicator便会根据所有可用的进程被创建出来（进程是通过mpi启动程序时的参数指定的）

```c++
// 返回一个int型错误码（大部分时候忽略）
int MPI_Init(
     int *  argc_p /*in/out*/,	//指向argc的指针，不使用设置为NULL
     char *** argv_p /*in/out*/ )	//指向argv的指针，不使用设置为NULL
```

### 通信子

**通信子**是一组可以互相发送消息的进程集合。MPI_Init的其中一个目的，是在用户启动程序时，定义由用户启动的所有进程组成的通信子。

#### MPI_Comm_size

```c
int MPI_Comm_size(
    #通信子
    MPI_Comm comm /*in*/,	// 通信子
    int*  comm_sz_p /*out*/) // 返回通信子内进程的数量
```

#### MPI_Comm_rank

```c
int MPI_Comm_rank(
    MPI_Comm comm /*in*/,	// 通信子
    int*  my_rank_p /*out*/)	//返回正在调用进程在通信子的进程号
```



### MPI_Finalize

告知MPI系统MPI已经所使用完毕，为MPI而分配的所有资源都可以释放。调用这个函数后并行部分将结束，但进程并没有结束，只是不能再继续使用MPI的函数了。





## 



## 全局通信

即多个节点间通信，不只是单点间的通信

###  MPI_Alltoall & MPI_Alltoallv、

MPI_Alltoall表示通信域内每个节点都要向其它节点发送和接受数据，发送和接收的数据长度固定，由 `sendcount` 和 `sendtype` 一起决定。

```c
int MPI_Alltoall(const void *sendbuf,
                int sendcount,
                 MPI_Datatype sendtype,
                void *recvbuf, 
                int recvcount, 
                MPI_Datatype recvtype,
                MPI_Comm comm)
```

- sendbuf代表发送缓冲区的起始地址
- sendcount代表要发送的数量
- sendtype代表发送的数据类型
- recvbuf代表接收缓冲区的起始位置
- recvcount代表要接收的数量
- recvtype代表要接收的类型
- comm代表通信子

当进程需要向其它进程发送数据长度不定的数据时，就需要用MPI_Alltoallv来解决，

```
int MPI_Alltoallv(const void *sendbuf,
                    const int *sendcounts,
                    const int *sdispls,
                    MPI_Datatype sendtype,
                    void *recvbuf,
                    const int *recvcounts,
                    const int *rdispls，
                    MPI_Datatype recvtype,
                    MPI_Comm comm)
```

相比MPI_Alltoall，这个方法有几个参数不一样：

- `sendcounts` 和 `recvcounts`。这两个参数是两个数组，数组中元素代表该下标位置的进程收发的数据长度是多少个类型长度。eg：sendcounts[0]=3,sendcounts[1]=4,代表该节点要往0号节点发送3个sendtype的数据，往1号节点发送4个sendtype的数据。
- `sdispls` 和 `rdispls` 。两个数组，数组中的每个元素代表了要发送（接收）的那块数据相对于缓冲区起始位置的位移量。



## 集合通信

集合通信是指在一组进程间的通信。有如下几个要求：

- 在通信子中所有的进程必须调用相同的集合通信函数，广播的函数在指定的source进程会执行发送操作，其它的进程就执行接受操作。即同一个函数在不同进程中会有不同的效果。
- 点对点通信的函数通过标签于通信子匹配

### MPI_Reduce

用于实现高效的全局运算，如求和，求最大值等，各个节点参与计算，将结果返回到根节点处，其它节点中的结果数据为NULL。属于**树形通信**。接收缓冲区output_data_p最后只会在dest_process中，但所有的进程仍然需要传递一个output_data_p对应的参数，非目标进程的进程可以传入NULL。

<img src="mpi%E5%AD%A6%E4%B9%A0.assets/5259397-bc034e2547b709a1.jpg" alt="img" style="zoom:33%;" />

```c
int MPI_Reduce(void* input_data_p,	// 发送缓冲区地址
               void* output_data_p,	// 接受缓冲区地址（可以是NULL）
               int count,	// 发送缓冲区元素个数
               MPI_Datatype datatype, // 元素类型
               MPI_Op operator, // 规约操作
               int dest_process, // 存放结果的根进程号
               MPI_Comm comm); // 通信子
```

这里需要注意，input_data_p和output_data_p不可以是同一个地址，否则会发生未知的错误。对于非目标进程的output_data_p可以为NULL。

具体的operator规约操作有：

| 运算符值   | 含义                       |
| ---------- | -------------------------- |
| MPI_MAX    | 求最大值                   |
| MPI_MIN    | 求最小值                   |
| MPI_SUM    | 求累加和                   |
| MPI_PROD   | 求累乘值                   |
| MPI_LAND   | 逻辑与                     |
| MPI_BAND   | 按位与                     |
| MPI_LOR    | 逻辑或                     |
| MPI_BOR    | 按位或                     |
| MPI_LOR    | 逻辑异或                   |
| MPI_BOR    | 按位异或                   |
| MPI_MAXLOC | 求最大值和最大值所在的位置 |
| MPI_MINLOC | 求最小值和最小值所在的位置 |

### MPI_Allreduce

和上面的MPI_Reduce类似，但在结果接受方面有所不同，这里所有的进程都会得到结果。属于**蝶形结构通信**。

<img src="mpi%E5%AD%A6%E4%B9%A0.assets/5259397-87e902f6e548af21.jpg" alt="img" style="zoom:33%;" />

```objectivec
int MPI_Allreduce(
    void*  input_data_p /*in*/, // 发送缓冲区地址 
    void* output_data_p /*out*/, // 接收缓冲区地址 
    int  count /*in*/, // 发送缓冲区内元素数
    MPI_Datatype  datatype /*in*/, // 发送元素的数据类型
    MPI_Op  operator /*in*/, // 规约操作
    MPI_Comm  comm /*in*/) //通信子
```

### MPI_Bcast

树形结构广播，属于一个进程向通信子中的其它所有进程发送相同的消息，是阻塞式通信。通过source参数指定发送进程，如果本进程不是source进程，那么调用该函数就会阻塞，等待source进程调用该函数，将数据写入data_p中。

![img](mpi%E5%AD%A6%E4%B9%A0.assets/5259397-ce34fae747f1e751.webp)

```objectivec
int MPI_Bcast(
    void*  data_p /*in/out*/, // 接收缓冲区（已装载）
    int  count /*in*/, // 缓冲区内最大项数
    MPI_Datatype  datatype /*in*/, // 项的数据类型 
    int  source /*in*/, // 根进程序号 
    MPI_Comm  comm /*in*/)
```

### MPI_Scatter

散射，将进程中的一组消息切成N块（通信子里面的进程数），分别将每一块发送给对应的进程，属于等长分发。和MPI_Reduce一样，接受和发送的缓冲区不能是同一个。

![img](mpi%E5%AD%A6%E4%B9%A0.assets/5259397-90b63124fa382fb7.webp)

```objectivec
int MPI_Scatter(
    void* send_buf_p /*in*/, //发送缓冲区
    int  send_count /*in*/,//发送到每个进程的数据量（一般和recv_count相同）
    MPI_Datatype  send_type /*in*/,//发送元素的数据类型
    void* recv_buf_p /*out*/,//接收缓冲区（已装载）
    int   recv_count /*in*/,//每次接收的元素数
    MPI_Datatype  recv_type /*in*/,//接收元素的数据类型
    int  src_proc /*in*/,//接收进程序号
    MPI_Comm  comm /*in*/)
```

### MPI_Gather

有了把数据发出去的方法，自然会有把数据收集起来的方法，这就是聚集。用法和MPI_Scatter类似

```objectivec
int MPI_Gather(
    void* send_buf_p /*in*/,//发送缓冲区
    int  send_count /*in*/,//发送缓冲区内元素数
    MPI_Datatype  send_type /*in*/,//发送元素的数据类型
    void* recv_buf_p /*out*/,//接收缓冲区（已装载）
    int   recv_count /*in*/,//从每个进程中接收到的数据量（一般和send_count相同）
    MPI_Datatype  recv_type /*in*/,//接收元素的数据类型
    int  dest_proc /*in*/,//接收进程序号
    MPI_Comm  comm /*in*/)
```





## MPI派生数据类型

如果发送数据的函数知道数据项的**类型**以及在内存中数据项集合的**相对位置**，就可以在数据项被发送出去之前将数据项**聚集**起来。类似地，接收数据的函数可以在数据项被接收后将数据分发到它们的内存中的正确的目标地址中。所以，一个**派生数据类型**是由一系列的MPI**基本数据类型**和**每个数据类型的偏移量**所组成的。

### MPI_Get_address

获取变量的系统地址，用于提取变量和之后计算偏移量。

```c
int MPI_Get_address(
    void* location_p /*in*/,	//所指向的内存单元的地址
    MPI_Aint* address_p /*out*/) //系统中真实的内存地址，用于之后寻址
```

### MPI_Type_create_struct

创建由不同基本数据类型的元素所组成的派生数据类型。

```objectivec
int MPI_Type_create_struct(
    int  count /*in*/,	//数据类型中元素项个数
    int  array_of_blocklengths[] /*in*/, //每个数据项中的元素个数（为了适配数组）
    MPI_Aint  array_of_displacements[] /*in*/, //每个数据项距离起始位置的偏移量
    MPI_Datatype  array_of_types[] /*in*/, //每个数据项的类型
    MPI_Datatype*  new_type_p /*out*/) //自定义数据类型出口
    
// eg：
MPI_Datatype type;    
MPI_Type_create_struct(3, {1,1,1}, {0,16,24}, {MPI_DOUBLE, MPI_DOUBLE, MPIINT}, &type);
// 即创建一个内涵三个元素，double、double、int的结构体，返回这个结构的类型type。
```

### MPI_Type_commit

将创建的MPI类型注册进MPI中，让其能够调用。（不经过这一步，其它的进程是不能识别这个类型的）

```c++
int MPI_Type_commit(
    MPI_Datatype* new_mpi_t_p /*in/out*/) //MPI_Type_create_struct返回的type
```

### MPI_Type_free

当然，动态创建一个MPI类型，肯定是要占用一定的内存空间的，所以当这个类型已经不会被使用时，可以调用free去释放存储空间。

```objectivec
int MPI_Type_free(
    MPI_Datatype* old_mpi_t_p /*in/out*/) // 构建的派生类型
```



## MPI错误处理

几乎任何 MPI 调用都返回一个整数错误代码，表示操作成功。如果没有错误发生，返回码为 `MPI_SUCCESS`； 如果发生错误，MPI 会在返回用户代码之前调用与通信器，窗口或文件对象关联的错误处理程序。有两个预定义的错误处理程序（用户可以定义其他错误处理程序）：

- `MPI_ERRORS_ARE_FATAL` - 错误导致 MPI 程序终止
- `MPI_ERRORS_RETURN` - 错误导致错误代码传递回用户

通信器和窗口的默认错误处理程序是 `MPI_ERRORS_ARE_FATAL`; 对于文件对象，它是 `MPI_ERRORS_RETURN`。`MPI_COMM_WORLD` 的错误处理程序也适用于与对象无关的所有操作（例如，`MPI_Get_count`）。因此，在不将错误处理程序设置为 `MPI_ERRORS_RETURN` 的情况下检查非 I / O 操作的返回值是多余的，因为错误的 MPI 调用将不会返回。

```c
// The execution will not reach past the following line in case of error
int res = MPI_Comm_size(MPI_COMM_WORLD, &size);
if (res != MPI_SUCCESS)
{
    // The following code will never get executed
    fprintf(stderr, "MPI_Comm_size failed: %d\n", res);
    exit(EXIT_FAILURE);
}
```

要启用用户错误处理，必须先更改 `MPI_COMM_WORLD` 的错误处理程序：

```c
MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

int my_rank = 0;
int size = 0;
int res = MPI_Comm_size(MPI_COMM_WORLD, &size);
res = MPI_Comm_rank(MPI_Comm comm, &my_rank);
if (res != MPI_SUCCESS)
{
    fprintf(stderr, "MPI_Comm_size failed: %d\n", res);
    MPI_Abort(my_rank, -1);//终止MPI程序
}
```

MPI 标准不要求 MPI 实现能够从错误中恢复并继续执行程序。

