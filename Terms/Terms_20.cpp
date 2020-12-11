// Authors: Handling
// Created by 15870 on 2020/11/29.
// This is 宁以 pass-by-reference-to-const 替换 pass-by-value

/* 探讨
 * 1. 使用引用类型能提升效率问题
 * 2. 使用引用进行传参可以实现多态且避免对象切割问题
 */
namespace experiment1 {
class Person {
 public:
  Person() = default;
  virtual ~Person() = default;
 private:
  std::string name_;
  std::string address_;
};

class Student: public Person {
 public:
  Student() = default;
  ~Student() = default;
 private:
  std::string school_name_;
  std::string school_address_;
};

bool VaildateStudent(Student a) {
  // a 执行了拷贝构造函数，且离开函数局部时销毁，内部执行了4个string的构造析构过程，耗时

}
bool VaildateStudent(const Student &a) {

}
}
namespace experiment2 {
class Window {
 public:
  std::string name() const { return name_; }
  virtual void display() const { //成员函数小写单词开头+大写单词
    // 显示窗口和其内容
  }
 private:
  std::string name_;
};

class WindowWithScrollBars: public Window {
 public:
  virtual void display() const override {
    // 显示窗口和其内容
  }
};

//by value 会发生截断， by reference 会有多态的效果
void PrintNameAndDisplay(Window w) {
  w.display();
}
void PrintNameAndDisplay(const Window &w) {
  w.display();
}

}


/* 总结
 * 1. 尽量以 pass by reference, 即高效也支持多态，避免切割问题
 * 2. 内置类型以及 STL 迭代器和函数对象，使用 pass by value 比较适当，（属于c语言块的内容以c语言方式进行处理）
 */