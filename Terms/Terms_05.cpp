// Authors: Handling
// Created by 15870 on 2020/11/13.
// This is 了解C++默默编写并调用哪些函数

/* 实验一： 查阅一个 empty 类编译器为其做的事情*/
#include <string>
namespace experiment1 {
class Empty {

};

//编译器生成展开后
class EmptyAutoGenerate {
 public:
  EmptyAutoGenerate() { }        //默认构造函数
  EmptyAutoGenerate(const EmptyAutoGenerate &) { }  //拷贝构造函数
  ~EmptyAutoGenerate() { }                    //析构函数

  EmptyAutoGenerate& operator=(const EmptyAutoGenerate &rhs) {} // 拷贝赋值运算符
};

void Test() {
  Empty e1; //默认构造函数
  Empty e2(e1); //拷贝构造函数
  e2 = e1;      //拷贝赋值运算符

  //e2,e1 的析构函数
}


}


/* 实验二： 编译器合成的函数做的事情*/
namespace experiment2 {
template <typename T>
class NamedObject {
 public:
  //如果其中你声明了一个构造函数，编译器就不会为其创建默认构造函数
  NamedObject(const char* name, const T& value) { }
  NamedObject(const std::string& name, const T& value) { }

 private:
  std::string name_value_;
  T object_value_;
};

void Test() {

  NamedObject<int> no1("Smallest Prime Number", 2);
  NamedObject<int> no2(no1);
  no2 = no1;

  //operator= 与 copy构造函数 都是编译器合成， 内置类型的成员使用拷贝bit方式，非内置则调用 其定义的
  // operator= 与 copy构造函数从右侧操作数拷贝数据
}


}

/* 注意：
 * 1. 默认拷贝赋值运算符/拷贝构造函数在成员含引用类型时不能被生成
 * 2. 当基类将拷贝赋值运算符/拷贝构造函数声明为 private或 delete， 也是不能被生成的。
 */

namespace experiment3 {
template <class T>
class NamedObject {
 private:
  std::string& name_value_;
  const T& object_value_;
 public:
  NamedObject(std::string& name, const T& object_value);
};

void Test1() {
  std::string str1("ss");
  std::string str2("ss");
  NamedObject<int> no1(str1, 1);
  NamedObject<int> no2(str2,2);
 // no2 = no1;  // ？？ 引用是不可以更改绑定的,这时编译器拒绝生成 operator=，copy constructor
}



class Base {
 public:
  Base() = default;
 private:
  Base(const Base& rhs) = delete;
  Base& operator=(const Base& rhs) = delete;
};

class Derived: public Base {

};

void Test2() {
  Derived d1;
  //Derived d2 = d1;  //出错
}
}

/*总结：
 *  编译器可以暗自为class 创建default构造函数，copy构造函数
 *  copy assignment 操作符，以及析构函数。
 */