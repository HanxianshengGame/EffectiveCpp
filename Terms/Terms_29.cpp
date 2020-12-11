// Authors: Handling
// Created by 15870 on 2020/12/1.
// This is 为 “异常安全” 而努力是值得的

/* 异常抛出时，带有异常安全性的函数会
 * 1. 不泄露任何资源     2. 不允许数据败坏
 */

/* 异常安全性提供以下三个保证之一：
 * 1. 基本承诺：如果异常被抛出，程序内的任何事物仍然保持在有效状态下。
 * 没有任何对象或数据结构因此而败坏，所有对象都处于一种内部前后一致的状态
 *
 * 2. 强烈保证：如果异常被抛出， 程序状态不改变。
 * 调用可能抛出异常的操作，如果调用失败应该恢复到调用之前的状态，调用成功就是完成成功
 *
 * 3. 不抛出异常保证： 承诺绝不抛出异常， 因为他们总是能完成它们原先承诺的功能。
 *（作用于内置类型）  nothrow
 *
 */


//实验1： 使用 copy and swap 实现强烈保证级别的异常安全性。
#include <memory>
#include <pthread.h>

namespace experiment1 {
class Image {

};
struct PMImpl {
  std::shared_ptr<Image> bg_image_sptr_; //背景，类型 smart point
  int image_change_;
};

void Lock(pthread_mutex_t *mutex) {
  pthread_mutex_lock(mutex);
}
void UnLock(pthread_mutex_t *mutex) {
  pthread_mutex_unlock(mutex);
}

class MutexLock29 {
 private:
  pthread_mutex_t *mutex_;
 public:
  explicit MutexLock29(pthread_mutex_t *mutex)
  : mutex_(mutex) {
    Lock(mutex_);
  }
  ~MutexLock29() {
    UnLock(mutex_);
  }
};

class PrettyMenu {
  void ChangeBackGround(std::istream &img_stream);
 private:
  pthread_mutex_t mutex_;
  std::shared_ptr<PMImpl> impl_sptr_;
};

void PrettyMenu::ChangeBackGround(std::istream &img_stream) {
  using std::swap;
  MutexLock29 mutex_lock(&mutex_);
  std::shared_ptr<PMImpl> new_sptr(new PMImpl(*impl_sptr_));
  new_sptr->bg_image_sptr_.reset(new Image());
  ++new_sptr->image_change_;

  swap(impl_sptr_, new_sptr);

}
}


/* 总结
 * 1. 异常安全函数即使发生异常也不会泄漏资源或允许
 * 任何数据结构败坏，这样的函数区分为三种可能的保证： 基本型，强烈型，不抛异常型。
 * 2. 强烈保证往往能通过 copy and swap 实现出来， 但 “强烈保证” 并非对所有函数都可实现或具备现实意义
 * 3. 函数提供的 “异常安全保证” 通常最高值等于其小函数中的最弱的异常安全性级别。
 *
 */