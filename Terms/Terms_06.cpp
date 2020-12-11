// Authors: Handling
// Created by 15870 on 2020/11/13.
// This is 若不想使用编译器自动生成的函数，就应该明确拒绝

/* 最简单的拒绝（copy构造函数与 copy赋值运算符）办法是 声明其为 private
 * 1. 为了解决在成员函数和友元函数仍然还是能调用，声明其而不定义其是个好办法（会报链接错误）
 * 2. 为了将链接错误提前到编译期，需要将其继承一个阻止 copy 的 base class（利用了继承了基类的拷贝操作为private的类，
 *                                                                    编译器自身将不会生成其 拷贝操作，使用时就会报错）
 */

class Uncopyable {
 protected:
  Uncopyable() { }
  ~Uncopyable() { }

 private:

  Uncopyable(const Uncopyable& );
  Uncopyable& operator=(const Uncopyable& );
};


class HomeForSale: private Uncopyable {

};

void Test() {
  HomeForSale h1;
 HomeForSale h2(h1); //编译器报错，因为 HomeForSale并没有拷贝构造函数
}

