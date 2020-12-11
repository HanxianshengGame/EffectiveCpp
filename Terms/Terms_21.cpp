// Authors: Handling
// Created by 15870 on 2020/11/29.
// This is 必须返回对象时，别妄想返回其 reference

/* 探索内容
 *  1. 拒绝返回局部作用域的局部变量的引用，那其实指向了一片不存在的区域
 */
#include <iostream>
namespace experiment1 {
class Rational {
  friend const Rational operator*(const Rational &lhs, const Rational &rhs);
 private:
  int n_;
  int d_;
 public:
  Rational(int numerator = 0, int denominator = 1)
  : n_(numerator),
    d_(denominator)
    { }
};
inline const Rational operator*(const Rational &lhs, const Rational &rhs) {
  return Rational(lhs.n_ * rhs.n_, lhs.d_ * rhs.d_);
}

}


/* 总结：
 * 1. 绝对不要返回 pointer 或 reference 指向的一个 local stack 对象，或者返回引用指向一个
 * heap-allocated 对象，或返回 pointer 或 reference 指向一个 local static对象而有可能同时
 * 需要多个这样的对象
 */
