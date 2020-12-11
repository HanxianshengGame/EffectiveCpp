// Authors: Handling
// Created by 15870 on 2020/11/27.
// This is 设计class犹如设计 type

// 如何设计高效的 classes 呢,  遵循问题产出设计规范：

/* 1. 新的type的对象应该如何被创建和销毁？ （构造函数析构函数）
 * 2. 对象的初始化和对象的复制该有什么样的差别？ （构造函数与拷贝赋值运算符）
 * 3. 新type的对象如果被passed by value（以值传递），意味着什么？ （copy构造函数）
 * 4. 什么是新typed  "合法值"？                    （维护约束条件，要在构造，赋值，setter函数进行错误检查）
 * 5. 你的新 type 需要配合某个继承图系？              （virtual 与 non-virtual的影响， 特别是析构函数 virtual）
 * 6. 你的新 type 需要什么类型的转换？               （explict 与 non-explict，以及隐式转换运算符定义）
 * 7. 什么样的操作符和函数对该新type而言是合理的？     (声明哪些函数， memeber函数还是否)
 * 8. 什么样的标准函数应该被驳回？                   （ 编译器自动生成的那些声明其为 delete）
 * 9. 谁该取用新的 type 的成员？                     （存取函数进行约束）
 * 10. 什么是新的 type 的“未声明接口”？               （）
 * 11. 你的新 type 有多么一般化？                     （template 或 一整个types家族）
 * 12. 你真的需要一个新type吗？                       （如果只是为了扩充功能而进行派生，倒不如直接定义一个 non-member函数）
 *
 */
