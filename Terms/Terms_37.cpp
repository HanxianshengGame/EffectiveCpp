// Authors: Handling
// Created by 15870 on 2020/12/8.
// This is 绝不重新定义继承而来的缺省参数值

//由于派生类永远不会重新定义继承来的 non-virtual 函数，所以我们这条是针对 virtual 而言的
#include <iostream>
class Shape {
 public:
  enum ShapeColor { Red, Green, Blue };
  virtual void draw(ShapeColor color = Red) const = 0;
};

class Rectangle: public Shape {
 public:
  void draw(ShapeColor color = Green) const override {
	switch (color) {
	  case Red:
	    std::cout << "Red" << std::endl;
	    break;
	  case Green:
	    std::cout << "Green" << std::endl;
	    break;
	  case Blue:
	    std::cout << "Blue" << std::endl;
	    break;
	}
  }

};

#include <memory>
using namespace std;
void Test() {
  unique_ptr<Shape> shape_sptr(new Rectangle());
  shape_sptr->draw();  //你不敢相信输出的竟然是 shape 的draw 的参数缺省值， 并没有触发动态绑定？？？

  //原因是因为： 缺省参数值执行的是静态绑定， 而不是运行期再次确定，缺省参数遵循调用者的静态类型而定
  // shape_sptr 的静态类型是 shape 的智能指针， 因此缺省值是 shape的draw的缺省值，而非 rectangle 的draw的缺省值
 // C++ 坚持以这种夸张的方式来运作是基于运行期效率来做的。

}


//使用 NVI手法替换虚函数所表现的行为不是很满意的情况
// base 的 public non-virtual 函数调用 private virtual， virtual 可被重新定义
// 我们让 non-virtual 函数指定缺省参数， private virtual 负责真正的工作

class ShapeNVI {
 public:
  enum shapeColor { RED, GREEN, BLUE };
  void draw(shapeColor color = RED) const {
    doDraw(color);
  }
 private:
  virtual void doDraw(shapeColor color) const = 0;
};

class RectangleNVI: public ShapeNVI {
 public:

 private:
  void doDraw(shapeColor color) const override {
   std::cout << " void doDraw(shapeColor color) const override  " << std::endl;
  }
};

int main() {
  Test();
}


/* 总结：
 * 1. 绝对不要重新定义一个继承而来的缺省参数值，因为缺省参数都是静态绑定，
 * 而virtual函数---你唯一应该覆写的东西-却是动态绑定。
 */
