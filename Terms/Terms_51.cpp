// Authors: Handling
// Created by 15870 on 2020/12/11.
// This is 编写 new 和 delete 时需固守常规

/* 1. 编写  operator new 需注意的规矩
 * 2. 编写  operator delete 需注意的规矩
 */

//1.

/* 1. 内含一个无限循环， 并在其中尝试分配内存，无法满足时调用 new-handler
 * 2. 处理 0 bytes 申请
 * 3. Class专属版本应该处理 ”比正确大小的（错误）“ 申请
 */

// operator new（伪码）
#include <vector>
#include <cstdlib>

//void *operator new(std::size_t size) noexcept(false) {
//  using namespace std;
//  if(size == 0)
//    size = 1;               //空类的对象内存为1
//
//  while (true) {
//    //尝试分配 size bytes;
//    void *heap_memory = malloc(size);
//    if(heap_memory)             //分配成功返回
//	  return heap_memory;
//    new_handler global_handler = set_new_handler(nullptr);
//    set_new_handler(global_handler);
//
//    if(global_handler)         //分配失败调用 new-handler
//	  global_handler();
//    else
//	  throw std::bad_alloc();
//  }
//}

class Base {
 public:
  static void* operator new(std::size_t size) noexcept(false);
  static void operator delete(void *raw_memory, std::size_t size) noexcept;
};

class Derived: public Base {

};

void *Base::operator new(std::size_t size) noexcept(false) {
  using namespace std;
  if(size != sizeof(Base))
	return ::operator new(size);     //因为多态的存在导致 size 可能与 sizeof(Base)不同，以原生new执行即可

    while (true) {
    //尝试分配 size bytes;
    void *heap_memory = malloc(size);
    if(heap_memory)             //分配成功返回
	  return heap_memory;
    new_handler global_handler = set_new_handler(nullptr);
    set_new_handler(global_handler);

    if(global_handler)         //分配失败调用 new-handler
	  global_handler();
    else
	  throw std::bad_alloc();
  }
}

//2.

/* 1. C++保证删除 null 指针永远安全
 * 2. Class专属版本应该处理 ”比正确大小的（错误）“ 申请
 */

//void operator delete(void *raw_memory) noexcept {
//  if(raw_memory == 0)
//	return;
//
//  free(raw_memory);
//}


void Base::operator delete(void *raw_memory, std::size_t size) noexcept {
  if(raw_memory == nullptr)
	return;
  if(size != sizeof(Base)) {
    ::operator delete(raw_memory);
	return;
  }

  free(raw_memory);
}


/* 总结：
 * 1. operator new 应该内含一个无穷循环， 并在其中尝试分配内存，如果它无法满足内存需要
 * 就应该调用 new-handler，它也应该有能力处理 0 bytes申请， Class 专属版本则还应该处理
 * ”比正确大小更大的（错误）申请“
 * 2. operator delete应该在收到 null 指针时不做任何事， Class 专属版本则还应该处理
 *  ”比正确大小更大的“ （错误） 申请。
 */

