# Linux的Shell脚本学习手册

## 简介

Linux中的脚本保存为 `.sh` 文件, Windows中的脚本一般保存为 `.ps1` 文件, 可以通过 `.bat` 文件调用运行. 

需要注意的是, 运行脚本时需要确保shell脚本有对应的运行权限, 在Linux中对应为x权限, 如果提示 `permission deny` , 可以用 `chmod name.sh 775` 提升权限后再运行

### 运行方法

- 在当前目录中运行

  ```shell
  ./hello.sh
  ```

- 用绝对目录运行

  ```shell
  /home/lemon/hello.sh
  ```

- 指定用bash或sh运行

  ```shell
  bash hello.sh #指定用bash作为启动器, 这时脚本中第一行就不用指定bash
  sh hello.sh
  ```

- 在当前的shell中运行. 前三种方式都是当前的shell创建了一个子shell来运行, 如果在脚本中有export输入环境变量等命令, 是不会影响到当前shell的, 这时就需要用 `source` 指令指定在当前shell中运行

  ```shell
  source hello.sh
  ```



## 特殊参数说明

- `$name`: 表示一个变量, 当定义了一个变量以后, 访问的时候需要用到\$符号

- `$1` ~ `$n` : 执行脚本时输入的命令行参数, 其中 `$0` 是脚本本身的名字, 剩下的依次为各个参数

  ```shell
  ./hello.sh nihao 1 3
  $1 = nihao, $2 = 1, $3 = 3
  ```

- `$#`: 命令行参数的个数

- `$!`: 最后运行的后台process的PID

- `$$`: shell自身的PID

- `$*`: 所有参数列表。如 `$*` 用 [] 括起来的话, 以"\$1 \$2 … \$n"的形式输出所有参数。 

-  `$@`: 所有参数列表。如"\$@"用 [] 括起来的话, 以"\$1" "\\$2" … "\$n" 的形式输出所有参数。



## 判断指令(if case)

### if判断

- `if` 语句做条件判断, 用 `if` 开头, `fi` 结尾, 用 `else` 做 双分支, 用 `elif` 做多分支判断

- 中间用 `[ ]` 表示判断条件, **两端必须有空格**, 可以用双括号 `(()())` 或 ` [[][]]`表示内嵌

- `if` 如果和 `then` 在一行, 需要在中间加一个 `;`

  ```shell
  if [ 条件判断 ];then
  	命令
  else 
  	命令
  fi
  
  # 或者
  if [ 条件判断 ]
  	then
  		命令
  elif [ 条件判断 ]
  	命令
  else 命令
  fi
  ```


### case in判断

类似于switch结构, 根据变量的值来判断进入哪个分支, 基本语法为:

- case会将变量的值于列出来的情况逐个对比, 如果符合就进入对应的分支, 如果都不符合, 则进入最后的 `*)` 分支.  `*)` 分支可以不存在, 想当于 `default`
- case某个分支以 `;;` 为结束符, 整个case以 `esac` 为结束符, 相当于 `break`, 但每个分支都必须写上 `;;` 结尾作为分支的结束, 否则报错
- 情况case可以是字符串, 数字或者是正则表达式, 其实 `*)` 就是任意匹配的正则表达式

```shell
case 变量 in
	情况1)
		执行指令;;
	情况2)
		执行指令;;
	*)
		执行指令
esac
```



## 循环指令(for while until)

### for循环

- **python风格**: `for ... in...` 从变量中取值, 从一个list序列中获取单个的值, 这个序列可以是数字序列, 也可以是一串字符
  
  - 数字序列: 针对 `{1...5}` 类似的数字序列, 效果为逐个从中取出数据, eg:
  
    ```shell
    # 取i的时候要用$i获取变量值
    for 变量 in 变量序列
    do
    	执行指令 $i
    done
    
    for i in {1...5} # 获取1-5
    for i in ${seq 1 10} # 挨个获取1,10序列中的数
    ```
  
- 字符串序列: 一般用于执行指令后的函数, 例如执行 `ls` 后打印元素, 或者逐个获取输入值eg:
  
    ```shell
    for i in `ls` # 从ls返回值中挨个取值
  ```
  
- **c语言风格** : 和c语言的for循环一样

    ```shell
    for(i= 1; i < 3; i++)
    ```

### while循环

一般是用在控制脚本执行流程上, while重复执行一系列指令, 当条件不满足时才跳出while

