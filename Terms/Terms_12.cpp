// Authors: Handling
// Created by 15870 on 2020/11/25.
// This is 复制对象时勿忘其每一个成分

// 如果拒绝编译器自动生成copying函数，如果你的代码不完全，他们也不告诉你！！！

// 实验一： 局部拷贝的错误


#include <string>
namespace experiment1 {
void LogCall(const std::string &func_name) {

}

class Date {};
class Customer {
 public:
  Customer(const Customer &rhs);
  Customer &operator=(const Customer &rhs);
  //...

 private:
  std::string name_;
  Date last_transaction_;
};

//这里如果没有复制新添加的变量，编译器也是不会进行提醒的，因为你已经拒绝编译器进行为你生成 copy 函数

Customer::Customer(const Customer &rhs)
	: name_(rhs.name_) {
  LogCall("Customer copy constructor");
}

Customer &Customer::operator=(const Customer &rhs) {
  LogCall("Customer copy assignment operator");
  name_ = rhs.name_;
  return *this;
}

//这里如果你需要修改 class 中的成员，你同时也需要修改其 两个 copy 函数




}

// 实验二： 继承拷贝的错误
// 继承一个基类后进行编写派生类的 copy 函数时应注意编译器不会进行对 派生类的基类部分自动拷贝

namespace experiment2 {
class PriorityCustomer : public experiment1::Customer {
 public:
  //...
  PriorityCustomer(const PriorityCustomer &rhs);
  PriorityCustomer &operator=(const PriorityCustomer &rhs);
  //...

 private:
  void Init() {
    //编写 copy 两个函数中重复的部分
  }
  int priority_;
};
PriorityCustomer::PriorityCustomer(const PriorityCustomer &rhs)
: experiment1::Customer(rhs),
  priority_(rhs.priority_)
{
  experiment1::LogCall("PriorityCustomer copy constructor");
}
PriorityCustomer &PriorityCustomer::operator=(const PriorityCustomer &rhs) {
  experiment1::LogCall("PriorityCustomer copy assignment operator");
  if(this != &rhs) {
    PriorityCustomer temp(rhs);
    std::swap(*this, temp);

//    experiment1::Customer::operator=(rhs);
//    priority_ = rhs.priority_;
  }
  return *this;
}

}

/* 总结：
 * 1. copying 函数应该确保复制 ”对象内的所有成员变量及所有 base class 成分“
 * 2. 不要尝试以某个 copying 函数实现另一个 copying函数，应该将共同机能放进第三个函数中
 * 并由两个 copying 函数共同调用
 */
