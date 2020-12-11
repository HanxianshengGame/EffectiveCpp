// Authors: Handling
// Created by 15870 on 2020/11/25.
// This is 成对使用 new 和 delete 时要采取相同形式

/* 1. new 对应 delete， new[] 对应 delete[]
 * 2. 尽量少使用数组，多使用 vector string 标准库
 */


#include <string>
void Test() {
  auto str_ptr = new std::string();
  delete str_ptr;

  auto str_arr_ptr = new std::string[10]();
  delete[] str_arr_ptr;
}
