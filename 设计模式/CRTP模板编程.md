## CRTP模板编程(模板注入)

### 概念

**CRTP**是`Curiously Recurring Template Pattern`的缩写，中文译为**奇异的递归模板模式**, 是派生类将自己作为模板参数传递给基类, 达到模板注入的效. 其优点是可以节约动态多态查找虚函数表的时间, 在编译阶段就实现静态多态. 定义方式类似于

```c++
template <class T>
class Base
{
    // methods within Base can use template to access members of Derived
};
class Derived : public Base<Derived>
{
    // ...
};
```

###### 特点

- 继承自模板类
- 派生类将自身作为参数传给模板类
- 不用虚函数, **实质上是派生类重写同名函数时, 对基类函数的屏蔽**

###### 颠倒继承

对于CRTP，是在**基类中调用派生类**的成员函数。而对于virtual虚函数，则是**派生类中调用基类**的成员函数，这就是我们所说的`颠倒继承`. 



### 实例

```c++
template <typename T>
class Base {
public:
    void interface() {
        static_cast<T*>(this)->imp();
    };
};

class Derived : public Base<Derived> {
public:
    void imp() {
        std::cout<< "in Derived::imp" << std::endl;  
    }
};

int main() {
  Derived b;
  d.interface(); 
  return 0;
}
```

1. 这里面使用static_cast<>而不用dynamic_cast<>的原因是 dynamic_cast应用于运行时，而模板是在编译器就进行了实例化, 所以只能用static进行转换.
2. 实质上, 这里在调用d.inerface() 的时候, 调用的是Base中的inerface(), 然后再通过static_cast转换为Derived指针进行多态函数的调用, 这个转换实际上是在编译阶段就已经做好了的



### 静态多态实例

```c++
#include <iostream>

template <typename T>
class Base{
 public:
  void interface(){
    static_cast<T*>(this)->imp();
    //static_cast<>中只能使用&或*, 否则报错
    T& temp = static_cast<T&>(*this);
    std::cout << typeid(temp).name() << std::endl;
  }
    
  void imp(){
    std::cout << "in Base::imp" << std::endl;
  }
};

class Derived1 : public Base<Derived1> {
 public:
  void imp(){
    std::cout << "in Derived1::imp" << std::endl;
  }
};

class Derived2 : public Base<Derived2> {
 public:
  void imp(){
    std::cout << "in Derived2::imp" << std::endl;
  }
};

struct Derived3 : public Base<Derived3>{};

template <typename T>
void fun(T& base){
    base.interface();
}

int main(){
  Derived1 d1;
  Derived2 d2;
  Derived3 d3;

  fun(d1);
  fun(d2);
  fun(d3);
}
```

在调用fun的时候, 首先将模板T转换为对应的派生类类型, 然后调用派生类中, 从基类继承下来的interface函数, 在通过interface中的static_cast转换为派生类指针, 从而表现出虚函数.

注意:

被递归模板模式使用的基类模板`base`中，不能使用模板参数类型T的对象，**只能使用T的指针或引用**，因为在base中，T的类型是不完整的，因为T还要继承自`base`。

