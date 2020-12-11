// Authors: Handling
// Created by 15870 on 2020/12/7.
// This is 确定你的public继承塑模出 is-a 关系


//C++进行面向对象编程最重要的规则是：public inheritance 意味着是 is-a 的关系（正向一类）
// ia-a： D 继承 B，则每一个类型为D的对象本身也是类型为B的对象，反则不成立

#include <cassert>
class Rectangle {
 public:
  virtual void setHeight(int new_height);
  virtual void setWidth(int new_width);
  virtual int height() const;
  virtual int width() const;
 private:
  int height_;
  int width_;
};
//使得长方形变大
void makeBigger(Rectangle &rectangle) {
  int old_height = rectangle.height();
  rectangle.setWidth(rectangle.width() + 10);
  assert(rectangle.height() == old_height);
}

class Square: public Rectangle {

};


void Test() {
  Square square;
  square.setWidth(10);
  square.setHeight(10);

  makeBigger(square); //???
  assert(square.width() == square.height()); //错误， 继承的体系并未使得在makeBigger的函数中Square的效果正确
}

/* public 继承意味着 is-a，适用于 base class 的事情也一定适用于 derived class身上
 * 未遵循is-a的代码即使编译通过，但不保证程序的行为是正确的
 */