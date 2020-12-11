// Authors: Handling
// Created by 15870 on 2020/12/11.
// This is 了解 new-handler 的行为


/* 1. 认识 new-handler 和 set_new_handler，并懂得设计一个 new-handler
 * 2. 定制 class 专属的 new-handler
 * 3. nothrow 这个发育不良的朋友
 */


//一.

/* new-handler: 当operator new 抛出异常以反映一个未获满足的内存需求之前，它会现调用一个客户
 * 指定的错误处理函数，一个所谓的 new-handler
 *
 * set_new_handler: 参数是指针，用于传入指定的 new-handler函数，返回值是（ 被替换的那个 new-handler)
 *
 */
//std下的标准库函数声明，模拟

namespace mystd {
typedef void (*new_handler)();
new_handler set_new_handler(new_handler p) noexcept; //noexcept： 表示不会抛出异常
}


// 使用 set_new_handler
#include <iostream>
void outOfMemory() {
  std::cerr << "Unable to satisfy request for memory\n";
  std::abort();
}

void Test1() {
  std::set_new_handler(outOfMemory);
  int *p = new int[10000000000];
  delete[] p;
}

// 当operator new无法满足内存申请时，它会不断调用 new-handler 函数，直到找到足够内存
// 那就是一个设计良好的 new-handler 函数必须做以下事情：

/* 1. 让更多内存被使用： 如果operator new 失败，下一次的分配动作可能成功
 * 策略： 程序一开始就分配一大块内存，而后当 new-handler 第一次被调用，将它们释放给程序使用
 *
 * 2. 安装另一个new-handler， 如果目前这个new-handler 并不能满足，让它有能力知道另外的 new-handler可以分配
 * 策略： 令new-handler 修改 “会影响 new-handler行为” 的static数据，namespace数据或 global 数据
 *
 * 3. 卸除 new-handler： 将 null 指针传给 set_new_handler ，一旦没有安装任何任何的 new-handler
 * operator new 会在内存分配不成功时抛出异常
 *
 * 4. 抛出 bad_alloc(或派生自 bad_alloc) 的异常， 这样的异常不会被operator new 捕捉，因此
 * 会被传播到内存索求处。
 *
 * 5. 不返回，通常调用abort 或 exit
 */


//二. 根据每个 class 不同定制不同的内存分配失败情况

/* 1. 首先需要 class 提供自己的 set_new_handler 和 operator new
 *  set_new_handler 使得客户得以指定 class 专属的 new-handler
 *  operator new 确保在分配 class 对象内存的过程中以 class 专属之 new-handler 替换 global new-handler
 *
 */

#include <new>
namespace custommemorymanagement {
class Widget {
 private:
  static std::new_handler current_handler_;
 public:
  static std::new_handler set_new_handler(std::new_handler p) noexcept;
  static void* operator new(std::size_t size) noexcept(false) ;
};

std::new_handler Widget::current_handler_ = nullptr;

/* set_new_handler 任务
 * 1. 设置新 new-handler
 * 2. 返回旧 old-handler
 */

std::new_handler Widget::set_new_handler(std::new_handler p) noexcept {
  std::new_handler old_handler = current_handler_;
  current_handler_ = p;
  return old_handler;
}

/* operator new 任务
 * 1.（更换错误处理函数） 调用标准 set_new_handler 设置自身保存的 current_handler，
 * 2. (执行内存分配) 分配失败则 global operator new 会调用 current_handler
 * 3. 其中 new-handler 在以异常的方式处理或退出operator new时，应注意将 class 中的 current_handler 回复到第1步设置之前
 * (保证不影响接下来的内存分配动作)
 *
 * 注意： 第3点使用RAII处理更好
 */

class Uncopyable {
 protected:
  Uncopyable() = default;
  ~Uncopyable() = default;

 private:

  Uncopyable(const Uncopyable& ) = delete;
  Uncopyable& operator=(const Uncopyable& ) = delete;
};


class NewHandlerHolder: private Uncopyable {
 public:
  explicit NewHandlerHolder(std::new_handler handler)
  : handler_(handler)
  { }

  ~NewHandlerHolder()
  {
    std::set_new_handler(handler_);
  }

 private:
  std::new_handler handler_;
};
void *Widget::operator new(std::size_t size) noexcept(false) {
  NewHandlerHolder holder(std::set_new_handler(current_handler_));
  return ::operator new(size);
}


void Test1() {
  Widget::set_new_handler(outOfMemory);
  auto *pw1 = new Widget(); //分配失败调用 outOfMemory
  auto *ps = new std::string();

  delete ps;
  delete pw1;
}

//由于该动作并不因 class 的不同而不同， 将 Widget 的 operator new 与
// set_new_handler 动作进行复用是必要的

template <typename T>
class NewHandlerSupport {
 public:
  static std::new_handler set_new_handler(std::new_handler p) noexcept;
  static void* operator new(std::size_t size) noexcept(false);

 private:
  static std::new_handler current_handler_;
};

template <typename T>
std::new_handler NewHandlerSupport<T>::current_handler_ = nullptr;
template<typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) noexcept {
  std::new_handler old_handler = current_handler_;
  current_handler_ = p;
  return old_handler;
}
template<typename T>
void *NewHandlerSupport<T>::operator new(std::size_t size) noexcept(false) {
  NewHandlerHolder h(std::set_new_handler(current_handler_));
  return ::operator new(size);
}


class NewClass: public NewHandlerSupport<NewClass> {

  };


void Test2() {
  NewClass::set_new_handler(outOfMemory);
  NewClass *p = new NewClass();
}
}


//三. nothrow 形式

void Test3() {
  using namespace custommemorymanagement;
  std::string *pw1 = new (std::nothrow) std::string(); //如果分配失败返回nullptr
  if(pw1 == nullptr) {

  }
}


int main() {
  Test1();
}

/* 总结：
 * 1. set_new_handler 允许客户指定一个函数，在内存分配无法获得满足条件时被调用
 * 2. Nothrow new 是一个颇为局限的工具，因为它只适用于内存分配，后继的构造函数调用
 * 还是可能抛出异常。
 *
 */



