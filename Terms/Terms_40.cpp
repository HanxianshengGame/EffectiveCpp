// Authors: Handling
// Created by 15870 on 2020/12/9.
// This is 明智而审慎地使用多重继承


/* 1. 歧义
 * 2. 菱形继承的问题
 * 3. 最适合使用多重继承的场景
 */


//1. 多个base class 的成员名字相同造成的歧义
#include <iostream>
#include <cstring>
class BorrowableItem {
 public:
  void checkOut() {
    std::cout << "BorrowableItem  void checkOut() " << std::endl;
  }
};

class ElectronicGadget {
 public:
  bool checkOut() const {
	std::cout << "ElectronicGadget bool checkOut() " << std::endl;
  }
};

class MP3Player:
	public BorrowableItem,
	public ElectronicGadget
{

};

void Test() {
  MP3Player mp3_player;
 // mp3_player.checkOut();  //??? 歧义
  mp3_player.ElectronicGadget::checkOut();  //显示指定base class即可解决
}

//2. 菱形继承的问题(虚拟继承)

class File { };
class InputFile: virtual public File { };
class OutputFile: virtual public File { };
class IOFile:
	public InputFile,
	public OutputFile
{
  /* 虚拟继承带来的影响
   * 1. 使用virtual继承的那些 classes 所产生的对象往往比使用 non-virtual
   * 继承的兄弟们体积大（安插共享指针），访问 virtual base classes 成员也慢
   *
   * 2. virtual base 的初始化责任是由继承体系中的最底层（最高级别的派生类）
   * 负责。
   */

};

/* 关于虚拟继承的建议
 * 1. 第一，非必要不使用 virtual bases，平常请使用non-virtual继承
 * 2. 第二，如果你必须使用 virtual base classes，尽可能避免在其中放置数据，这样
 * 可用避免在 base classes 初始化发生错误
 */


//3. 正确使用多重继承

#include <memory>

//一个人的基本接口
class IPerson {
 public:
  virtual ~IPerson() = default;
  virtual std::string name() const = 0;
};


class DatabaseID { };
//获取一个数据库信息
DatabaseID askUserForDatabaseID() {
  return DatabaseID();
}

//人类基本信息实现
class PersonInfo {
 public:
  explicit PersonInfo(DatabaseID pid)
  : pid_(pid)
  { }
  virtual ~PersonInfo() = default;
  virtual const char* theName() const;

 private:
  virtual const char* valueDelimOpen() const;
  virtual const char* valueDelimClose() const;

  DatabaseID pid_;

};
const char *PersonInfo::theName() const {
  static char value[1024];
  std::strcpy(value, valueDelimOpen());

  //得到名字

  std::strcat(value, valueDelimClose());

  return value;
}

const char *PersonInfo::valueDelimOpen() const {
  return "[";
}
const char *PersonInfo::valueDelimClose() const {
  return "]";
}


// 使用 CPerson 表现人的实体，公有继承接口（IPerson）与 私有继承实现 (PersonInfo)
// 由于CPerson是转调Personinfo的实现来完成自己的接口，且需要重新定义 virtual 函数
// 那么就具备 （根据某物实现的语义）, 使用private继承或者 public继承+复合 来完成

class CPerson: public IPerson, private PersonInfo {
 public:
  explicit CPerson(DatabaseID pid)
  : PersonInfo(pid)
  { }
  std::string name() const override {
	return PersonInfo::theName();
  }
 private:
  const char *valueDelimOpen() const override {
	return "1";
  }
  const char *valueDelimClose() const override {
	return "1";
  }
};



std::unique_ptr<IPerson> makePerson(
	const DatabaseID &person_identifier) {
  return std::unique_ptr<IPerson>(new CPerson(person_identifier));
}




int main() {
  DatabaseID id;
  auto person = makePerson(id);
  std::cout << person->name();
}

/* 总结：
 * 1. 多重继承比单一继承复杂，且容易引起歧义，使用显示指定base class可解决
 * 2. virtual继承会增加大小，速度，初始化复杂度等成本，如果 virtual base classes
 * 不带任何数据，那将是最具实用价值的情况
 * 3. 多重继承的确有正当用途，其中最常用即
 * public 继承某个 interface 与 private 继承某个协助实现的 class 的情况
 */