// Authors: Handling
// Created by 15870 on 2020/12/9.
// This is 通过复合塑模出 has-a 或 “根据某物实现出”

//复合包括 关联，聚合，组合  （A has B）

//is-a ： 是继承，   has-a： 是复合    挑选合适的关系帮我们完成更好的面向对象设计

//做一个必须以复合完成的功能实例: is-a 意味着 派生类必须能作为基类 完成基类能完成的所有功能

#include <list>
#include <algorithm>
template <class T>
class MySet {
 public:
  bool member(const T &item) const;
  void insert(const T &item);
  void remove(const T &item);
  std::size_t size() const;
 private:
  std::list<T> rep_; //为什么是 has-a，而不是 is-a，很重要的区别就是set插入的元素不能是重复的
};
template<class T>
bool MySet<T>::member(const  T &item) const {
  return std::find(rep_.cbegin(), rep_.cend(), item) != rep_.cend();
}
template<class T>
void MySet<T>::insert(const T &item) {
  if(!member(item))
    rep_.push_back(item);
}
template<class T>
void MySet<T>::remove(const T &item) {
  auto iter = std::find(rep_.begin(), rep_.end(), item);
  if(iter != rep_.end())
    rep_.erase(iter);
}
template<class T>
std::size_t MySet<T>::size() const {
  return rep_.size();
}

void Test() {
  MySet<int> my_set;
  my_set.insert(1);
  my_set.remove(1);
}
int main() {
  Test();
}

/* 总结：
 * 1. 复合的意义和public继承完全不同
 * 2. 在应用领域，复合意味着 has-a（有一个），在实现领域，复合意味着（根据某物实现出）
 */