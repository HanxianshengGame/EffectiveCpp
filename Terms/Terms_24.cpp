// Authors: Handling
// Created by 15870 on 2020/11/29.
// This is  若所有参数皆需类型转换，请为此采用 non-member 函数

namespace experiment3 {
class Rational {
 public:
  Rational(int numerator = 0, int denominator = 1)
  : n_(numerator),
    d_(denominator)
  { }

  int numerator() const;
  int denominator() const;
  const Rational operator*(const Rational &rhs) const;
 private:
  int n_;
  int d_;
};
//const Rational operator*(const Rational &lhs, const Rational &rhs) {
//
//}
void Test() {
  Rational one_half(1,2);
  Rational result = one_half * 2; //2转化为了 rational
  result = 2 * one_half; //报错

  //2 * one_half     2并不在Rational.operator*(rhs) 的参数列中，会报错
  //one_half * 2     2 在 one_half.operator*(rhs) 的参数列中，会报错

  //解决办法： 将 operator* 变为非成员函数

}
}

/* 总结
 * 1. 若重载运算符函数的操作数皆需类型转换，就声明其为非成员函数
 */

