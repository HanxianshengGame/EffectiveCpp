// Authors: Handling
// Created by 15870 on 2020/12/10.
// This is 将与参数无关的代码抽离 templates

/* 1. 非类型模板参数会带来代码膨胀
 * 2. 将与参数无关的重复代码抽离，减少代码膨胀量（依赖于共性与变性）
 * 3. 类型模板参数也会带来代码膨胀 和 解决方案
 */


//1. 代码膨胀

#include <vector>
template <typename T, std::size_t n>
class SquareMatrix {
 public:
  void invert() { }
};

void Test() {
  SquareMatrix<double, 5> sm1;  //调用 SquareMatrix<double, 5>::invert
  sm1.invert();

  SquareMatrix<double, 10> sm2; //调用 SquareMatrix<double, 10>::invert
  sm2.invert();
}

//上述就会出现两份vert代码，由于非模板类型参数具现化模板引起的 代码膨胀


//2. 共性与变性的分析, 对类以 private 继承 或 复合 来抽离代码，并以函数参数或class成员变量
// 替换 template 参数

template <typename T>
class SquareMatrixBase2 {
 protected:
  void invert(std::size_t matrix_size) { }
};

template <typename T, std::size_t n>
class SquareMatrix2: private SquareMatrixBase2<T> {
 private:
  using SquareMatrixBase2<T>::invert;
 public:
  void invert() { this->invert(n); }
};

template <typename T>
class SquareMatrixBase3 {
 protected:
  SquareMatrixBase3(std::size_t n, T *elem)
  : size_(n),
    data_(elem)
  {

  }
  void setDataPtr(T *elem) { data_ = elem; }
 private:
  std::size_t size_;
  T *data_;
};

template <typename T, std::size_t n>
class SquareMatrix3: private SquareMatrixBase3<T> {
 public:

  explicit SquareMatrix3()
  : SquareMatrixBase3<T>(n, nullptr),
    data_(new T[n*n])
  {
    this->setDataPtr(data_);
  }

  ~SquareMatrix3() {
	delete[] data_;
  }
 private:
  T *data_;
};


void Test3() {
// 这样我们调用SquareMatrix3，不同的非类型模板参数会生成不同的 代码，但共性的那一份 SquareMatrixBase3<T> 是一样的
  SquareMatrix3<int,10> square_matrix_3;
}


//3. 类型模板参数也会导致膨胀，比如 vector<long> 与 vector<char>,
// 但只要二进制表述相同（参考指针的空间字节数）我们可以实现共享码

#include <iostream>

//将共性提取
void printAddress(void *address) {
  //...
  std::cout << address << std::endl;
  //...
}

template <class T>
void funcTest(T *address) {
  //这里写对地址做什么事情
  printAddress(address);
}





int main() {
}

/* 总结
 * 1. Templates 生成多个classes 和 多个函数，所以任何 template 代码都不应该与 某个造成膨胀
 * 的 template 参数形成相依关系
 *
 * 2. 因非类型模板参数造成的代码膨胀， 往往可以消除， 做法是以函数参数或class成员变量替换templates参数
 * 3. 因类型参数而造成的代码膨胀，往往可降低，做法是以相同二进制表述的具现类型共享实现码。
 */