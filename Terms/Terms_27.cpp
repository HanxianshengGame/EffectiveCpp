// Authors: Handling
// Created by 15870 on 2020/12/1.
// This is 尽量少做转型动作

/* const_cast<T>()  //去除添加 const常量性（一般用于引用）
 * dynamic_cast<T>() //动态类型转换，将指向派生类对象的基类指针（引用）转换为派生类指针（引用）
 * reinterpret_cast<T>() //不可移植行为类似 c 语言转
 * static_cast<T>（） // 静态类型转换（隐式转换显示表现）
 */

class Base {
 public:
  virtual ~Base() { }
};
class Derived: public Base {
 public:
  virtual ~Derived() { }
};
#include <stdexcept>
#include <vector>
#include <memory>
void Test() {
  const int &a = 5;
  int b = const_cast<int&>(a);
  Derived derived;
  Base &base_ref = derived;
  try {
	auto &derived_ref = dynamic_cast<Derived &>(base_ref);
  }
  catch (std::bad_cast &e) {

  }

  int num = 0;
  int *int_ptr = &num;
  char *ch_ptr = reinterpret_cast<char *>(int_ptr);


  int num_int;
  double num_double = static_cast<double>(num_int);
}

// 实验1： 派生类的virtual 动作先调用 base 的对应函数，容易出现的错误
namespace experiment1 {
class Window {
 public:
  virtual void OnResize() {}

};

class SpecialWindow: public Window {
 public:
  virtual void OnResize() {
    static_cast<Window>(*this).OnResize(); //错误的使用，这里使用的是副本形式，不改变到自身
    Window::OnResize();  //正确的使用，使用的是本身的 基类部分，改变到自身
  }
};
}

//实验2： 更容易避免使用 dynamic_cast 的两种方法
// 普遍的实现版本基于 class 名称之字符串比较， 当深度继承时，其strcmp就会变多，因此注重效率的代码应该
// 对 dynamic_cast 保持机敏和猜疑

// 之所以使用 dynamic_cast, 通常是因为你想在你认定为 derived_class 对象身上执行 derived class 操作函数
// 但你手上仅有一个 指向 base 的 pointer 或 reference

//1. 直接使用容器存储 指向 derived class 对象的指针(比较不切实际)
//2. base class 提供 virtual 函数做你想对各个 Window 派生类做的事情，使用多态性质即可 （推荐）

namespace experiment2 {
class Window2 {
 public:
  virtual ~Window2() = default;

};
class SpecialWindow2: public Window2 {
 public:
  void Blink() { }
  ~SpecialWindow2() override = default;
};


void NativeFunciton() {
  typedef std::vector<std::shared_ptr<Window2>> VPM;
  VPM win_ptrs;
  for (auto & win_ptr : win_ptrs) {
   if(auto *sw_ptr = dynamic_cast<SpecialWindow2*>(win_ptr.get())) {
     sw_ptr->Blink();
   }
  }

}

void Function1() {
  typedef std::vector<std::shared_ptr<SpecialWindow2>> VPM;
  VPM sw_ptrs;
  for (auto & sw_ptr : sw_ptrs) {
	sw_ptr->Blink();
  }

}
class Window3 {
 public:
  virtual void Blink() { }
  virtual ~Window3() = default;
};

class SpecialWindow3: public Window3 {
 public:
  void Blink() override { }
  ~SpecialWindow3() override = default;
};


void Function2() {
  typedef std::vector<std::shared_ptr<Window3>> VPM;
  VPM win_ptrs;
  for (auto &win_ptr : win_ptrs) {
    win_ptr->Blink();
  }
}
}


/* 总结：
 * 1. 如果可以，请避免转型，特别是注重效率的代码中避免 dynamic_casts，如果有个
 * 设计需要转型动作，请试着发展无须转型的替代设计
 * 2. 如果转型是必要的，试着将它隐藏于某个函数背后， 客户随后可以调用该函数， 而不需要
 * 将转型放在他们的代码中， othertype ObjectToOther(object &obj)
 * 3. 宁可使用C++ style(新式)转型， 不要使用旧式转型， 前者很容易辨识出来，而且也有比较分门别类的职掌
 */