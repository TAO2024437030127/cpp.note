/**
 * =====================================================================
 * C++ 知识点笔记 —— 四、面向对象 OOP
 * =====================================================================
 *
 * 目录：
 *  1. 类与对象：class/struct、public/protected/private
 *  2. 构造函数、拷贝构造、移动构造、析构函数
 *  3. 运算符重载
 *  4. 继承：公有/保护/私有、多继承、菱形继承、虚继承
 *  5. 多态：虚函数、纯虚函数、抽象类、动态/静态绑定
 *  6. this 指针
 */

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// ---------------------------------------------------------------------
// 1. 类与对象
// ---------------------------------------------------------------------
/*
 * struct 默认访问控制是 public；class 默认是 private。
 * struct 适合简单数据结构；class 适合带封装、继承的抽象类型。
 */
struct Point {
    int x, y;              // struct 里默认 public
    Point(int a = 0, int b = 0) : x(a), y(b) {}
};

class Person {
public:                    // 公开接口
    Person(string n) : name(n) {}
    string getName() const { return name; }
private:                   // 私有成员：外部不能直接访问
    string name;
};

// 访问控制总结：
//   public:    类内外都能访问
//   protected: 本类和派生类可访问，外部不能
//   private:   仅本类可访问（友元除外）

// ---------------------------------------------------------------------
// 2. 构造 / 拷贝 / 移动 / 析构
// ---------------------------------------------------------------------
/*
 * 构造：
 *   默认构造：无参构造；一旦自定义了带参构造，默认构造不会自动生成。
 *   带参构造：传参初始化。
 *   拷贝构造：用一个对象构造另一个对象。
 *   移动构造：C++11 引入，从临时对象“偷”资源。
 *   析构函数：对象销毁时调用，用于释放资源。
 *
 * 调用时机：
 *   Test t2(t1);      // 拷贝构造
 *   func(Test(10));   // 移动构造/拷贝构造（编译器优化可能省略）
 *
 * 浅拷贝 vs 深拷贝：
 *   默认拷贝构造是浅拷贝：指针成员只复制地址，两个对象指向同一块堆内存，
 *   析构时会 double free。
 *   深拷贝：分配新内存并复制数据。
 */
class Test {
public:
    int *data = nullptr;

    Test() { cout << "默认构造\n"; }

    Test(int n) {
        cout << "带参构造 " << n << '\n';
        data = new int(n);
    }

    // 拷贝构造（深拷贝）
    Test(const Test &other) {
        cout << "拷贝构造(深拷贝)\n";
        if (other.data) {
            data = new int(*other.data);   // 新分配一块内存
        }
    }

    // 移动构造（C++11）
    Test(Test &&other) noexcept {
        cout << "移动构造\n";
        data = other.data;                 // 偷走指针
        other.data = nullptr;              // 源对象置空，防止析构释放
    }

    // 析构函数
    ~Test() {
        cout << "析构\n";
        delete data;
    }

    // 赋值运算符：也需处理深拷贝/自赋值（此处简单演示）
    Test &operator=(const Test &other) {
        cout << "拷贝赋值\n";
        if (this != &other) {
            delete data;
            data = other.data ? new int(*other.data) : nullptr;
        }
        return *this;
    }
};

Test makeTemp(); // 前向声明

void demo_ctor_copy_move()
{
    cout << "\n--- 构造/拷贝/移动 ---\n";

    Test a(10);
    Test b(a);             // 拷贝构造：b.data 是独立内存
    *b.data = 20;
    cout << "a.data=" << *a.data << ", b.data=" << *b.data << '\n';

    Test c(Test(30));      // 编译器可能优化为直接构造；C++17 起省略拷贝/移动
    c = a;                 // 拷贝赋值
    cout << "c.data=" << *c.data << '\n';

    // 移动构造示例
    Test m(makeTemp());
    cout << "m.data=" << *m.data << '\n';
}

Test makeTemp()
{
    Test t(99);
    return t;
}

// ---------------------------------------------------------------------
// 2.1 虚析构 virtual ~
// ---------------------------------------------------------------------
/*
 * 当基类指针指向派生类对象时，如果析构函数不是 virtual，
 * delete base 只会调用基类析构函数，不会调用派生类析构函数，导致资源泄漏。
 * 因此基类析构函数通常声明为 virtual。
 */
class BaseV {
public:
    BaseV() { cout << "BaseV 构造\n"; }
    virtual ~BaseV() { cout << "BaseV 析构\n"; }   // 虚析构
};

class DerivedV : public BaseV {
public:
    DerivedV() { cout << "DerivedV 构造\n"; }
    ~DerivedV() override { cout << "DerivedV 析构\n"; }
};

void demo_virtual_destructor()
{
    cout << "\n--- 虚析构 ---\n";
    BaseV *p = new DerivedV();
    delete p;              // 同时调用 DerivedV 和 BaseV 析构
}


// ---------------------------------------------------------------------
// 3. 运算符重载
// ---------------------------------------------------------------------
/*
 * 运算符重载有两种方式：
 *   1. 成员函数重载：左操作数必须是当前类对象
 *   2. 全局函数重载：支持左操作数为其他类型，更灵活
 * 常见可重载：+ - * / == < << >> [] () = new delete
 * 注意事项：
 *   - 不能改变运算符优先级/结合性/操作数个数
 *   - 不是所有运算符都可重载（如 :: . .* ?:）
 *   - 一般把 ==、< 用于结构体比较/排序，operator<< 用于输出
 */
