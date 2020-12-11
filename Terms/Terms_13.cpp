// Authors: Handling
// Created by 15870 on 2020/11/25
// This is 以对象进行管理资源

//基于对象的资源管理办法，是十分有效的

//实验一： 体验基于对象资源管理的方法
#include <iostream>
#include <memory>
namespace experiment1 {
class Investment {
  //...
};

Investment* CreateInvestment() {
  return new Investment();
}

void Test() {
  //这里我们使用智能指针进行演示
  std::shared_ptr<Investment> ptr_inv(CreateInvestment());
}

/*  自行编写的写法
 *  1. 获得资源后立即放入管理对象内， ”以资源取得时机便是初始化时机“ （RAII）
 *  2. 管理对象运用析构函数确保资源被释放。 （不论控制流如何，只要对象被销毁，析构函数必会调用）
 *  在析构函数中有异常，请遵循条款8
 */

class InvestmentManager {
 public:
  explicit InvestmentManager(Investment *ptr_inv)
  : ptr_inv_(ptr_inv),
    freed_(false)
  { }

  void Free() {
	delete ptr_inv_;
	freed_ = true;
  }
  ~InvestmentManager() {
	if (!freed_) {
	  try {
		delete ptr_inv_;
		freed_ = true;
		std::cout << "RAII" << std::endl;
	  }
	  catch (...) {

	  }
	}
  }
 private:
  Investment *ptr_inv_;
  bool freed_;
};

void Test2() {
  InvestmentManager manager(CreateInvestment());
  //manager.Free();
}
}

int main() {
  using namespace experiment1;
  Test2();

}


/* 总结：
 * 1. 为了防止资源泄漏，请使用 RAII对象， 他们在构造函数中获得资源并在析构函数中释放资源
 * 2. 两个常被使用的 RAII 对象 classes 分别是 std::shared_ptr 和 std::unique_ptr
 * shared_ptr 是一个 RCSP(reference count smart pointer)
 */




