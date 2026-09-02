/**
 * =====================================================================
 * C++ 知识点笔记 —— 六、C++11 及以后核心特性（笔试高频）
 * =====================================================================
 *
 * 目录：
 *  1. auto / decltype
 *  2. 范围 for 循环
 *  3. 右值引用、移动语义、std::move
 *  4. lambda 表达式
 *  5. 智能指针 unique_ptr / shared_ptr / weak_ptr
 *  6. 初始化列表 {}
 *  7. nullptr
 *  8. constexpr
 */

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
using namespace std;

// ---------------------------------------------------------------------
// 1. auto / decltype
// ---------------------------------------------------------------------
/*
 * auto：根据初始化表达式自动推导类型。
 *   使用场景：遍历迭代器、模板返回类型、复杂类型简化。
 * decltype：不实际执行表达式，仅取得表达式的类型。
 */
void demo_auto_decltype()
{
    cout << "\n--- auto / decltype ---\n";

    auto a = 10;                 // int
    auto b = 3.14;               // double
    vector<string> vs{"a", "b"};
    auto it = vs.begin();        // vector<string>::iterator

    // 用 decltype 获取变量类型
    decltype(a) c = a + 5;       // int

    // 引用折叠 / decltype(auto) 也是 C++14 特性，笔试偶尔出现
    cout << a << ' ' << b << ' ' << (*it) << ' ' << c << '\n';

    // auto 会忽略顶层 const 和引用：
    const int ci = 0;
    auto x1 = ci;                // x1 是 int，不是 const int
    const auto x2 = ci;          // 手动加 const
    int &r = a;
    auto x3 = r;                 // x3 是 int，不是引用
    auto &x4 = r;                // x4 是 int&，需要显式 &
    (void)x1; (void)x2; (void)x3; (void)x4;
}

// ---------------------------------------------------------------------
// 2. 范围 for 循环
// ---------------------------------------------------------------------
void demo_range_for()
{
    cout << "\n--- 范围 for ---\n";
    vector<int> v{1, 2, 3, 4};

    // 只读：for (auto x : v)
    for (int x : v) cout << x << ' ';
    cout << '\n';

    // 修改：需要引用，最好用 auto&
    for (auto &x : v) x *= 10;
    for (const auto &x : v) cout << x << ' ';   // const auto& 避免拷贝
    cout << '\n';

    // 注意：范围 for 中修改容器可能导致迭代器失效，通常不要增删元素
}

// ---------------------------------------------------------------------
// 3. 右值引用、移动语义、std::move
// ---------------------------------------------------------------------
/*
 * 左值：表达式结束后依然存在的对象（有地址）。
 * 右值：临时对象、字面量等（没有持久地址）。
 * 移动语义：把临时对象的资源“转移”而非“拷贝”。
 * std::move：把一个左值“强转”成右值引用，目的是匹配移动构造/移动赋值。
 * 使用后原对象处于有效但未指定状态，一般不应再使用（可重新赋值/析构）。
 */
class BigBuffer {
public:
    int *p;
    BigBuffer() : p(new int[100]) {}
    ~BigBuffer() { delete[] p; }

    // 拷贝构造
    BigBuffer(const BigBuffer &other) : p(new int[100]) {
        cout << "拷贝构造（深拷贝）\n";
        for (int i = 0; i < 100; i++) p[i] = other.p[i];
    }

    // 移动构造
    BigBuffer(BigBuffer &&other) noexcept : p(other.p) {
        cout << "移动构造（偷资源）\n";
        other.p = nullptr;
    }
};

BigBuffer makeBuffer()
{
    BigBuffer b;
    return b;   // RVO/移动优化后通常不产生额外拷贝
}

void demo_move()
{
    cout << "\n--- 右值引用 / std::move ---\n";
    BigBuffer a;
    BigBuffer b(std::move(a));  // 将 a 转为右值，调用移动构造
    // 此时 a.p 为 nullptr，不要再用 a 访问资源
}

// ---------------------------------------------------------------------
// 4. lambda 表达式
// ---------------------------------------------------------------------
/*
 * 语法：[捕获列表](参数列表) -> 返回类型 { 函数体 }
 * 捕获方式：
 *   []            不捕获
 *   [=]          值捕获全部外部变量
 *   [&]          引用捕获全部外部变量
 *   [x]          值捕获 x
 *   [&x]         引用捕获 x
 *   [=, &x]      混合
 * 用途：sort 自定义比较、回调、局部函数。
 */
void demo_lambda()
{
    cout << "\n--- lambda ---\n";
    vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};

    // 无捕获：排序自定义比较
    sort(v.begin(), v.end(), [](int x, int y) { return x > y; });
    cout << "降序: ";
    for (int x : v) cout << x << ' ';
    cout << '\n';

    // 捕获外部变量
    int base = 100;
    auto add_base = [base](int x) { return x + base; };   // 值捕获
    auto inc_base = [&base](int x) { return x + (++base); }; // 引用捕获
    cout << "value capture: " << add_base(1) << ", base=" << base << '\n';
    cout << "ref capture: " << inc_base(1) << ", base=" << base << '\n';

    // 指定返回类型
    auto divide = [](double x, double y) -> double { return x / y; };
    cout << divide(7.0, 2.0) << '\n';
}

