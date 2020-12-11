// Authors: Handling
// Created by 15870 on 2020/11/25.
// This is 在资源管理类中小心 copying 行为


/* 资源的类型并非都是 heap+based 资源上，你需要建立自己的资源管理类
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

class MutexLock {
 private:
  pthread_mutex_t *mutex_ptr_;
 public:
  explicit MutexLock(pthread_mutex_t *mutex)
  : mutex_ptr_(mutex)
  {
    Lock(mutex);
    std::cout << "MutexLock Lock!" << std::endl;
  }

  ~MutexLock() {
    Unlock(mutex_ptr_);
	std::cout << "MutexLock Unlock!" << std::endl;
  }
};

pthread_mutex_t mutex;
void Test() {
  pthread_mutex_init(&mutex, nullptr);
  MutexLock lock(&mutex);

//  MutexLock lock2(lock);  //会发生什么事情, 答案就是重复加锁，死锁
}

/* 面对资源 copy 的动作有如下解决方案
 * 1. 禁止拷贝
 * 2. 对底层资源使用 RCSP ，引用计数法 （使用 shared_ptr）
 * 3. 深拷贝复制底部资源
 * 4. 转移底部资源的拥有权
 */

namespace experiment1 {
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

  ~MutexLock() {
	Unlock(mutex_ptr_);
	std::cout << "MutexLock Unlock!" << std::endl;
  }
};


}
namespace experiment2 {
//将mutex的底层资源交付给 shared_ptr 管理即可，并传递给其删除器
class MutexLock {
 private:
  std::shared_ptr<pthread_mutex_t> mutex_ptr_;
 public:
  explicit MutexLock(pthread_mutex_t *pm)
  : mutex_ptr_(pm, Unlock)
  {
    Lock(mutex_ptr_.get());
  }
};
}
namespace experiment3 {
//直接 bit copy
}

namespace experiment4 {
  //将原资源交付给现管理，并将原资源指针置空
}

int main() {
  Test();
}

/* 总结
 * 1. 复制 RAII 对象必须一并复制它所管理的资源，资源的copying行为决定RAII对象的copying行为
 * 2. 普遍而常见的 RAII class copying行为是： 抑制copying，施行引用计数
 */