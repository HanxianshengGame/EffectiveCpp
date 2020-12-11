// Authors: Handling
// Created by 15870 on 2020/11/29.
// This is 将成员变量声明为 private


/* 探讨内容
 * 1. 如何对成员变量进行有效的控制
 */

namespace experiment1 {
class Accesslevels {
 public:
  int read_only() const { return read_only_; }
  void set_write_only(int tmp) { write_only_ = tmp; }

  int read_write() const { return read_write_; }
  void set_read_write(int tmp) { read_write_ = tmp; }
 private:
  int no_access_;    //无任何访问权限
  int read_only_;   //仅读
  int read_write_;  //读写

  int write_only_; //仅写
};
}

/* 总结：
 * 1. 切记将成员变量设置为 private，这可赋予客户访问数据的一致性，可细微划分访问控制，允许约束条件
 * 得到保证，并提供 class 作者以充分的实现弹性。
 *
 * 2. 仅存在两种访问权限： private（提供封装）和 其他（不提供封装），protected 并不比 public 更具封装性
 *
 */
