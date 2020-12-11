// Authors: Handling
// Created by 15870 on 2020/12/7.
// This is 避免遮掩继承而来的名称

class Base {
 private:
  int x_;
 public:
  virtual void mf1() = 0;
  virtual void mf1(int) {

  }
  virtual void mf2() {

  }
  void mf3() {

  }
  void mf3(double) {

  }

};

class Derived: public Base {
 public:
  using Base::mf3;   //让Base class内命名为 mf1 和 mf3 的所有东西在
                     // Derived 作用域都可见
  using Base::mf1;
  virtual void mf1() {
    Base::mf3(1.0);  //调用被隐藏的 mf3

  }
  void mf3() { mf3(1); }
  void mf4() { }

};

/* 总结
 * 1. derived class 内的名称会遮盖 base classes 内的名称， 在public 继承下从来没有人希望如此
 * 2. 为了让遮盖的名称再见天日，可使用using声明式子，或转交函数  类名::function
 *
 */

