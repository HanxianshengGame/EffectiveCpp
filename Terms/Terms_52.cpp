// Authors: Handling
// Created by 15870 on 2020/12/11.
// This is 写了 placement new 也要写 placement delete


/* 1. 关于使用 new 因构造函数抛出异常出现的内存泄漏问题，
 * placement new 要与 placement delete 对应 才能使得运行期系统寻找到 处理异常导致内存邪路问题
 *
 * 2. 解决派生类隐藏 Base 的 placement new 与 placement delete版本
 */

//1

/*  问题：
 *  new 一个对象有两部，一部是分配内存，一步调用对象的构造函数
 *  如果内存分配成功，但构造函数抛出异常，内存会交付给运行期系统处理，但需要对应的 delete 版本
 *  如果使用了 placement new ，但未定义对应的 placement delete 就会出现问题
 *
 *  解决： 定义参数个数与类型相同的 placement new 与 placement delete
 */

#include <vector>
class Widget {
  static void* operator new(std::size_t size, void *placement_ptr);
  static void operator delete(void *memory_ptr, void *placement_ptr);
};

void Test1() {

}


//2.

/* Base 声明的 placement new 与 placement delete 会被 Derived 定义的隐藏掉
 * 标准形式的 operator new 与 operator new 也会因为 Class 声明而被隐藏
 *
 * 解决： 将正常形式的 new和delete 全部放在 一个 Base class里
 * Derived class 声明 placement new 与 delete， 且使用using声明式子将 Base class 中的
 * operator new 和 delete 暴露
 */

class StandardNewDeleteForms {
 public:
  static void* operator new(std::size_t size) noexcept(false) {
	return ::operator new(size);
  }
  static void operator delete(void *memory_ptr) noexcept {
    ::operator delete (memory_ptr);
  }
  static void* operator new(std::size_t size, void *placement_ptr) noexcept {
	return ::operator new(size,placement_ptr);
  }
  static void operator delete(void *memory_ptr, void *placement_ptr) noexcept {
	::operator delete (memory_ptr,placement_ptr);
  }
  static void* operator new(std::size_t size, const std::nothrow_t &nt) noexcept {
	return ::operator new(size,nt);
  }
  static void operator delete(void *memory_ptr,const std::nothrow_t &nt) noexcept {
	::operator delete (memory_ptr,nt);
  }

};

class Derived: public StandardNewDeleteForms {
 public:
  using StandardNewDeleteForms::operator new;
  using StandardNewDeleteForms::operator delete;
};

int main() {

}

/* 总结：
 * 1. 当你写一个 placement operator new， 请确定也写出了对应的 placement
 * operator delete， 如果没有这样做，你的程序可能会发生隐微而时断时续的内存泄漏
 *
 * 2. 当你声明了 placement new 与 delete， 请确定别无意识第遮掩了 他们的正常版本
 */