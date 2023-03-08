# C语言用宏函数实现简单的元编程

最近在一个C语言项目中看到一个头文件中很多的函数声明, 但是死活都找不到函数的定义, 只是看到有一个宏在疯狂的使用, 并且里面全都是那些定义的函数名, 仔细研究了一下, 发现其实它是用宏函数实现了一点最原始的的元编程, 学习一下.

## 宏简介

一般来说, c语言中的宏在使用方式上被分为2种: 宏定义和带参数的宏(即一般说的宏函数), 但实际上宏在c语言种只执行一种功能, 那就是 **文本替换 **, 只是简单的将一段文本替换为另一段文本, 而没有考虑其中的语义信息, 也不会有任何的错误检查, 展开来说包括:

- 由 `#define` 指令定义符号, 替换使用这个符号的代码段
- 插入或删除被 `#include` 指令包含的文件的内容, 一般由保护式头文件声明来实现
- 指定编译条件, 根据传入编译器的指令决定编译的代码内容

⚠️ 一般来说宏都会用大小来表示, 而且在c++工程中最好少用宏, 尽量可以用const/static/inline来代替, 这样可以在编译阶段就进行错误检查


## 宏函数(带参数的宏)
带参数的宏可以在定义的宏指令后面用 `()` 将参数写出来, 在编译阶段展开时进行对应的文本替换. 基于这种原理, 理论上我们就可以在宏里面用传入的名字当作函数名, 用传入的类型当作参数类型, 在编译阶段实现简单的代码生成.
案例如下: 

```c
#include <stdio.h>

void Foo(int data)
{
        printf("I am callback, data is %d\n", data);
}

/** @brief func_name 展开为函数名
 *  @brief func_name_string 函数名的字符串,用于日志输出
 *  @type 函数参数类型
 *  @callback 回调函数
 * */
#define FUNC_GENERATE(func_name, func_name_string, type, callback)\
        void \
        func_name(type data)\
        {\
        int _data = data + 1;\
        printf("I am func; func_name: "func_name_string"\n");\
        callback(_data);\
        }

// 宏函数在这里展开
FUNC_GENERATE(FuncPrint, "FuncPrint", int, Foo)

int main()
{
        // 调用已经展开的宏函数
        FuncPrint(10);
        return 0;
}
```

用 `gcc -E` 预编译指令查看展开的函数如下, 其实也只是做了简单的文本替换
```c
# 868 "/usr/include/stdio.h" 3 4

# 2 "CMacroFunction.c" 2

# 3 "CMacroFunction.c"
void Foo(int data)
{
 printf("I am callback, data is %d\n", data);
}
# 24 "CMacroFunction.c"
void FuncPrint(int data){ int _data = data + 1; printf("I am func; func_name: ""FuncPrint""\n"); Foo(_data); }

int main()
{
 FuncPrint(10, 2);
 return 0;
}
```


## 升级版本

之前看到的祖传老代码还有一种写法, 是直接把需要的参数定义在后面, 用分号隔开, 例如下面这样

```c

/** @brief func_name 展开为函数名
 *  @brief func_name_string 函数名的字符串,用于日志输出
 *  @type 函数参数类型
 *  @callback 回调函数
 * */
#define FUNC_GENERATE(func_name, func_name_string, type, callback)\
        void \
        func_name(data, a)\
        type data;\
        int a;\
        {\
        int _data = data + 1;\
        printf("I am func; func_name: "func_name_string"\n");\
        callback(_data);\
        }

// 宏函数在这里展开
FUNC_GENERATE(FuncPrint, "FuncPrint", int, Foo)

int main()
{
        // 调用已经展开的宏函数
        FuncPrint(10, 2);
        return 0;
}
```
函数展开以后如下:
```c
void FuncPrint(data, a) int data;int a;{ int _data = data + 1; printf("I am func; func_name: ""FuncPrint""\n"); Foo(_data); }
```

这时候的每个参数声明 `type data; int a;` 必须要打分号, 不然编译就报错, 这种函数声明还没见过, 于是去查了*c reference*文档, 原来是以前的函数定义方法, 可以不定义函数原型, 在调用的时候再去下面找是否有参数匹配, 如果不匹配则判断为未定义的调用. 

总结起来就是: C语言之前的语法可以将参数的类型定义在函数体外, 函数体内部只用形参占据参数位置即可
> **Syntax**
> noptr-declarator ( identifier-list ) attr-spec-seq(optional)	(2)	(until C23)
> where:
> identifier-list: comma-separated list of identifiers, only possible if this declarator is used as part of old-style function definition
attr-spec-seq: (C23)an optional list of attributes, applied to the function type
>
> **explain**
> Old-style (K&R) function definition. This declaration does not introduce a prototype and any future function call expressions will perform default argument promotions and will invoke undefined behavior if the number of arguments doesn't match the number of parameters.
> 
> **demo**
> int max(a, b) 
int a, b; { return a>b?a:b; } // definition expects ints; the second call is undefined
int n = max(true, (char)'a'); // calls max with two int args (after promotions)
int n = max(12.01f, 3.14); // calls max with two double args (after promotions)



