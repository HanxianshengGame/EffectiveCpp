// Authors: Handling
// Created by 15870 on 2020/11/13.
// This is 为多态基类声明 virtual 析构函数
#include <iostream>
/*一：问题浮现： 销毁一个heap分配的基类指针（指向的是派生类）内存泄漏问题
 */

namespace experiment1 {
class TimeKeeper1 {
 public:
  TimeKeeper1() = default;
  ~TimeKeeper1() = default;
  //...
};

class AtomicClock : public TimeKeeper1 {

};

class WaterClock : public TimeKeeper1 {

};
class WristWatch : public TimeKeeper1 {

};

TimeKeeper1 *GetTimerKeeper() {
  //工厂模式，返回一个派生类指针
  TimeKeeper1 *clock = new WaterClock();
  return clock;
}
void Test1() {
  TimeKeeper1 *ptk = GetTimerKeeper();
  delete ptk;    //会出现内存泄漏问题

  /* 原因： 通过GetTimeKeeper 返回的指针是一个基类指针，销毁基类指针则会取基类的部分（调用基类的析构函数）
   * 官方： C++明白指出，当derived class对象经由一个base class指针被删除，而其base class带一个 non-virtual函数
   * 其结果就是未定义-实际执行下来发生的就是对象的 derived 成分没被销毁
   *  解决： 给base class 设置一个 virtual 析构函数即可
   */

  class TimeKeeper2 {
   public:
	TimeKeeper2() = default;
	virtual ~TimeKeeper2() = default;
	//...
  };

}
}

/* 二： 验证： 任何 class 带有virtual函数都几乎确定应该有一个 virtual 析构函数,
 *            没有理由地把所有 class 的析构函数设置为 virtual的行为是错误的。
*/

namespace experiment2 {
// 设置虚函数的目的是为了能实现多态，因此可以肯定有派生类的存在。

//如下所示，我们随意在一个class将其析构函数设置为 virtual，
/*virtual的声明会使得 Point的结构膨胀，安插指向虚函数表的 vptr
 * （32bit计算机体系Point空间为 64bits，因为 vptr的存在变为 96bits）
 */
class Point {
 private:
  int x_;
  int y_;
 public:
  Point(int xcoord, int ycoord) {

  }
  virtual ~Point() {

  }
};

}


/* 三： 利用析构函数实现抽象类*/

namespace experiment3 {
class AWOV {
 public:
  virtual ~AWOV() = 0;     //当一个类中无其余函数时，将析构函数定义为pure virtual是一个方法
};

AWOV::~AWOV() { } //需要定义，不然会 未定义的链接错误

class B: public AWOV {       //这里注意，在基类与派生类转换时（派生类的继承说明符需要是 public）
 public:
  ~B() override {
    std::cout << "~B()" << std::endl;
  }
};
}
int main() {
  using namespace experiment3;
  AWOV* b = new B();
  delete b;
}

/*总结：
 * 1. polymorphic base classes 应该声明一个virtual析构函数， 如果 class 带有任何 virtual 函数，
 * 他就应该拥有一个 virtual 析构函数
 *
 * 2. Class 的设计目的如果不是当作 base classes 使用，就不应该声明 virtual 析构函数
 */
