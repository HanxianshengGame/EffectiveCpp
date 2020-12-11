// Authors: Handling
// Created by 15870 on 2020/11/27.
// This is 让接口容易被正确使用，不容易被误用

//理想上，如果客户企图使用某个接口而却没有获得他预期的行为，这个代码不能通过编译

// 实验一：

/* 1. 明智而审视地导入新类型对于预防“接口被误用” 有神奇疗效
 * 2. 以函数替换对象，预先定义有效的 对象使得接口更具备安全性
 */

namespace experiment1 {
struct Day {
  int val_;
  explicit Day(int d)
  : val_(d)
  { }
};
struct Month {
  int val_;
  explicit Month(int m)
	  : val_(m)
  { }
};

struct Year {
  int val_;
  explicit Year(int y)
  : val_(y)
  { }
};

class Date {
 private:
  Month month_;
  Day day_;
  Year year_;
 public:
  Date(const Month &m, const Day &d, const Year &y)
  : month_(m),
    day_(d),
    year_(y)
    {  }

};

#include <memory>
void Test() {
  Date date1(Month(1), Day(1), Year(2020));  //预防客户参数输入混乱的好办法，显示设置其参数
  std::shared_ptr<>
}

class Month2 {
 private:
  explicit Month2(int m)
  : val_(m)
  { }

  int val_;

 public:
   //使用该方法能避免输入参数的越界等错误问题
  static Month May() {
	return Month(5);
  }

};
}


//实验二：
/*
 * 1. 除非有非常好的理由，否则应令 type 与 内置 type 提供一致的行为接口。
 */

/* 总结
 *  1. 好的接口很容易被正常使用，不容易被误用
 *  2. “促进正确使用”的办法包括接口的一致性， 以及与内置类型的行为兼容
 *  3. “阻止误用” 的办法包括建立新类型， 限制类型上的操作，束缚对象值， 以及消除客户的资源管理责任
 *  4. shared_ptr 支持定制型删除其，可防范在不同DLL间出现的引用计数的问题，可被用来自动解除互斥锁等等资源问题
 *
 */