循环, 如果把判断条件设置为 *true*, 那么while将会不能退出. 语法为:

```shell
while 判断条件
do
	执行指令
done
```

### until循环

until刚好和while循环相反, until是不满足条件时才会进入循环, 若条件满足则立刻跳出循环, 语法为:

```shell
until 判断条件
do 
	执行指令
done
```

### 循环控制及退出指令

- `break` : 立刻退出循环
- `continue` : 跳出这一次循环, 进入下一次循环
- `exit` : 退出脚本
- `return` : 退出这个函数



## 比较判断

### 数字比较

| 指令      | 描述                     |
| :-------- | :----------------------- |
| n1 -eq n2 | n1是否与n2相等, 相等为真 |
| n1 -ge n2 | n1是否大于或等于n2       |
| n1 -gt n2 | n1是否大于n2             |
| n1 -le n2 | n1是否小于或等于n2       |
| n1 -lt n2 | n1是否小于n2             |
| n1 -ne n2 | n1是否不等于n2           |

例如

```shell
#!/bin/base
number1=$1
number2=$2

# -z表示number2长度为0
if [ -z $number2 ];then
	echo "please imput all args"
	exit
fi

# 比较两个数大小
if [ $number1 -gt $number2];then
	echo "number1 > number2"
else
	echo "number1 < number2"
	
./hello.sh 1 2
> number1 < number2
```

### 字符串比较

类似于数字比较

| 指令         | 描述              |
| :----------- | :---------------- |
| str1 = str2  | str1和str2相同    |
| str1 != str2 | str1和str2不相等  |
| str1 < str2  | str1是否比str2小  |
| str1 > str2  | str1是否比str2大  |
| -n str1      | str1的长度是否非0 |
| -z str1      | str1的长度是否为0 |

### 文件比较

| 指令            | 描述                                                         |
| :-------------- | :----------------------------------------------------------- |
| -d file         | file是否存在并是一个目录                                     |
| -e file         | file是否存在                                                 |
| -f file         | file是否存在并是个文件                                       |
| -r file         | file是否存在并可读                                           |
| -s file         | file是否存在并非空                                           |
| -w file         | file是否存在并可写                                           |
| -x file         | file是否存在并可执行                                         |
| -O file         | file是否存在并属当前用户所有                                 |
| -G file         | file是否存在并且默认组与当前用户相同                         |
| file1 -nt file2 | file1是否比file2新                                           |
| file1 -ot file2 | file1是否比file2旧                                           |
| file1 -ef file2 | 判断file1是否和file2的Inode号一致, 即两个文件是否为同一个文件。多用于判断硬链接。 |

例如:

```shell
#!/bin/base
if[ -d $/home/lemon ];then
	echo "this is a dir and exist"
else echo "this is not a dir or not exist"
fi

./hello.sh
> this is a dir and exist
```

### 多重条件判断

| 指令           | 描述                   |
| -------------- | ---------------------- |
| expr1 -a expr2 | 逻辑与                 |
| expr1 -o expr2 | 逻辑或                 |
| !expr          | 逻辑非                 |
| [] \|\| []     | 用**或**来合并两个条件 |
| [] && []       | 用**与**来合并两个条件 |

例如:

```shell
#!/bin/bash
read -p "please input a score:" score
echo -e "your score [$score] is judging by sys now"

if [ "$score" -ge "0" ]&&[ "$score" -lt "60" ];then
    echo "sorry,you are lost!"
elif [ "$score" -ge "60" ]&&[ "$score" -lt "85" ];then
    echo "just soso!"
elif [ "$score" -le "100" ]&&[ "$score" -ge "85" ];then
    echo "good job!"
else
    echo "input score is wrong , the range is [0-100]!"
fi

```



## 函数

定义函数类似于c语言, 就是将一段操作封装, 作为可以重复使用的某个部件, 基本的语法如下:

- `function` 关键字可以省略, 但建议保留, 可读性更强,  `return` 也可以没有, 即不返回任何值
- 调用函数的时候直接写出函数名即可, 需要传递参数的话直接在后面加上即可, 多个参数用空格分开即可
- 可以通过 `$1` 获取传入的参数

```shell
function 函数名(){
	执行指令
	[return 返回值] #可以不写
}

# eg:
function func(){
	if [ ! $1 ];then
		echo "error"
	else
		echo $1
	fi
}

#调用
func nihao wo 
> nihao
```







