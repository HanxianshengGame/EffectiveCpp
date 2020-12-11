// Authors: Handling
// Created by 15870 on 2020/12/11.
// This is 请使用 traits classes 表现类型信息

/* 1. 迭代器类型的区分与traits（类型萃取技术）的实现
 * 2. 使用重载，使得 traits classes 在编译期对类型执行 if。。。else 测试
 * 3. 总结如何使用一个 traits class
 */

//1.

#include <iterator>
#include <iostream>

void Test1() {
  using namespace std;
  input_iterator_tag iter1;      //只写  （只能向前移动，且所指物都只能操作一次）
  output_iterator_tag iter2;     //只读   （只能向前移动，且所指物都只能操作一次）

  forward_iterator_tag iter3;   //前向   继承 input_iterator_tag ,能操作多次
  bidirectional_iterator_tag iter4; //双向 继承 forward_iterator_tag 能操作多次
  random_access_iterator_tag iter5; //范围双向  继承 bindirectional_iterator_tag  能操作多次
}


// STL 中 通过萃取容器得到其 iterator 类型之后才能实施不同的算法
/* 萃取技术的实现：
 * 1. 确定若干你想将来可取得的类型相关信息
 * 2. 为该信息选择一个名称
 * 3. 提供一个 template 和一组 特化版本（有需要的话），内含你希望支持的类型信息
 */


struct attack_tag { };
struct defer_tag { };

class AttackCharacter {
 public:
  using category = attack_tag; //类别为 attack_tag
};

class DeferCharacter {
 public:
  using category = defer_tag; // 类别为 defer_tag
};

template <typename T>
struct CharacterTraits {
  using category = typename T::category;
};

class SpecialCharacter {

};

template <>
struct CharacterTraits<SpecialCharacter> {
  using category = attack_tag;
};


void Test2() {
  if(typeid(typename CharacterTraits<SpecialCharacter>::category) ==
     typeid(typename CharacterTraits<AttackCharacter>::category)) {
    std::cout << "category is same" <<std::endl;
  }
}



//2. 模仿 STL 中的做法，使用重载解决 CharacterTraits<SpecialCharacter>::category 在编译阶段能完成
// 但由于if 的原因却是推迟到运行期核定的问题 得到解决

// 当你重载某个函数 f，编译器会根据传来的实参选择最合适的重载函数

void f(attack_tag tag) {

}
void f(defer_tag tag) {

}

template <typename Character>
void func() {
  f(typename CharacterTraits<Character>::category());
}

// public 继承遵循 is-a 的语法，因此derived class 在base class 的方法上也能行的通
int main() {
  Test2();
}


//3.

/* 如何正确使用一个 traits class
 * 1. 建立一组重载函数（劳工）或函数模板，彼此之间的差异仅在各自的 traits 参数，令每个函数实现码与
 * 其接收之 traits 信息相应和
 *
 * 2. 建立一个控制函数（身份像工头）或函数模板（advance），它调用上述那些 ”劳工函数“并传递 traits class
 * 所需信息
 */



/* 总结：
 * 1. Traits classes 使得 ”类型相关信息“ 在编译期可用， 他们以 templates 和 ”templates特化“ 完成实现
 * 2. 整合重载技术后， traits classes 有可能在编译期对类型执行 if...else 测试
 */