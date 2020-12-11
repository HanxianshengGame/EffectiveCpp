// Authors: Handling
// Created by 15870 on 2020/11/29.
// This is 考虑写出一个不抛出异常的swap函数


//1. 实现一个 pimpl 手法的类, 并写出其 特例化的swap 函数
#include <iostream>
#include <vector>
namespace experiment1 {

class WidgetImpl {
 public:
 private:
  int data1_;
  int data2_;
  std::vector<int> v;
};

class Widget {
 public:

  void swap(Widget &other) noexcept {
    using std::swap;
    swap(impl_ptr_, other.impl_ptr_);
  }
 private:
  WidgetImpl *impl_ptr_;
};
}

using experiment1::Widget;
namespace std {
template <>
void swap<Widget>(Widget &lhs, Widget &rhs) noexcept {
  lhs.swap(rhs); //调用了该
  std::cout << "特化" << std::endl;
}
}

int main() {
  Widget lhs, rhs;
  using std::swap;
  swap(lhs, rhs);
}

/* 1. 提供一个 public swap 成员函数，让它高效地置换你的类型的两个对象值，这个函数不能抛出异常
 * 2. 在你的 class 或 template 所在的命名空间内提供一个 non-member swap，并令它调用上述 swap 成员函数
 * 3. 如果你正编写一个 class， 为你的class 特化 std::swap ，并令它调用你的 swap 成员函数
 * 4. 最后调用 swap 时，确保包含一个 using 声明符，以便让 std::swap 在你的函数曝光
 * 然后不加任何 namespace 修饰符， 赤裸裸地调用 swap。
 * 5. 注意：不能抛出异常，因为swap是帮助 class 提供异常安全性的保障，  基于的条件就是 swap不能抛出异常
 */