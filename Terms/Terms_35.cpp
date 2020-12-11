// Authors: Handling
// Created by 15870 on 2020/12/7.
// This is 考虑virtual函数意外的其他选择

// 1. virtual 函数带来的虚函数表以及虚表指针的负担之外
// 2. 其灵活性十分差，继承重写虚函数后不具有可变与可增内容灵活度


#include <functional>
#include <utility>
#include <iostream>
// 人物健康值的计算基本设计
class GameCharacter {
 public:
  virtual int healthValue() const {
	return 1;
  } //返回人物的健康指数
};

// 使用 NVI(Non-Virtual Interface) 手法实现 Template Method 模式（主张 virtual应该总是private）
// 私有impure virtual 使得派生类重新定义， member function 调用其 impure virtual 实现（且在调用前后都可以做额外的事情）

class GameCharacterNVI {
 public:
  int healthValue() const {
	//... 做一些事前工作
	int health_val = doHealthValue();

	//...
  }
 private:
  virtual int doHealthValue() const {
	//计算健康值
	return 1;
  }
};


//以策略模式替换 virtual， 以基于对象的思路（std::bind + std::function）替换virtual+多态


/*策略模式的构成：
 * 1. GameCharacter 作为策略的执行者
 * 2. HealthCalcFunc 作为策略（角色生命值健康情况的计算）
 */

//演示1： 不使用bind+function，使用指针作为回调函数实现策略模式
class GameCharacterStrategy1;
int defaultHealthCalc(const GameCharacterStrategy1 &gc) {
  return 1; //缺省情况下的生命情况计算
}
class GameCharacterStrategy1 {
 public:
  using HealthCalcFunc = int (*)(const GameCharacterStrategy1 &gc);

  explicit GameCharacterStrategy1(HealthCalcFunc hcf = defaultHealthCalc)
	  : health_calc_func_(hcf) {}

  void setHealthCalFunc(HealthCalcFunc hcf) { health_calc_func_ = hcf; }
  int healthValue() const { return health_calc_func_(*this); }

 private:
  HealthCalcFunc health_calc_func_;
};

/*Strategy 提供了有趣的弹性：
 * - 同一人物类型之不同实体可以有不同的健康计算函数。
 * - 某已知人物之健康指数计算函数可以在运行期变更。
 *
 */

class EvilBadGuy : public GameCharacterStrategy1 {
 public:
  explicit EvilBadGuy(HealthCalcFunc hcf = defaultHealthCalc)
	  : GameCharacterStrategy1(hcf) {}
};

int loseHealthQuickly(const GameCharacterStrategy1 &) {
  return 1;
}
int loseHealthSlowly(const GameCharacterStrategy1 &) {
  return 1;
}

void Test1() {
  EvilBadGuy ebg(loseHealthQuickly);
  ebg.setHealthCalFunc(loseHealthSlowly);
  EvilBadGuy ebg2(loseHealthSlowly);
}

//演示2： 由 std::function+std::bind 完成 Strategy 模式

//std::function 这样的对象可保持任何可调用物质函数指针，函数对象，成员函数指针（而非仅函数指针）
// 且具有一定的兼容性，可调用物的参数可以隐式转换为其function声明的参数以及返回值能隐式转换为 function声明的返回值
// 就可以兼容

class GameCharacterStrategy2;
int defaultHealthCalc2(const GameCharacterStrategy2 &gc) {
  return 1;
}
class GameCharacterStrategy2 {
 public:
  using HealthCalcFunc = std::function<int(const GameCharacterStrategy2 &)>;
  explicit GameCharacterStrategy2(const std::string &name,
								  HealthCalcFunc hcf = defaultHealthCalc2)
	  : name_(name),
		health_calc_func_(std::move(hcf)) {}

  int healthValue() const { return health_calc_func_(*this); }
  std::string name() const { return name_; }
  void setHealthCalcFunc(HealthCalcFunc hcf) { health_calc_func_ = std::move(hcf); }

 private:
  HealthCalcFunc health_calc_func_;
  std::string name_;
};

short calcHealth(const GameCharacterStrategy2 &) {
  return 1;
}
struct HealthCalculator {
  int operator()(const GameCharacterStrategy2 &) const {
	return 1;
  }
};

class GameLevel {
 public:
  float health(const GameCharacterStrategy2 &gc) const {
	std::cout << gc.name() << std::endl;
	return 1;
  }
};

void Test2() {
  GameCharacterStrategy2 ebg1("ebg1",calcHealth);
  GameCharacterStrategy2 ebg2("ebg2",HealthCalculator()); // 函数对象
  GameLevel game_level;
  ebg2.setHealthCalcFunc(std::bind(&GameLevel::health, game_level, ebg2));
  ebg2.healthValue();
}
int main() {
  Test2();
}

/* 总结：
 * 1. virtual 的替换手法八廓 NVI 手法以及 Strategy 设计模式的多种形式
 * NVI 手法自身是一个特殊形式的 Template Method 设计模式
 * 2. 将机能从成员函数移到 class 外部函数，带来的一个缺点是，非成员函数无法访问
 * class 的 non-public 成员。
 * 3. std::function 对象的行为就像一般函数指针，这样的对象可接纳 “与给定之目标签名式” 兼容
 * 的所有可用调用物。
 */