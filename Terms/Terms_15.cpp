// Authors: Handling
// Created by 15870 on 2020/11/25.
// This is 在资源管理类中提供对原始资源的访问


// 提供原始资源的访问以方便客户
/* 例如： shared_ptr 的get获取原始指针
 * 1. 显式返回原始指针
 * 2. 隐式直接进行转换（在类中定义 转换资源类型的 运算符）
 */
#include <iostream>
#include <pthread.h>
#include <memory>

void Lock(pthread_mutex_t *mutex) {
  pthread_mutex_lock(mutex);
}
void Unlock(pthread_mutex_t *mutex) {
  pthread_mutex_unlock(mutex);
}
namespace experiment {
class Uncopyable {
 protected:
  Uncopyable() = default;
  ~Uncopyable() = default;
 private:
  Uncopyable(const Uncopyable &rhs);
  Uncopyable& operator=(const Uncopyable &rhs);
};


class MutexLock: private Uncopyable {
 private:
  pthread_mutex_t *mutex_ptr_;
 public:
  explicit MutexLock(pthread_mutex_t *mutex)
	  : mutex_ptr_(mutex)
  {
	Lock(mutex);
	std::cout << "MutexLock Lock!" << std::endl;
  }
  //显式
  pthread_mutex_t* GetMutexPtr() { return mutex_ptr_; }
  //隐式
  using MutexPtr = pthread_mutex_t*;
  operator MutexPtr() const {
    return mutex_ptr_;
  }

  ~MutexLock() {
	Unlock(mutex_ptr_);
	std::cout << "MutexLock Unlock!" << std::endl;
  }
};

pthread_mutex_t mutex;
void Test() {
  MutexLock mutex_lock(&mutex);
  pthread_mutex_t *mutex_ptr = mutex_lock; //隐式使用
}

}

/* 总结
 * 1. APIs 往往要求访问原始资源，所以每个 RAII class 应该提供一个
 * ”取得其所管理之资源“ 的办法
 * 2. 对原始资源的访问可能经由显示转换或隐式转换。一般而言显示转换比较安全，
 * 但隐式转换对客户比较方便。
 */
