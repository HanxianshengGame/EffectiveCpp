// Authors: Handling
// Created by 15870 on 2020/12/7.
// This is 区别接口继承和实现继承

// public 继承观念由两部分组成： 函数接口继承和函数实现继承

#include <string>
#include <iostream>
class Shape {
 public:
  virtual void draw() const = 0;  //pure virtual函数的目的是为了让 derived class 只继承函数接口
  virtual void error(const std::string& msg);  //impure virtual 函数的目的是让 derived class 继承该函数的接口和缺省实现
  int objectID() const; //non-virtual函数的目的是为了令derived class 继承函数的接口及一份强制性实现（不变性，不应该被重新定义）

 private:
  int object_id_{};
};

// 我们可以为 pure virtual 提供一个定义，C++并不会发出怨言，但调用其途径仅 “调用时指定其class名称”
void Shape::draw() const {
  std::cout << "  void Shape::draw() const " << std::endl;
}
void Shape::error(const std::string &msg) {

}
int Shape::objectID() const {
  return object_id_;
}

class Rectangle: public Shape {
  //...
  void draw() const override;

};
void Rectangle::draw() const {
  std::cout << "  void Rectangle::draw() const " << std::endl;
}

//重要说明：pure virtual 函数必须在 derived class 重新声明，但也可以有自己的一份实现，需要显式调用而已
// 适用于 接口与缺省情况并存的情况下 （比 impure class 好用）


int main() {
  Shape *ps = new Rectangle();
  ps->draw();  //调用 rectangle  版本
  ps->Shape::draw();   //调用 Shape 版本
}


/* 总结：
 * 1. 接口继承和实现继承不同，在public 继承下， derived classes总是继承 base class 的接口
 * 2. pure class 函数只具体指定接口继承
 * 3. 简朴的 impure virtual 函数具体指定接口继承与缺省实现继承
 * 4. non-virtual 函数具体指定接口继承以及强制性实现继承
 * 5. 一个典型的程序有 80% 的时间花费在 20% 的代码上，请将心里放在那举足轻重的代码上。
 *
 */