// ---------------------------------------------------------------------
// 5. 智能指针
// ---------------------------------------------------------------------
/*
 * unique_ptr：独占所有权，不可拷贝，可 move 转移。
 * shared_ptr：共享所有权，引用计数归零时释放对象。
 * weak_ptr：不增加引用计数，用于观察/打破 shared_ptr 循环引用。
 *
 * 底层要点：
 *  - shared_ptr 内部有控制块，保存引用计数和删除器。
 *  - 循环引用：A 持 shared_ptr<B>，B 持 shared_ptr<A> 时，计数永远不为 0；
 *    把其中一方改为 weak_ptr 可打破。
 */
struct Node {
    string name;
    Node(string n) : name(n) { cout << "Node 构造 " << name << '\n'; }
    ~Node() { cout << "Node 析构 " << name << '\n'; }
};

// 演示循环引用：A->B, B->A 会造成泄漏
struct PersonA;
struct PersonB;

struct PersonA {
    shared_ptr<PersonB> b;
    ~PersonA() { cout << "~PersonA\n"; }
};

struct PersonB {
    // 使用 weak_ptr 打破循环
    weak_ptr<PersonA> a;
    ~PersonB() { cout << "~PersonB\n"; }
};

void demo_smart_ptr()
{
    cout << "\n--- 智能指针 ---\n";

    // unique_ptr
    unique_ptr<Node> u1(new Node("u1"));
    // unique_ptr<Node> u2 = u1;             // 错误：不可拷贝
    unique_ptr<Node> u2 = std::move(u1);     // 转移所有权
    if (u2) cout << "u2 owns node\n";

    // shared_ptr
    shared_ptr<Node> s1 = make_shared<Node>("s1");
    {
        shared_ptr<Node> s2 = s1;            // 拷贝增加引用计数
        cout << "use_count=" << s1.use_count() << '\n';
    }
    cout << "after scope use_count=" << s1.use_count() << '\n';

    // 循环引用 + weak_ptr 打破
    shared_ptr<PersonA> pa = make_shared<PersonA>();
    shared_ptr<PersonB> pb = make_shared<PersonB>();
    pa->b = pb;
    pb->a = pa;          // weak_ptr 不会使 PersonA 引用计数增加
    cout << "pa use_count=" << pa.use_count() << ", pb use_count=" << pb.use_count() << '\n';
}

// ---------------------------------------------------------------------
// 6. 初始化列表 {}
// ---------------------------------------------------------------------
/*
 * {} 称为列表初始化/大括号初始化，可防止窄化转换。
 * initializer_list<T> 是标准库支持接收 {} 的类型。
 */
class IntBag {
    vector<int> data;
public:
    IntBag(initializer_list<int> list) : data(list) {}
    int sum() const {
        int res = 0;
        for (int x : data) res += x;
        return res;
    }
};

void demo_initializer_list()
{
    cout << "\n--- 初始化列表 ---\n";
    int x{42};               // 安全初始化
    (void)x;
    // int y{3.14};          // 错误：double -> int 窄化
    IntBag bag{1, 2, 3, 4};
    cout << "bag sum=" << bag.sum() << '\n';

    vector<int> v{1, 2, 3};  // 调用 initializer_list 构造
    cout << "vector size=" << v.size() << '\n';
}

// ---------------------------------------------------------------------
// 7. nullptr
// ---------------------------------------------------------------------
/*
 * nullptr 是 C++11 的空指针常量，类型为 nullptr_t，可隐式转为任意指针。
 * 旧式 NULL 本质是 0，在函数重载时可能误匹配 int。
 */
void f(int) { cout << "f(int)\n"; }
void f(char *) { cout << "f(char*)\n"; }

void demo_nullptr()
{
    cout << "\n--- nullptr ---\n";
    int *p1 = nullptr;
    int *p2 = NULL;    // 在 C++ 中通常也 OK，但推荐 nullptr
    if (p1 == nullptr) cout << "p1 null\n";
    (void)p2;

    f(nullptr);   // 会调用 f(char*)
    // f(NULL);   // 可能有二义性：NULL 若是 0，则调用 f(int)
}

// ---------------------------------------------------------------------
// 8. constexpr
// ---------------------------------------------------------------------
/*
 * constexpr 表示“编译期可求值”的常量表达式函数/变量。
 * const 强调“运行时不可修改”，constexpr 强调“编译期求值”。
 */
constexpr int factorial(int n)
{
    // C++14 起 constexpr 函数体内可以有循环
    int res = 1;
    for (int i = 2; i <= n; i++) res *= i;
    return res;
}

void demo_constexpr()
{
    cout << "\n--- constexpr ---\n";
    constexpr int fac10 = factorial(10);   // 编译期计算
    int arr[fac10 > 0 ? 10 : 1];           // 可作数组长度
    cout << "factorial(10)=" << fac10 << '\n';
    (void)arr;
}

int main()
{
    cout << "======== 06 C++11 核心特性 ========\n";
    demo_auto_decltype();
    demo_range_for();
    demo_move();
    demo_lambda();
    demo_smart_ptr();
    demo_initializer_list();
    demo_nullptr();
    demo_constexpr();
    return 0;
}
