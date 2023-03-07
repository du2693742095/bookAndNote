# CGNS库函数

c语言相关函数返回值基本都是ier（int）错误状态，用之后的错误处理函数处理。



## 文件操作



### 打开和关闭文件

- `cg_open` - Open a CGNS file
- `cg_version `- Get CGNS file version
- `cg_precision` - Get CGNS file precision
- `cg_close` - Close a CGNS file
- `cg_is_cgns` - Check for a valid CGNS file
- `cg_save_as` - Save the open CGNS file
- `cg_set_file_type` - Set default file type
- `cg_get_file_type` - Get file type for open CGNS file

#### 函数

```c
ier = cg_open(char *filename, int mode, int *fn);
ier = cg_version(int fn, float *version);
ier = cg_precision(int fn, int *precision);
ier = cg_close(int fn);
ier = cg_is_cgns(const char *filename, int *file_type);
ier = cg_save_as(int fn, const char *filename, int file_type, int follow_links);
ier = cg_set_file_type(int file_type);
ier = cg_get_file_type(int fn, int *file_type);
```

#### 参数说明

| 变量名       | 功能                                                         |
| ------------ | ------------------------------------------------------------ |
| filename     | CGNS 文件的名称，必要时包括路径名。这个字符变量的长度没有限制。 |
| mode         | 用于打开文件的模式。当前支持的模式是`CG_MODE_READ`（只读）、 `CG_MODE_WRITE`（只写）和`CG_MODE_MODIFY`（读写）。 |
| fn           | CGNS 文件索引号。                                            |
| version      | CGNS 版本号。                                                |
| precision    | 指明CGNS 文件的精度。返回值将是 32（32 位）、64（64 位）之一，如果未知则为 0。 |
| file_type    | CGNS 文件的类型。这通常是`CG_FILE_ADF` 或`CG_FILE_HDF5`，具体取决于基础文件格式。但是，当以 32 位构建时，还有一个选项可以通过将文件类型设置为 `CG_FILE_ADF2`来创建2.5版本的CGNS 文件。 |
| follow_links | 此标志确定在保存 CGNS 文件时链接是否保持不变。如果非零，则链接将被删除，与链接文件相关的数据将复制到新文件。 |
| ier          | 错误码。                                                     |

#### 注意事项

- 函数`cg_open`必须始终是第一个被调用的。它打开一个 CGNS 文件进行读取和/或写入，并返回一个索引号`fn`。索引号用于在后续函数调用中标识 CGNS 文件。可以同时打开多个 CGNS 文件。当前一次打开的文件数限制取决于平台。在 SGI 工作站上，此限制设置为 100（ *stdio.h*中的参数`FOPEN_MAX`）。
- 对于现有文件，函数`cg_is_cgns`可用于确定文件是否为 CGNS 文件，以及文件类型（`CG_FILE_ADF`或`CG_FILE_HDF5`）。如果文件是 CGNS 文件， 则 `cg_is_cgns`返回`CG_OK`，否则 返回`CG_ERROR`并将`file_type`设置为`CG_FILE_NONE`。
- 打开文件时，如果未找到`CGNSLibraryVersion_t`节点，则假定 CGNS 版本号的默认值为 1.05。这对应于 CGNS 标准的旧版本，不包括当前标准支持的许多数据结构。
- 函数`cg_close`必须始终是最后调用的函数。它关闭由索引号`fn`指定的 CGNS 文件并释放保存 CGNS 数据的内存。当打开文件进行写入时，`cg_close`在关闭文件之前将内存中的所有 CGNS 数据写入磁盘。因此，如果省略，则 CGNS 文件不会被正确写入。
- 为了减少内存占用和提高执行速度，网格坐标或流解等大型数组实际上并不存储在内存中，只保留有关节点的基本信息，而数据的读写直接在应用程序的内存中进行。还尝试对非结构化网格元素数据执行相同的操作。
- 当使用`CG_MODE_WRITE` 创建新 CGNS 文件时，所使用的默认数据类型在编译时确定。如果 CGNS 库是使用 HDF5 版本 1.8 或更高版本支持构建的，文件类型将为`CG_FILE_HDF5`，否则使用`CG_FILE_ADF`。这可以通过将环境变量 `CGNS_FILETYPE`设置为`adf`、`hdf5`或 `adf2` 之一，或者通过在调用 cg_open 之前调用例程`cg_set_file_type`来更改。 使用参数`CG_FILE_NONE`调用`cg_set_file_type`将重置库以使用默认文件类型。注意：如果设置了环境变量 `CGNS_FILETYPE`，则优先生效。
- 由`fn` 标识的 CGNS 文件可以使用`cg_save_as`保存为不同的文件名和类型。为了另存为 HDF5 文件，库必须已构建为支持 HDF5。始终构建 ADF 支持。函数`cg_set_file_type`为新创建的 CGNS 文件设置默认文件类型。函数 `cg_get_file_type`返回由`fn`标识的 CGNS 文件的文件类型。如果 CGNS 库构建为 32 位，则附加文件类型`CG_FILE_ADF2`可用。这允许创建一个 2.5 兼容的 CGNS 文件。



