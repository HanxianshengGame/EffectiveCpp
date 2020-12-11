// Authors: Handling
// Created by 15870 on 2020/12/10.
// This is 学习处理模板化基类内的名称

/* 1. 模板特例化并介绍模板继承出现的问题（编译器并不会去基类搜寻接口）
 * 2. template base class members 无效调用的解决
 */

#include <string>
class CompanyA {
 public:
  void sendCleartext(const std::string &msg) { }
  void sendEncrypted(const std::string &msg) { }

};

class CompanyB {
 public:
  void sendCleartext(const std::string &msg) { }
  void sendEncrypted(const std::string &msg) { }
};

class MsgInfo { };

template <typename Company>
class MsgSender {
 public:
  void sendClear(const MsgInfo &info) {
    std::string msg;
    Company c;
    c.sendClearText(msg);
  }
  void sendSecret(const MsgInfo &info) {

  }
};

//如果 Company 没有 sendClearText，就会使得调用失败
//1. 使用模板特例化完成特殊的MsgSender（使Company具现化在定义时）
class CompanyZ {
 public:
  void sendEncrypted(const std::string &msg) { }
  //没有sendClearText
};



//特例化的版本不能有 sendClearText
template <>
class MsgSender<CompanyZ> {
 public:
  void sendSecret(const std::string &info) { }
};




//2. C++ 模板继承时由于 base class 并没有具现化，C++并不知道继承的究竟是什么类，因此其内含的 members全部被隐藏
// 因此继承而来的 sendClearMsg 会被隐藏


template <typename Company>
class LoggingMsgSender: public MsgSender<Company> {
 public:
  using MsgSender<Company>::sendClear;     //使用using暴露 base 中的 member
  void sendClearMsg(const MsgInfo &info) {

    //发送前的信息写到 log
    this->sendClear(info);         //使用this-> 指涉其base class的member名称
    //发送后的信息写到 log
  }
};

/* 总结：
 * 1. 对于特别的模板参数类型可采用模板特例化完成
 * 2. 可在派生template类内通过 this-> 或 using 暴露其 base class template 成员名称
 */




int main() {

}