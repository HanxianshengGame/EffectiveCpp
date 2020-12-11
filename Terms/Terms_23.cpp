// Authors: Handling
// Created by 15870 on 2020/11/29.
// This is 宁以 non-member, non-friend 替换 member 函数

/* 实验
 * 1. 在许多情况下 非成员非友元函数的做法比 member 好得多。
 * 2. C++ 对于命名空间的妙用
 */

namespace experiment1 {
class WebBrower {
 public:
  void clearCache() { }
  void clearHistory() { }
  void removeCookies() { }

  void clearEverything() { } //调用上面3个成员函数

};

void clearBrowser(WebBrower &wb) {  //非成员版本
  wb.clearCache();
  wb.clearHistory();
  wb.removeCookies();
}
/* 1. 面向对象守则要求数据尽可能被封装，member函数封装性比 non-member 函数差，且需要重新编译整个类与相关联的
 *    因为不想调用到该函数的实例对象也能调用到，但如果是非成员版本就是谁能用谁调用.
 * 2. 愈多东西被封装，我们改变那些东西的能力就越大，越方便，因为涉及到有限的客户代码
 */

}
namespace experiment2 {
namespace WebBrowserStuff {
class WebBrowser {  };
}



// 将所有便利函数防止多个头文件内但隶属于同一个命名空间，意味着客户可以轻松扩展这一组
//便利函数，他们需要做的就是添加更多 non-member non-frient 函数到此命名空间内。


namespace WebBrowserStuff {
// 清理函数

void clearBrowser(WebBrowser &wb) {

}
}

namespace WebBrowserStuff {
// 写书签相关的便利函数
}
}


/* 总结
 * 1. 宁可拿 non-member non-friend 函数替换member函数，这样做可能增加封装性，包裹弹性
 * 和机能扩充性。
 */