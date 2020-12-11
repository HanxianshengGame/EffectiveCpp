// Authors: Handling
// Created by 15870 on 2020/11/16.
// This is 绝不在构造和析构过程种调用 virtual 函数
#include <iostream>

/* 实验一： 证明在 base class 构造期间， virtual 函数不是 virtual 函数。*/

namespace experiment1 {
class Transaction {
 public:
  Transaction();
  virtual void logTransaction() const { std::cout << "transaction" << std::endl; }
};
Transaction::Transaction() {
  logTransaction();
}

class BuyTransaction: public Transaction {
 public:
  virtual void logTransaction() const override { std::cout << "BugTransaction" << std::endl; }
};
class SellTransaction: public Transaction {
 public:
  virtual void logTransaction() const override { std::cout << "SellTransaction" << std::endl;}
};

void Test() {
  BuyTransaction b;  //调用的是base class 的虚函数 logTransaction
  // 原因1： 如果在构造base class时调用的是 derived class 的函数（会使用到derived class成员，
  // 但此时成员都是未构造的，会出现问题）
  // 原因2： 在derived class对象的 base class构造期间，对象本身是base class而不是 derived class
  // 不止virtual函数会被编译器解析至 base class，运行期类型信息，也会把对象视为 base class

  //相对应析构函数执行到base class部分，编译器也会视当前对象为 base class
}

}

/* 实验二：如何确保每一次有 Transaction继承体系上的对象被创建，就会有适当版本的 logTransaction被调用 */

/* 由于你无法在 base class构造时期通过 virtual函数 调用到 derived class 的函数，
 * 因此可以使用 非virtual 通过在 derived class构造函数传（必要参数）传递到 base class 的构造函数，
 * 进而调用 base class 的通过必要参数而实行的普通函数
 */

#include <string>
namespace experiment2 {
class Transaction {
 public:
  explicit Transaction(const std::string& str);
  void logTransaction(const std::string& str);
};
Transaction::Transaction(const std::string &str) {
  logTransaction(str);
}
void Transaction::logTransaction(const std::string &str) {
  //操作
}

class SellTransaction: Transaction {
 public:
  explicit SellTransaction(const std::string& str);

 private:
  //一个亮点， 比起在初始化列表种给与 base class 所需数据， 利用辅助函数创建一个值传给 base class
  //构造函数往往比较方便（也比较可读），令此函数为static， 也就不可能意外地指向“初期 SellTranscation未
  //构造完成的成员“
  static std::string CreateString();
};

SellTransaction::SellTransaction(const std::string &str)
: Transaction(str) {


}
std::string SellTransaction::CreateString() {
  return std::__cxx11::string();
}
}
int main() {
  using namespace experiment1;
  Test();

}

/* 总结
 * 1. 在base class 构造和析构期间不要调用 virtual 函数，因为这类调用从不下降至 derived class
 *  (比起当前执行构造函数和析构函数那层)，virtual本质上并没有用
 */