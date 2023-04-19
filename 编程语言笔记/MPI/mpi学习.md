# mpi学习

## 简介

mpi间的通信是根据节点ID来的



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

### MPI_Reduce

用于实现高效的全局运算，如求和，求最大值等，各个节点参与计算，将结果返回到根节点处。

```c
int MPI_Reduce(void* input_data_p,
               void* output_data_p,
               int count,
               MPI_Datatype datatype,
               MPI_Op operator,
               int dest_process,
               MPI_Comm comm);
```

- input_data_p指向每个进程想要参与运算的数据，output_data_p只在根节点中有效，包含计算的结果
- operator表示对数据的操作，具体有一下类型：

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