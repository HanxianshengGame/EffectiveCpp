// Authors: Handling
// Created by 15870 on 2020/11/12.
// This is  确定对象在使用前已经被初始化

// 1. 内置类型需要在使用之前初始化
// 2. 对象类型，初始化责任落在构造函数中

/*  实验一： C++的对象成员变量的初始化发生在进入构造函数本体之前  */
#include <string>
#include <list>
#include <utility>
#include <vector>
#include <cstring>
namespace experiment1 {


class PhoneNumber {

};

class ABEntry {
 private:
  std::string the_name_;
  std::string the_address_;
  std::vector<PhoneNumber> the_phones_;
  int num_times_consulted_;

 public:

  ABEntry(const std::string &name, const std::string &address,
		  const std::vector<PhoneNumber> &phones);
  ABEntry(std::string name, std::string address,
		  const std::vector<PhoneNumber> &phones, ...);
};
ABEntry::ABEntry(const std::string &name, const std::string &address, const std::vector<PhoneNumber> &phones) {
  the_name_ = name;
  the_address_ = address;
  the_phones_ = phones;
  num_times_consulted_ = 0;
}


/* 如上代码会对成员先进行默认构造函数的调用，之后在进行赋值。
 * 解决办法是： 使用 member initalization list 初始化列表替换赋值动作
 *             为了规范统一：将全部成员（无物也要使用初始化列表初始化）

 */

ABEntry::ABEntry(std::string name, std::string address, const std::vector<PhoneNumber> &phones, ...)
	: the_name_(std::move(name)),
	  the_address_(std::move(address)),
	  the_phones_(),
	  num_times_consulted_(0)  //内置类型，需要进行手动赋予初始值
{ }


}



/* C++类对象的初始化次序： base class 总是早于其 derived class 被初始化， class 的成员变量总是以其声明次序被初始化*/
/* 实验二：C++对于定义在不同编译单元内的non-local static 对象的初始化相对次序无明确定义。*/
namespace experiment2 {
class FileSystem {
 public:
  std::size_t num_disks() const { return 1; }
};

extern FileSystem tfs;

class Directory {
 public:
  Directory() {
    std::size_t disks = tfs.num_disks(); //这里假若tfs没有被初始化，发生的事情就无法预料
  }
};

//解决办法就是使用 Singleton 使得 non-local static 搬到自己的专属函数中
//C++保证， 函数内的 local static对象会在 “首次遇上该对象之定义上” 被初始化。
//多线程环境下执行顺序的麻烦性： 尽量在单线程运行期按一定顺序初始化这些 static
FileSystem& FileSystemSingleton() {
  static FileSystem tfs;
  return tfs;
}
}


/* 总结：
 * 1. 为内置型对象进行手工初始化，因为 C++不保证初始化他们
 * 2. 构造函数最好使用成员初始值，而不要在构造函数本体内使用赋值操作， 且初始
 * 列列出的成员变量，其排序次序应该和他们在 class 中声明次序相同
 * 3. 为免除“跨编译单元之初始化次序”问题， 请以 local static 对象替换 non-static 对象
 */

int main() {
  using experiment2::Directory;

  Directory temp_dir;
}