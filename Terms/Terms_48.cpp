// Authors: Handling
// Created by 15870 on 2020/12/11.
// This is 认识 template 元编程



/* 1. 了解什么是 template 元编程（TMP template metaprograming）
 * 2. 实现一个 template 简单例子
 *
 */

//1.

/* 简介： TMP 是编写 模板程序 并执行于编译期的过程（也可以是说执行于C++编译器内的程序）
 * 优点：执行与编译期，检测错误更早，程序更高效，较小的执行文件，较短的运行期，较少的内存需求
 * 缺点：编译时间变长
 */

#include <iterator>
template <typename IterT, typename DistT>
void advance1(IterT &iter, DistT d) {
  if(typeid(typename std::iterator_traits<IterT>::iterator_category)
     == typeid(std::random_access_iterator_tag)) {
//    iter += d;
  }
}
#include <list>
void Test1() {
  std::list<int>::iterator iter;
  advance1(iter,10);  //隐式接口不符合，错误
}

//2. TMP 实现计算阶乘

template <unsigned n>
struct Factorial {
    enum { value = n * Factorial<n-1>::value };
  };

template <>
struct Factorial<1> {
  enum { value = 1 };
};
#include <iostream>
void Test2() {
  std::cout << Factorial<10>::value
            << std::endl;
}
int main() {
  Test2();

}


/* 总结：
 * 1. TMP 可将工作由运行期移到编译期，因而得以实现早期错误侦测和更高的执行效率
 * 2. TMP 被用来 生成 ”基于政策选择组合“ 的客户定制代码，也可以避免生成对某些特殊类型
 * 并不适合的代码:
 */