### 配置cgns文件结构

- `**cg_configure**` - Configure CGNS internals
- `**cg_error_handler**` - Set CGNS error handler
- `**cg_set_compress**` - Set CGNS compression mode
- `**cg_get_compress**` - Get CGNS compression mode
- `**cg_set_path**` - Set the CGNS link search path
- `**cg_add_path**` - Add to the CGNS link search path

#### 函数

```c
ier = cg_configure(int option, void *value);
ier = cg_error_handler(void (*)(int, char *));
ier = cg_set_compress(int compress);
ier = cg_get_compress(int *compress);
ier = cg_set_path(const char *path);
ier = cg_add_path(const char *path);
```

#### 参数说明

| 变量名     | 作用                                                         |
| ---------- | ------------------------------------------------------------ |
| `option`   | The option to configure, currently one of `CG_CONFIG_ERROR`, `CG_CONFIG_COMPRESS`, `CG_CONFIG_SET_PATH`, `CG_CONFIG_ADD_PATH`, `CG_CONFIG_FILE_TYPE`, `CG_CONFIG_RIND_INDEX`, `CG_CONFIG_HDF5_DISKLESS`, `CG_CONFIG_HDF5_DISKLESS_INCR`, `CG_CONFIG_HDF5_DISKLESS_WRITE`, `CG_CONFIG_HDF5_COMPRESS`, or `CG_CONFIG_HDF5_MPI_COMM` as defined in *cgnslib.h*. |
| `value`    | The value to set, type cast as `void *`. In Fortran the type is `TYPE(C_PTR)`. |
| `compress` | CGNS compress (rewrite) setting.                             |
| `path`     | Pathname to search for linked to files when opening a file with external links. |

#### 注意事项

- 要使用cg_error_handle，需要先用cg_configure定义遇CG_CONFIG_ERROR，相当于注册回调函数。
- 函数cg_error_handler如果`flag` 不为零，那么当遇到错误时，库将打印错误消息并以代码 1 退出。将`flag`设置为零（默认值）可以防止这种情况，并将错误返回给用户代码。





## 文件跳转

### Accessing a Node

- `**cg_goto**` - Access a node via label/name, index pairs
- `**cg_gorel**` - Access a node via relative path
- `**cg_gopath**` - Access a node via pathname
- `**cg_golist**` - Access a node via arrays of labels and indices
- `**cg_where**` - Get path to current node

### Deleting a Node

- `**cg_delete_node**` - Delete a node



## 错误处理

```c
//获取错误信息
error_message = const char *cg_get_error();

//错误退出（一般不用）
void cg_error_exit();                        

//直接打印错误信息
void cg_error_print();   
```



## Node结构

### CGNS Base Information

- `cg_base_write` - Create and/or write to a CGNS base node
- `cg_nbases` - Get number of CGNS base nodes in file
- `cg_base_read` - Read CGNS base information
- `cg_cell_dim` - Get the cell dimension for the CGNS base

#### 函数

```c
ier = cg_base_write(int fn, char *basename, int cell_dim, int phys_dim, int *B);
ier = cg_nbases(int fn, int *nbases);
ier = cg_base_read(int fn, int B, char *basename, int *cell_dim,int *phys_dim);
ier = cg_cell_dim(int fn, int B, int *cell_dim);
```

#### 参数说明

| 变量名     | 功能                                                         |
| ---------- | ------------------------------------------------------------ |
| `fn`       | CGNS file index number.                                      |
| `B`        | Base index number, where 1 ≤ `B` ≤ `nbases`.                 |
| `nbases`   | Number of bases present in the CGNS file `fn`.               |
| `basename` | Name of the base.                                            |
| `cell_dim` | Dimension of the cells; 3 for volume cells, 2 for surface cells and 1 for line cells. |
| `phys_dim` | Number of coordinates required to define a vector in the field. |

### Zone Information

