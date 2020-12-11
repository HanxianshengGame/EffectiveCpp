// Authors: Handling
// Created by 15870 on 2020/12/10.
// This is 运用成员函数模板接受所有兼容类型



/* 1. 同一个template的不同具现体之间不存在固有关系，需要定义泛化的隐式转换
 * 2. 泛化的copy与assignment函数，并不会阻止编译器生成他们自己的 copy与assignment函数
 */

//1. 了解 template 不同具现体之间的转换与 模板类型之间的转换并不兼容
#include <iostream>
class Top {
 public:
  virtual ~Top()  = default;
};
class Middle: public Top {

};
class Bottom: public Middle { };

template <typename T>
class SmartPtr {
 public:
  explicit SmartPtr(T *real_ptr)
  : real_ptr_(real_ptr)
  { }

  ~SmartPtr() {
	delete real_ptr_;
  }
 private:
  T *real_ptr_;
};

void Test1() {
  Top *pt1 = new Middle();
  Top *pt2 = new Bottom();
  const Top *pct2 = pt1;

  delete pt1;
  delete pt2;

//  SmartPtr<Top> pt3 = SmartPtr<Middle>(new Middle());  //错误
}

//写出一个泛化copy构造函数来兼容，限制工作交给实际的类型去转换


template <typename T>
class SmartPtr2 {
 public:
  explicit SmartPtr2(T *real_ptr)
	  : real_ptr_(real_ptr)
  { }

  //泛化模板函数，如果 rhs 的内部指针能转换为 real_ptr_，则编译通过，限制工作交付给内置类型
  template<typename U>
  SmartPtr2(const SmartPtr2<U> &rhs)
  : real_ptr_(rhs.get())
  { }

  T* get() const { return real_ptr_; }

  ~SmartPtr2() {
	delete real_ptr_;
  }
 private:
  T *real_ptr_;
};

void Test2() {
  SmartPtr2<Top> ptr = SmartPtr2<Middle>(new Middle());

}




//2. 泛化的 copy 构造函数并不会阻止编译器生成它们自己的 copy 构造函数，
// 如果你想完完全全控制 copy动作，请写出泛化版本与非泛化版本

int main() {
  Test1();
}

/* 总结：
 * 1. 请使用 member function templates（成员函数模板）生成 ”可接受所有兼容类型“ 的函数
 * 2.  泛化的 copy 构造函数并不会阻止编译器生成它们自己的 copy 构造函数，
 * 如果你想完完全全控制 copy动作，请写出泛化版本与非泛化版本
 */