// Authors: Handling
// Created by 15870 on 2020/11/8.
// This is 尽量以 const， enum， inline 替换 #define
#include <algorithm>
#include <string>
#define ASPECT_RATIO 1.653
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))

/*  ASPECT_RATIO 从未被编译器看到，而是被预处理器展开， 可能出现的问题：
 *
 * 1. 运用该宏定义的常量时出现了编译错误信息时，错误提示提到的是 1.653 而非常量名，排查错误难
 * 2. 普通的变量会被编译器看到，会记录到记号表中（symbol table） ，而宏定义的常量不会被记录，导致出现
 *    常量值的目标码（object code），从而可能造成代码膨胀（代码膨胀会导致额外的换页行为，降低指令高速缓存装置的击中率，带来效率损失）。
 * 3. 函数宏虽不会有函数调用的栈方面的开销，但是缺点很明显，难读，容易出错
 */

//一. 以 const 替换 宏定义常量
namespace constreplace {

//1. 定义普通常量
//使用原生指针定义常量字符串时，要记得使用双重const
const char *const author_name = "handling";
//标准库字符串常量更好的写法
const std::string author_name2("handling");

//2. 定义 class专属常量
// 为了将常量的作用域限制在类中，将常量变为其成员，为了至多一份实体，需要让其成为static成员。

class GamePlayer {
 public:
  static const int kNumTurns = 5; //常量声明式,且已经赋予初值，定义时不能再次设初值
  int scores_[kNumTurns];
};

//静态非常量需要及时在实现文件中加入定义式， 静态常量只要不对其取其地址，我们可以声明而不实现其（应该是做了处理）
const int GamePlayer::kNumTurns;

}

//二. 以enum hack 替换 宏定义常量

namespace enumreplace {


class GamePlayer {
 private:
// 适用于 编译器不支持 static常量完成在类内部的初值设定（声明式）
  enum { NUM_TRUNS = 5 };       //the enum hack 令NUM_TRUNS 成为了5的一个记号名称，不能取地址
                                //1. 类似 define，但有记号表记录，没有类似define常量的缺点
                                //2. 有更多的约束，可以拒绝其他pointer或reference指向该常量
                                //3. 实用主义， enum hack 是模板元编程的基础技术
  int scores_[NUM_TRUNS];

};
}

//三. 以 inline 替换 宏定义函数

namespace inlinereplace {

//1. 使用inline 可以获取到 宏带来的效率以及一般函数可预料行为与类型安全检查
//2. 遵循作用域与访问权限规则，可以在类内写出 inline

template <class T>
inline void CallWithMax(const T& a, const T& b) {
  int max_val = std::max(a,b);
}
}

/*总结：
 * 1. 对于单纯常量， 最好以 const 对象或 enums 替换 #defines
 * 2. 对于形似函数的宏， 最好改用 inline函数替换 #define
 */

int main() {
  using constreplace::GamePlayer;
  using namespace inlinereplace;
  CallWithMax(1,2);
}