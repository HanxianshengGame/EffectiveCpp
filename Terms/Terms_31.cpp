// Authors: Handling
// Created by 15870 on 2020/12/1.
// This is 将文件间的编译依存关系降至最低



//实验1： 相依赖于声明式，而不要相依定义式实现一个类，遵循以下
//1. 如果能够使用 object references 或者 object pointers可以完成任务，就不要使用 objects
//   使用 objects 就需要定义该类型的定义式，在声明一个类时需要知道其sizeof大小
//2. 如果能够，尽量以 class 声明式替换 class 定义式

#include <string>
namespace experiment1 {
class Date30;
Date30 Today();
void ClearAppointments(Date30 d);  // 函数声明时参数返回值可以不用知道其定义式，但是在定义时必须可知

class Date30 {

};
Date30 Today() {

}


class DateTest;
class AddressTest;
class PersonTest {
 private:
  std::string the_name_;
  DateTest *the_birth_date_;
  AddressTest *the_address_;
 public:
  PersonTest(const std::string &name, const DateTest &birthday,
			 const AddressTest &addr);
  std::string name() const;
  std::string birth_date() const;
  std::string address() const;
};
}


/* 总结：
 * 1. 支持“编译依存性最小化”的一般构想是： 相依于声明式， 不要相依定义式
 * 基于此构想两个手段是 Handle classes（将自身的实现作为另一个类，且将该类的指针作为自己的实现） 和 Interface classes
 * （以抽象类作为接口）
 * 2. 程序库头文件应该以 “完全且仅有声明式” 的形式存在。
 */
