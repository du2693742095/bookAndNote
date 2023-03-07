# CMake学习笔记



## 设置输出目录

主要是几个变量：

1. `CMAKE_ARCHIVE_OUTPUT_DIRECTORY`：默认存放静态库的文件夹位置；
2. `CMAKE_LIBRARY_OUTPUT_DIRECTORY`：默认存放动态库的文件夹位置；
3. `LIBRARY_OUTPUT_PATH`：默认存放库文件的位置，如果产生的是静态库并且没有指定`CMAKE_ARCHIVE_OUTPUT_DIRECTORY` 则存放在该目录下，动态库也类似；
4. `CMAKE_RUNTIME_OUTPUT_DIRECTORY`：存放可执行软件的目录；

例如：

```cmake
cmake_minimum_required(VERSION 3.1) 
project(TEST)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/archive)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/runtime)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/path)
add_subdirectory(fun)
```





## 设置可执行文件的输出目录

```
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
```

在Win + VS环境下,会自动在你所设置的目录后面扩展一层 目录,所以最终生成的Debug版本程序会在 **${PROJECT_SOURCE_DIR}/../bin/Debug**目录下,Release版本程序会在 **${PROJECT_SOURCE_DIR}/../bin/Release**目录下.
``
在Linux + GCC环境下,无论是Debug还是Release,生成的可执行程序会直接放在你所设置的目录下,不会有差异.

## 设置库文件的输出目录

```
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
```

在Win + VS环境下,会自动在你所设置的目录后面扩展一层 目录,所以最终生成的Debug版本库会在 **${PROJECT_SOURCE_DIR}/../lib/Debug**目录下,Release版本库会在 **${PROJECT_SOURCE_DIR}/../lib/Release**目录下.
``
在Linux + GCC环境下,无论是Debug还是Release,生成的库文件会直接放在你所设置的目录下,不会有差异.

## 分别设置了Debug版本和Release版本可执行文件的输出目录

```
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin)
```

一旦设置上面的属性,在任何环境下生成的库文件都将直接放在你所设置的目录.

## 分别设置了Debug版本和Release版本下库文件的后缀名

```
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELEASE_POSTFIX "_r")
```

## 分别设置了Debug版本和Release版本下可执行文件的后缀名.

```
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "_r")
```