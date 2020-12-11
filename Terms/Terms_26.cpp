// Authors: Handling
// Created by 15870 on 2020/11/29.
// This is 尽可能延后变量定义式的出现时间

/* 尽可能延后的真正意义
 * 1： 不仅只是延后变量的定义，直到非带使用该变量的前一刻为止，甚至
 * 应该尝试延后这份定义直到能够给它初值实参为止。
 * 2. 不仅能够避免构造（析构）非必要对象，还能避免无意义的default的构造行为。
 */

//实验1： 尽可能延后能优化代码
#include <iostream>
#include <string>
namespace experiment1 {
std::string encryptPassword(const std::string &password) {
  using namespace std;
  string encrypted;
  if(password.length() < 1) {
	throw logic_error("Password is too short"); //这里如果抛出异常，则 encrypted 就过早定义
  }

  //...  必要动作，将加密后的密码返回 encrypted
  //...
  return encrypted;
}

// 修改上述

std::string encryptPassword2(const std::string &password) {
  using namespace std;

  if(password.length() < 1) {
	throw logic_error("Password is too short"); //这里如果抛出异常，则 encrypted 就过早定义
  }

  // 得到加密密码
  //返回前进行构造，高效的做法
  string encrypted("密码");
  return encrypted;
}


}

//实验2：循环中所使用变量需不需要延后，还是提前定义？

namespace experiment2 {
//方法A
void FunctionA() {
  using namespace std;
  string tmp_str;
  for (int i = 0; i != 100; ++i) {
    tmp_str = "111";
  }
}

void FunctionB() {
  using namespace std;
  for(int i = 0; i != 100; ++i) {
    string tmp_str(" ");
  }
}

/* FunctionA 的变量是  一个构造函数 + n个拷贝赋值运算符 + 1 个析构函数
 * FunctionB 的变量是  n 个构造函数 + n个析构函数
 *
 * 做法A造成名称 tmp_str 的作用域比做法B 更大，
 * 除非你知道赋值成本比 “构造和析构” 成本低， 或者你对效率特别敏感，否则你应该使用做法B。
 */
}

/* 总结
 * 尽可能延后变量定义的出现，这样做可以增加程序的清晰度并改善程序效率。
 */