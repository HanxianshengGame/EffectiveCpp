// Authors: Handling
// Created by 15870 on 2020/12/9.
// This is 明智而审慎地使用 private 继承


#include <iostream>
/* 理解private 继承所能实现的效果
 * 1. 将 base class 中的所有成员变为 private 属性
 * 2. 编译器不会自动将一个derived class 对象转换为一个 base class 对象
 * 3. private 继承意味着 根据某物实现出，而非 is-a 语义，只有实现部分被继承，接口不会被继承
 */

class Person {

};

class Student: private Person {
  //...
};

void eat(const Person &p) {

}

void Test() {
  Student student;
//  eat(student);     //私有继承 不能转换
}

class Timer {
 public:
  explicit Timer(int tickFrequency) { }
  virtual void onTick() const = 0;
};

class Widget: private Timer { //Widget 是想使用Timer而非 is-a 语义
 private:
  virtual void onTick() const {

  }
};

//使用 public 继承加复合 比 private继承 更好
//1. 阻止了派生类重新定义virtual的要求
//2. 将Widget的编译依存性降至最低
class WidgetComposition {
 private:
  class WidgetTimer: public Timer {
   public:
    void onTick() const override {

    }
  };

  WidgetTimer timer_;
};

// private 继承主要用于 “当一个意欲” 成为 derived class 者想访问一个
// 意欲成为 base class 者的protected成分，或为了重新定义一或多个 virtual 函数

// 空间最优化会促使你选择 private 继承而非 “继承加复合”
// 空类的sizeof为1，c++会为其安插一个char到空对象中，使得空对象间有一定的区分
// EBO:空间基类最优化，将空类（未含成员的类）空间在继承后优化掉

class Empty {};
class HoldsAnInet {
 private:
  int x_;
  Empty e_;
};

class HoldsAnInetEBO: private Empty {
 private:
  int x_;
};


int main() {
  std::cout << "继承+复合：" << sizeof(HoldsAnInet)
            << "私有继承： " << sizeof(HoldsAnInetEBO)
            << std::endl;
  // 输出  8    4
}


// 考虑过所有方案后，仍认为 private 继承是“表现程序内两个 classes 之间的关系”的最佳办法，才使用

/* 总结：
 * 1. private 继承意味着 （根据某物实现出）的语义， 比复合的级别低，但是当
 * derived class 需要访问 protected base class 的成员，或者重新定义继承而来的 virtual 函数时，是合理的
 *
 * 2. 和复用不同，private继承可用造成 empty base 最优化， 这对致力于 “对象尺寸最小化” 的程序开发者来说很重要。
 */