class Money {
public:
    int yuan;
    Money(int y = 0) : yuan(y) {}

    // 成员函数重载 +
    Money operator+(const Money &other) const {
        return Money(yuan + other.yuan);
    }

    // 成员函数重载 ==
    bool operator==(const Money &other) const {
        return yuan == other.yuan;
    }

    // 友元 + 全局函数实现 cout << Money
    friend ostream &operator<<(ostream &os, const Money &m);
};

// 全局函数重载 << （输出流左操作数不是 Money，因此必须全局/友元）
ostream &operator<<(ostream &os, const Money &m) {
    os << m.yuan;
    return os;
}

// 全局函数重载 比较大小，方便 set/map/sort
bool operator<(const Money &a, const Money &b) {
    return a.yuan < b.yuan;
}

void demo_operator_overload()
{
    cout << "\n--- 运算符重载 ---\n";
    Money m1(10), m2(20);
    Money m3 = m1 + m2;
    cout << "m3 = " << m3 << '\n';
    cout << boolalpha << (m1 == Money(10)) << ' ' << (m1 < m2) << noboolalpha << '\n';
}

// ---------------------------------------------------------------------
// 4. 继承
// ---------------------------------------------------------------------
/*
 * 继承方式影响基类成员在派生类中的访问级别：
 *   公有继承 public：public -> public，protected -> protected
 *   保护继承 protected：public/protected -> protected
 *   私有继承 private：public/protected -> private
 * 最常用是 public 继承，表示 is-a 关系。
 */

// 公有继承演示
class AnimalBasic {
public:
    void eat() { cout << "Animal eat\n"; }
protected:
    int age = 0;
private:
    int secret = 1;   // 派生类也不能直接访问
};

class DogBasic : public AnimalBasic {
public:
    void setAge(int a) { age = a; }   // protected 可访问
    // void f() { secret = 2; }      // 错误：private 不可访问
};

// 多继承
class A {
public:
    int a = 1;
    void fa() { cout << "A::fa\n"; }
};

class B {
public:
    int b = 2;
    void fb() { cout << "B::fb\n"; }
};

class C : public A, public B {
public:
    int c = 3;
};

// 菱形继承 + 虚继承
class DiamondBase {
public:
    int value = 100;
};

class D1 : virtual public DiamondBase {};
class D2 : virtual public DiamondBase {};
class Diamond : public D1, public D2 {};

void demo_inheritance()
{
    cout << "\n--- 继承 ---\n";
    DogBasic dog;
    dog.eat();
    dog.setAge(5);

    C obj;
    obj.fa();
    obj.fb();
    cout << "a+b+c=" << obj.a + obj.b + obj.c << '\n';

    // 如果不使用 virtual，Diamond 中会有两份 DiamondBase::value，
    // 访问 obj.value 会产生二义性；虚继承让 DiamondBase 只有一份。
    Diamond d;
    cout << "diamond value = " << d.value << '\n';
}

// ---------------------------------------------------------------------
// 5. 多态
// ---------------------------------------------------------------------
/*
 * 静态绑定：编译期根据静态类型决定调用哪个函数（普通函数、非虚函数重载）。
 * 动态绑定：运行期根据对象实际类型调用虚函数，通过虚函数表（vtable）实现。
 *
 * 虚函数 virtual：在基类声明，派生类用 override 覆盖。
 * 纯虚函数 =0：基类不能实例化（抽象类），派生类必须实现才能创建对象。
 */
class Shape {
public:
    // 纯虚函数：Shape 是抽象类
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r;
public:
    explicit Circle(double radius) : r(radius) {}
    double area() const override { return 3.14159265358979 * r * r; }
};

class Rect : public Shape {
    double w, h;
public:
    Rect(double width, double height) : w(width), h(height) {}
    double area() const override { return w * h; }
};

void printArea(const Shape &s)
{
    // 形参是 Shape&，实际根据对象动态绑定 area()
    cout << "area = " << s.area() << '\n';
}

void demo_polymorphism()
{
    cout << "\n--- 多态 ---\n";
    Circle c(2.0);
    Rect r(3.0, 4.0);
    printArea(c);   // 输出圆的面积
    printArea(r);   // 输出矩形面积
}

// ---------------------------------------------------------------------
// 6. this 指针
// ---------------------------------------------------------------------
/*
 * this 是成员函数中指向“当前调用对象”的指针。
 * 用途：
 *   - 形参与成员同名时区分；
 *   - 链式调用返回 *this；
 *   - 拷贝赋值运算符中检查自赋值。
 * 静态成员函数没有 this。
 */
class Counter {
    int cnt;
public:
    Counter(int v = 0) : cnt(v) {}
    Counter &set(int cnt) {
        this->cnt = cnt;          // this->cnt 是成员，cnt 是形参
        return *this;             // 返回对象自身
    }
    Counter &add(int x) {
        cnt += x;
        return *this;
    }
    int get() const { return cnt; }
};

void demo_this()
{
    cout << "\n--- this 指针 ---\n";
    Counter c;
    c.set(5).add(10);             // 链式调用
    cout << "counter = " << c.get() << '\n';
}

int main()
{
    cout << "======== 04 面向对象 OOP ========\n";
    demo_ctor_copy_move();
    demo_virtual_destructor();
    demo_operator_overload();
    demo_inheritance();
    demo_polymorphism();
    demo_this();
    return 0;
}