- `cg_zone_write` - Create and/or write to a zone node
- `cg_nzones` - Get number of zones in base
- `cg_zone_read` - Read zone information
- `cg_zone_type` - Get type of zone (structured or unstructured)aa·`cg_index_dim` - Get the index dimension for the CGNS zone

#### 函数

```c
ier = cg_zone_write(int fn, int B, char *zonename, cgsize_t *size,ZoneType_t zonetype, int *Z);
ier = cg_nzones(int fn, int B, int *nzones);
ier = cg_zone_read(int fn, int B, int Z, char *zonename, cgsize_t *size);
ier = cg_zone_type(int fn, int B, int Z, ZoneType_t *zonetype);
ier = cg_index_dim(int fn, int B, int Z, int *index_dim);
```

#### 参数说明

| 变量名   | 功能                                                         |
| -------- | ------------------------------------------------------------ |
| fn       | CGNS 文件索引号。                                            |
| B        | 基本索引号，其中 1 ≤ `B` ≤ `nbases`。                        |
| Z        | 区域索引号，其中 1 ≤ `Z` ≤ `nzones`。                        |
| nzones   | base`B`中存在的区域数。                                      |
| zonename | 区域的名称。                                                 |
| size     | 每个 索引--维度 中的顶点、单元格和边界顶点的数量。对于结构化网格，维度在数组中具有单位跨度（例如`[NVertexI, NVertexJ, NVertexK, NCellI, NCellJ, NCellK, NBoundVertexI, NBoundVertexJ, NBoundVertexK]`）。 |

* 请注意，对于非结构化网格，单元格的数量是最高阶元素的数量。因此，在三个维度中，它是 3-D 单元的数量，在二维中，它是 2-D 单元的数量。同样对于非结构化网格，如果节点在内部节点和边界节点之间排序，则可选参数 `NBoundVertex`必须设置为等于边界节点的数量。默认情况下，`NBoundVertex`等于零，这意味着节点未排序。`请注意， NBoundVertex` 的非零值仅适用于非结构化网格。对于结构化网格，`NBoundVertex`参数在所有方向上始终等于 0。

### Simulation Type

- `**cg_simulation_type_write**` - Write simulation type
- `**cg_simulation_type_read**` - Read simulation type



## 网格操作



### zone网格坐标

- `cg_grid_write` - Create a `GridCoordinates_t` node
- `cg_ngrids` - Get number of `GridCoordinates_t` nodes
- `cg_grid_read` - Get name of a `GridCoordinates_t` node
- `cg_grid_bounding_box_read` - Get bounding box associated to a `GridCoordinates_t` node
- `cg_grid_bounding_box_write` - Write bounding box associated to a `GridCoordinates_t` node
- `cg_coord_write` - Write grid coordinates
- `cg_coord_partial_write` - Write subset of grid coordinates
- `cg_coord_general_write` - Write shaped array to a subset of grid coordinates
- `cg_ncoords`- Get number of coordinate arrays
- `cg_coord_info` - Get info about a coordinate array
- `cg_coord_read` - Read grid coordinates
- `cg_coord_general_read` - Read subset of grid coordinates to a shaped array

#### 函数

```c
ier = cg_grid_write(int fn, int B, int Z, char *GridCoordName, int *G);
ier = cg_ngrids(int fn, int B, int Z, int *ngrids);
ier = cg_grid_read(int fn, int B, int Z, int G, char *GridCoordName);
ier = cg_grid_bounding_box_write(int fn, int B, int Z, int G, DataType_t datatype, void *bbox_array);
ier = cg_grid_bounding_box_read(int fn, int B, int Z, int G, DataType_t datatype, void *bbox_array);
```

#### 参数说明

| 变量名             | 功能                                                         |
| ------------------ | ------------------------------------------------------------ |
| fn                 | CGNS文件索引号                                               |
| B                  | 基本索引号，其中 1 ≤ `B` ≤ `nbases`。                        |
| Z                  | 区域索引号，其中 1 ≤ `Z` ≤ `nzones`。                        |
| `G`                | 网格索引号，其中 1 ≤ `G` ≤ `ngrids`。                        |
| ngrids             | 区域`Z`的`GridCoordinates_t`节点数。                         |
| GridCoordinateName | `GridCoordinates_t`节点的名称。请注意，名称“ `GridCoordinates` ”是为原始网格保留的，并且必须是要定义的第一个`GridCoordinates_t` 节点。 |
| datatype           | 写入文件或读取的边界框数组的数据类型。坐标边界框可接受的数据类型是`RealSingle` 和`RealDouble`。 |
| `bbox_array`       | 具有边界框值的数据数组。                                     |