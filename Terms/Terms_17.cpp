// Authors: Handling
// Created by 15870 on 2020/11/25.
// This is 以独立语句将 newed 对象置入智能指针

//我们来演示一个复杂的错误
#include <memory>
#include <stdexcept>
namespace experiment {
class Widget { };
int priority() {
  throw std::runtime_error("");
}

void ProcessWidget(std::shared_ptr<Widget> widget_sptr, int priority) {

}

void Test() {
  ProcessWidget(std::shared_ptr<Widget>(new Widget), priority());
  //这里可能会出现内存泄漏

  //c++ 语言的函数传参调用顺序弹性很大
  //可能会出现调用顺序如下的表现:
  /* 1. new Widget
   * 2. priority()      //假若这里抛出了异常，就会有内存邪路问题
   * 3. shared_ptr<Widget>
   */
  //更好的办法
  std::shared_ptr<Widget> pw(new Widget());
  ProcessWidget(pw, priority());

}
}

/* 总结
 * 1. 尽量以独立的语句将 heap 资源置入 smart pointer
 *
 */


