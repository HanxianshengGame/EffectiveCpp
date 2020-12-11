// Authors: Handling
// Created by 15870 on 2020/12/9.
// This is 了解 typename 的双重意义

/* 1. typename 定义模板类型的基本用法解析
 * 2. typename 用于指涉嵌套丛属类型名称
 */


//1. typename 定义模板类型
template <class T>
class Widget;

template <typename T>
class Widget;

// 这里 typename 定义模板类型无区别， 一般情况下如果模板类型可容纳（类类型与基本类型时用typename）



//2. 从属名称与 typename 指涉嵌套从属类型名称
#include <iostream>
template <typename C>
void print2nd(const C &container) {
  if(container.size() >= 2) {

	//指涉从属类型名称时，在前加上typename，因为C++也可能认为 const_iterator 是一个全局变量或静态类型
	//C++解决这种歧义的做法就是解析器在 template 遭遇从属类型名称时，便认为这个名称并非是一个类型。
	typename C::const_iterator iter(container.begin());   //C::iterator 是一个嵌套丛书类型名称（依赖于模板参数C，且是内嵌在C的类型）
     ++iter;

     int value = *iter;
     std::cout << value;
  }
}


//嵌套从属类型名称不需指定 typename 的情况

template <typename T>
class Base {
	 public:
	  class Nested {
	    explicit Nested(int x) {

	    }

};
};


template <typename T>
 class Derived: public Base<T>::Nested {      //继承列无须指定 typename
  public:
   explicit Derived(int x)
   : Base<T>::Nested(x) {             //初始化列表也无需指定

     typename Base<T>::Nested temp;
   }

 };


// 对内嵌从属类型进行 起别名

template <typename IterT>
void workWithIterator(IterT iter) {
  typename std::iterator_traits<IterT>::value_type temp(*iter);

  using value_type = typename std::iterator_traits<IterT>::value_type;
  value_type temp2(*iter);
}


/* 总结：
 * 1. 声明template参数时， 前缀关键字 class 与 typename 可互换
 * 2. 请使用关键字 typename 标识嵌套从属类型名称， 但不得在 base class lists（基类列）
 * 或 member initialization list（成员初始值列）内以它作为 base class 修饰符。
 *
 */