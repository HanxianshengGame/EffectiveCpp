// Authors: Handling
// Created by 15870 on 2020/11/25.
// This is 令operator= 返回一个 reference to *this


//这份协议可以说是为了实现连锁赋值而 创造的协议

namespace experiment1 {
void Test() {
  int x, y, z;
  x = y = z = 15; // 连锁赋值，被STL容器都遵守的协议，我们就随大众把

}

class Widget {
 public:
  Widget& operator=(const Widget& rhs) {
    //...
    return *this;
  }

  Widget& operator+=(const Widget& rhs) {
    //...
	return *this;
  }

};

void Test2() {
  Widget w1;
  Widget w2 = w1;
}
}

//总结：连锁赋值几乎被所有内置类型和标准库程序提供的类型， string，vector都遵守，因此
//我们自定义的也最好共同遵守