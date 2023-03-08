/*========== C语言用宏函数实现简单的元编程 ============*/

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
        func_name(data)\
        type data;\
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