// Authors: Handling
// Created by 15870 on 2020/11/25.
// This is 在operator= 中处理自赋值。

/* 自赋值是十分没有必要的，但也是代码漏点很多的一个问题
 * 1. 类中有heap资源， 编写代码时需要注意 异常安全性
 * 2. 自赋值的避免
 */
#include <vector>
namespace experiment1 {
class Widget {
 public:
  void swap(Widget& rhs);
  Widget(const Widget& rhs) = default;
  Widget& operator=(const Widget& rhs);
};
Widget& Widget::operator=(const Widget &rhs) {
  Widget temp(rhs);
  swap(temp);
  return *this;
}
void Widget::swap(Widget &rhs) {
  if(this != &rhs) {
    std::swap(rhs, *this);
  }
}
}

/* 总结：
 * 确保当对象自我赋值时 operator= 有良好的行为（鲁棒性，通用性更强），其中技术包括
 * 1. 来源对象和目标对象的地址
 * 2. 精心周到的语句顺序
 * 3. copy and swap
 *
 * 确保任何函数如果操作一个以上的对象，且多个对象都是同一个对象时，其行为仍然正确
 */

