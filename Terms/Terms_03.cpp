// Authors: Handling
// Created by 15870 on 2020/11/8.
// This is 尽可能使用 const

/* 一. const 是C++中的对于变量语义约束（不可修改），编译器会强制实行这项约束
 *     只要该值不可被改变（事实），就应该去进行约束
 */

#include <vector>
const int val1 = 5;
static int val2 = 5;
const int *pointer1 = &val1;  //底层 const  通过pointer1访问指向的元素时可读不可写
int* const pointer2 = &val2;  //上层 const  不可改变 pointer2 的指向



std::vector<int> vec;
const std::vector<int>::iterator  iter1 = vec.begin();      //上层const， iter的指向不能被改变
std::vector<int>::const_iterator iter2 = vec.begin();       //底层const， 通过iter访问元素时为可读不可写

// 二. const 最具威力的是面对函数声明时的应用， const可以与函数返回值，各参数，函数自身（成员函数）产生关联

//1. 令函数返回一个常量值，可以降低因客户操作而造成的（意外错误）
namespace example1 {
class Rational {
//一个“良好的用户自定义类型”的特征是他们避免无端地与内置类型不兼容
};

const Rational operator*(const Rational &lhs, const Rational &rhs) {
  return Rational();
}

void Test() {
  Rational a, b, c;
//  if(a*b = c) {}   //本身是 a*b == c, 写成了=，如果我们的operator*返回值是非const，就不会引起编译报错（但事实上代码是错误的）
}
}


//2. const 实施成员函数: 确认该成员函数可作用于 const 对象身上。
/* 第一：使得 class接口更加容易被理解，（得知那些函数可以改动对象内容那些不行）
 * 第二：使得操作 const 对象成为可能， （pass by reference-to-const 方式传递对象），我们有const成员函数处理const对象
 */

#include <string>
#include <cstring>

namespace example2 {

// 成员函数如果只是常量性不同，可以被重载
class TextBlock {
 private:
  std::string text_;
 public:
  const char &operator[](std::size_t position) const { return text_[position]; }
  char &operator[](std::size_t position) { return text_[position]; }
};

/*如何实施对对象进行const限制的措施：
 * 1. bitwise const阵营：不更改对象内的任何一个 bit （太过于强硬）
 * 2. bitwise constness阵营： 对对象的成员变量实施 const（编译器的做法）
 * 3. logical constness: 一个const成员函数可以修改它所处理的对象的某些 bits，但请确保客户端侦测不出
 *                       (实现办法是利用C++的一个与const相关的摆动场： mutable)
 */



class CTextBlock1 {
 private:
  char *text_;
 public:
  //常量成员函数，但返回值竟然是可以被修改 ？ 遵循第三种派别
  char &operator[](std::size_t position) const { return text_[position]; }
};

// 使用mutable 实现 logical constness，规定一些bit可以被修改

class CTextBlock2 {
 private:
  char *text_;
  mutable std::size_t text_length_;
  mutable bool length_isvaild_;
 public:
  std::size_t length() const;
};
std::size_t CTextBlock2::length() const {
  if(!length_isvaild_) {
    text_length_ = std::strlen(text_);
    length_isvaild_ = true;
  }
  return text_length_;
}

//在const 和 non-const 成员函数中避免重复（写出重复的代码）
//实现operator[]的机能一次并使用它两次，令其中一个调用另外一个，这促使 常量性 移除。
//非常量机能调用常量机能是合适的，因为常量中使用非常量代码是有歧义

class TextBlock3 {
 private:
  char *test_;
 public:
  const char& operator[](std::size_t position) const {
    return test_[position];
  }

  char& operator[](std::size_t position) {
    return const_cast<char&>(
    	    static_cast<const TextBlock3&>(*this)[position]
    	    );
  }

};


}



/* 总结：
 * 1. 将某些东西声明为const能帮助编译器去甄别错误用法
 * （作用域对象，函数参数，函数返回类型，成员函数本体）
 * 2. 编译器强制实施 bitwise constness,但我们编写程序时应该使用 “概念上的常量性”
 * 3. 当const 和 non-const成员函数有着实质等价的实现时，令 non-const 版本调用const版本可避免代码重复
 *
 */



