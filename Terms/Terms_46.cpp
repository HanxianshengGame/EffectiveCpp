// Authors: Handling
// Created by 15870 on 2020/12/10.
// This is 需要类型转换时请为模板定义非成员函数

/* 1. 第 24 条法则在模板中并不适用，（template 实参推导过程中不将隐式类型转换纳入考虑）
 * 2. 解决： 以template 相关的 ”函数支持所有参数之隐式类型转换“时，请将那些函数定义为 class template 内部的friend函数
 */


//1. 复现 24 （”唯有non-member函数才有能力在所有实参上实施隐式类型转换“）在template不适用

template <typename T>
class Rational {
 public:
  Rational(const T &numerator = 0,
		   const T &denominator = 1) {

  }

  const T numerator() const { }
  const T denominator() const { }
};

template <typename T>
Rational<T> operator*(const Rational<T> &lhs,
	                  const Rational<T> &rhs)
{

}

void Test() {
  Rational<int> one_half(1,2);
//  Rational<int> result = one_half * 1;      //失败，1隐式转换为 Rational<T>失败
}

// 原因： template 在实参推导过程中并不考虑”通过构造函数而发生的隐式类型转换“，因此 operator*在该情况下并不会被推导出


//2: 解决：在template class 中指涉 operator*函数为友元，由于类
//模板推导不依赖 tempalte 实参推导（施行于 function templates上），所以编译器总是能够在
// class Rational<T> 具现后找到 友元函数 operator*的声明 (并因此缓和 template 实参推导)
// 注意： friend 仅代表声明， 我们需要定义，可以在友元声明处直接定义，或定义额外的non-member供友元调用

/* 这里解释一下
 * 1. 为了让类型转换可能发生在所有实参上，我们需要一个 non-member 函数，为了让
 * 这个函数被自动具现化， 我们需要将它声明在 class 内部，
 * 而在 class 内部声明 non-member 的唯一方法是友元。
 */

template <typename T>
class Rational2;

template <typename T>
Rational2<T> doMultiply(const Rational2<T> &lhs,
						const Rational2<T> &rhs) {

}

template <typename T>
class Rational2 {
 public:

    friend Rational2 operator*(const Rational2 &lhs,
                               const Rational2 &rhs) {
      doMultiply(lhs,rhs); //operator* 暴露给了doMultiply 的所需东西
    }


  Rational2(const T &numerator = 0,
		   const T &denominator = 1) {

  }

  const T numerator() const { }
  const T denominator() const { }
};

template <typename T>
Rational2<T> operator*(const Rational2<T> &lhs,
					  const Rational2<T> &rhs)
{

}

void Test2() {
  Rational2<int> one_half(1,2);
  Rational2<int> result = one_half * 1;      //失败，1隐式转换为 Rational<T>失败
}

int main() {
  Test2();
}


/* 1. 当我们编写一个 class Template， 而它所提供之 ”与此template相关的“ 函数支持 ”所有参数之隐式类型转换“ 时
 * 请将那些函数定义为 ”class template“ 内部的friend 函数。
 */