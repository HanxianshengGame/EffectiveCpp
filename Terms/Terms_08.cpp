// Authors: Handling
// Created by 15870 on 2020/11/16.
// This is 别让异常逃离析构函数

//首先C++并不禁止析构函数抛出异常，但在析构函数中抛出异常很容易导致内存泄漏（程序过早结束）

/* 实验一： 验证析构函数抛出异常的问题 */
#include <regex>
namespace  experiment1 {
class DBConnection {
 public:
 //这个函数返回一个 DBConnection对象；
static DBConnection create() {
   DBConnection db;
   return db;
 }

  //关闭联机，失败时抛出异常
  void close() { }

};

//RTII: 创建一个用来管理DBConnection资源的 class ，并在其析构函数调用 close
class DBConnManager {
 private:
  DBConnection db_;
 public:
  explicit DBConnManager(DBConnection db)
  :db_(db)
  { }


  ~DBConnManager() {
    db_.close();
  }
};


void Test() {
  DBConnManager dbc(DBConnection::create());
  //这里我们做一个评测
  /* 如果当dbc析构时调用close顺利，一切美好
   * 如果抛出异常， DBConnManager析构函数就会过早结束，并传播该异常（此时连接未关闭），会导致一定问题
   */

};


}

/* 实验二： 使用3种策略解决该问题，避免析构函数传播异常*/

namespace experiment2 {
class DBConnection {
 public:
  static DBConnection create() {
	DBConnection db;
	return db;
  }

  void close() { }

};

class DBConnManager {
 private:
  DBConnection db_;
 public:
  explicit DBConnManager(DBConnection db)
	  :db_(db)
  { }

//1: 如果 close 抛出异常就结束程序，调用 abort 即可
//  ~DBConnManager() {
//	try {
//	  db_.close();
//	} catch (...) {
//	  //记录对close的调用失败信息
//	  std::abort();
//	}
//  }
//

//2. 吞下因调用close而发生的异常
//
//   ~DBConnManager() {
//	 try {
//	   db_.close();
//	 } catch (...) {
//	   // 吞下该异常，记录对close调用信息
//	   //吞下异常也许比负担“草率结束程序”带来的风险好
//	 }
//  }

// 以上两种解决办法都无法对 “导致 close 抛出异常” 的情况做出反应

// 3. 重新设计 DBConn接口，使客户对有机会对可能出现的问题作出反应
/* @1： 管理类提供一个 close 函数，赋予客户一个机会处理因该操作而发生的异常。
 * @2： 管理类设置标志位并在析构函数调用时检测其是否正常关闭，如果未关闭，则正常关闭
 * @3： 第二步在析构函数种再次关闭失败后，我们将又退回 “强迫结束程序或吞下异常的套路”
 *
 */
  class DBConnManager2 {
   private:
    DBConnection db_;
    bool closed_;
   public:
    explicit DBConnManager2(DBConnection db)
    : db_(db)
	{ }

	void close() {
      db_.close();
      closed_ = true;
    }

    ~DBConnManager2() {
      if(!closed_) {
		try {
		  db_.close();
		  closed_ = true;
		}
		catch (...) {
		  //最终处理，记录对 close 的调用失败信息
		}
      }
    }

  };
};

}

/* 总结：
 * 1. 析构函数绝不要吐出异常， 如果一个被析构函数调用的函数可能抛出异常，析构函数
 * 应该捕捉任何异常， 然后吞下他们（不传播）或结束程序。
 *
 * 2. 如果客户端需要对某个操作函数运行期间抛出的异常做出反应，那么 class 应该提供一个
 * 普通函数（而非在析构函数种）执行该操作
 */