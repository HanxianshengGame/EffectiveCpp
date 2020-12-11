// Authors: Handling
// Created by 15870 on 2020/12/9.
// This is 了解隐式接口与编译器多态


#include <vector>
class Widget {
 public:

  Widget() =default;
  virtual ~Widget() = default;
  virtual std::size_t size() const;
  virtual void normalize();
  void swap(Widget &other);

  bool operator!=(const Widget &rhs) {
	return false;
  }
};


std::size_t Widget::size() const {

}

void Widget::normalize() {

}

void Widget::swap(Widget &other) {

}

Widget some_widget;
template <typename T>
void doProcessing(T &w) {
  if(w.size() > 10 && w != some_widget) {
    T temp(w);
    temp.normalize();
    temp.swap(w);
  }
}

int main() {
  Widget w;
  doProcessing(w);
}

/* 总结：
 * 1. classes 和 templates 都支持接口和多态
 * 2. class 而言接口都是显示的，以函数签名为中心，多态则是通过virtual函数发生在运行期
 * 3. 对template参数而言，接口是隐式的，基于有效表达式，多态是通过template具现化和函数重载解析
 * 发生于编译器。
 */