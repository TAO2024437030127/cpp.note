/**
 * =====================================================================
 * C++ 知识点笔记 —— 五、模板与泛型编程
 * =====================================================================
 *
 * 目录：
 *  1. 函数模板 template<typename T>
 *  2. 类模板
 *  3. 模板特化：全特化、偏特化
 *  4. typedef / using 类型别名
 *
 * 模板的意义：编写与类型无关的通用代码，编译时按需实例化。
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ---------------------------------------------------------------------
// 1. 函数模板
// ---------------------------------------------------------------------
/*
 * template<typename T> 也可写作 template<class T>，含义相同。
 * 编译器会根据实参自动推导 T。
 */
template <typename T>
T my_max(T a, T b)
{
    return a > b ? a : b;   // 要求 T 支持 > 运算符
}

// 多个模板参数：返回值类型也可以由模板指定
template <typename T, typename U>
auto add2(T a, U b) -> decltype(a + b)    // 尾置返回类型
{
    return a + b;
}

// 模板参数可显式指定
template <typename T>
T to_num(const string &s);   // 声明示例

template <typename T>
T to_num(const string &s)
{
    // 简单示意，实际可以 stoi/stol/stod
    return static_cast<T>(stod(s));
}

void demo_function_template()
{
    cout << "\n--- 函数模板 ---\n";
    cout << my_max(3, 5) << '\n';          // 自动推导 int
    cout << my_max(2.7, 1.8) << '\n';      // 自动推导 double
    cout << my_max<string>("abc", "abd") << '\n';

    cout << add2(3, 4.5) << '\n';          // double
    cout << add2<int, int>(1, 2) << '\n';  // 显式指定 T=U=int

    cout << to_num<int>("42") << '\n';     // 显式 T=int
}

// ---------------------------------------------------------------------
// 2. 类模板
// ---------------------------------------------------------------------
/*
 * 类模板让容器/数据结构支持任意类型。
 * 注意：模板的成员函数通常在头文件内实现，因为需要可见完整定义才能实例化。
 */
template <typename T>
class MyArray {
private:
    T *arr;
    int size_;
public:
    explicit MyArray(int n) : size_(n) {
        arr = new T[size_]{};
    }
    ~MyArray() { delete[] arr; }

    T &operator[](int i) { return arr[i]; }       // 支持 arr[i] 修改
    int size() const { return size_; }
};

template <typename T, int N>   // 非类型模板参数：编译期常量
class FixedArray {
    T data[N];
public:
    int size() const { return N; }
    T &operator[](int i) { return data[i]; }
};

void demo_class_template()
{
    cout << "\n--- 类模板 ---\n";
    MyArray<double> a(3);
    a[0] = 1.5;
    a[1] = 2.5;
    cout << "MyArray size=" << a.size() << ", a[1]=" << a[1] << '\n';

    FixedArray<int, 5> fa;                    // N=5 编译期确定
    fa[0] = 10;
    cout << "FixedArray size=" << fa.size() << ", fa[0]=" << fa[0] << '\n';
}

// ---------------------------------------------------------------------
// 3. 模板特化
// ---------------------------------------------------------------------
/*
 * 全特化：针对某个具体类型写专门实现。
 * 偏特化：只指定部分模板参数（指针、引用、多个参数中的一部分等）。
 * 特化常用于优化特殊类型，或处理不支持默认实现的类型。
 */

// 通用版本
template <typename T>
string describe(T x)
{
    return "generic: " + to_string(static_cast<long long>(x));
}

// 全特化：T = string
template <>
string describe<string>(string s)
{
    return "specialized string: " + s;
}

// 类模板特化
template <typename T>
class Printer {
public:
    static void print(const T &v) { cout << "generic print: " << v << '\n'; }
};

// 全特化：T = double
template <>
class Printer<double> {
public:
    static void print(const double &v) { cout << "double print: " << v << '\n'; }
};

// 偏特化：T*（指针类型）
template <typename T>
class Printer<T *> {
public:
    static void print(T *p) {
        if (p) cout << "pointer print: " << *p << '\n';
        else cout << "nullptr\n";
    }
};

void demo_specialization()
{
    cout << "\n--- 模板特化 ---\n";
    cout << describe(123) << '\n';               // generic
    cout << describe(string("hello")) << '\n';   // 全特化 string

    Printer<int> p1;
    p1.print(5);

    Printer<double> p2;
    p2.print(3.14);                              // 全特化 double

    int x = 7;
    Printer<int *> p3;
    p3.print(&x);                                // 偏特化指针
}

// ---------------------------------------------------------------------
// 4. typedef / using
// ---------------------------------------------------------------------
/*
 * typedef 是 C 风格类型别名。
 * using 是 C++11 风格类型别名，可读性更好，也支持别名模板。
 */
typedef unsigned long long ull;                  // typedef
using ll = long long;                            // using 别名

template <typename T>
using Vec = vector<T>;                           // 别名模板：typedef 做不到

void demo_type_alias()
{
    cout << "\n--- 类型别名 ---\n";
    ull u = 1ULL;
    ll l = 2LL;
    Vec<int> v{1, 2, 3};
    cout << u << ' ' << l << ' ' << v.size() << '\n';
}

int main()
{
    cout << "======== 05 模板与泛型编程 ========\n";
    demo_function_template();
    demo_class_template();
    demo_specialization();
    demo_type_alias();
    return 0;
}
