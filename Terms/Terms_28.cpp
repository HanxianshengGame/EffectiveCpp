// Authors: Handling
// Created by 15870 on 2020/12/1.
// This is 避免返回 handles 指向对象内部成分

//handles(指针，引用，迭代器)

//实验1： 探索返回对象内部成分带来的弊端
//1. 破坏封装性
//2. 导致空悬指针等情况
#include <memory>
namespace experiment1 {
class Point {
 private:
  int x_;
  int y_;
 public:
  Point(int x, int y)
  : x_(x),
    y_(y)
  { }

  void set_x(int new_val) { x_ = new_val; }
  void set_y(int new_val) { y_ = new_val; }
};
struct RectData {
  Point ul_point_;
  Point lr_point_;
};

class Rectangle {
 private:
  std::shared_ptr<RectData> data_sptr_;
 public:
  Point& upper_left() const { data_sptr_->ul_point_; }
  const Point& upper_right() const { data_sptr_->lr_point_; }

  //取点导致了外部可修改
  //且导致了空悬指针的产生
};

void Test() {
  const Point *ptr = &(Rectangle().upper_right());
  const Point *ptr2 = &(Rectangle().upper_left()); //不报错，这样会导致出现风险“内部对象比对象本身更长寿”

  const Rectangle *ptr3 = &Rectangle(); //报错
}

}


/* 总结
 * 避免返回 handles(ref， 指针，迭代器)指向对象内部
 * 增加封装性，且帮助 const 成员函数像一个 const，减少 空悬 的发生
 */

