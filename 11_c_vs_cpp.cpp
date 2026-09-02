/**
 * =====================================================================
 * C++ 知识点笔记 —— 十一、C 和 C++ 区别
 * =====================================================================
 *
 * 目录：
 *  1. 面向对象支持
 *  2. 函数重载、模板
 *  3. new/delete vs malloc/free
 *  4. 引用、bool 类型
 *  5. STL 库
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// ---------------------------------------------------------------------
// 1. 面向对象支持
// ---------------------------------------------------------------------
/*
 * C 是面向过程语言，主要用函数组织代码。
 * C++ 是面向对象语言，支持 class、继承、多态、封装。
 * C++ 也兼容大部分 C 语法，可面向过程书写。
 */

// ---------------------------------------------------------------------
// 2. 函数重载、模板
// ---------------------------------------------------------------------
/*
 * C 语言不允许同名函数重载（编译后函数名就是原名加下划线等）。
 * C++ 支持函数重载，靠“名字修饰(name mangling)”区分。
 * C++ 还支持模板，实现泛型编程；C 没有模板。
 */
int add_c_style(int a, int b) { return a + b; }

// C++ 重载
int add_cpp(int a, int b) { return a + b; }
double add_cpp(double a, double b) { return a + b; }

// 模板
template <typename T>
T add_t(T a, T b) { return a + b; }

void demo_overload_template()
{
    cout << "\n--- 函数重载 / 模板 ---\n";
    cout << add_cpp(1, 2) << '\n';
    cout << add_cpp(1.5, 2.5) << '\n';
    cout << add_t(3, 4) << '\n';
}

// ---------------------------------------------------------------------
// 3. new/delete vs malloc/free
// ---------------------------------------------------------------------
/*
 * malloc/free 是 C 的库函数：
 *   - 只分配/释放内存，不调用构造函数/析构函数。
 *   - 返回 void*，需要强制类型转换。
 *   - 需要自己指定字节数。
 *
 * new/delete 是 C++ 运算符：
 *   - new 分配内存并调用构造函数，delete 调用析构函数并释放。
 *   - 类型安全，不需要强转（除 void* 场景）。
 *   - new[]/delete[] 支持数组。
 *
 * 实际工程优先：
 *   - 对象资源用 new/delete 或更推荐智能指针；
 *   - 与 C 库对接可用 malloc/free。
 */
class DemoObj {
public:
    int v;
    DemoObj(int x) : v(x) { cout << "DemoObj 构造 " << v << '\n'; }
    ~DemoObj() { cout << "DemoObj 析构 " << v << '\n'; }
};

void demo_new_vs_malloc()
{
    cout << "\n--- new/delete vs malloc/free ---\n";

    // C 风格：不会调用构造/析构
    int *p = (int *)malloc(sizeof(int));
    *p = 10;
    free(p);

    // C++ 风格：调用构造/析构
    DemoObj *obj = new DemoObj(42);
    delete obj;

    // 数组
    DemoObj *arr = new DemoObj[2]{DemoObj(1), DemoObj(2)};
    delete[] arr;
}

// ---------------------------------------------------------------------
// 4. 引用、bool 类型
// ---------------------------------------------------------------------
/*
 * C 没有真正的引用（& 只表示取地址）；C++ 有引用语法。
 * C++ 有 bool true/false；C 通常用 int 0/非0 模拟，C99 有 _Bool。
 * C++ 有 nullptr；C 用 NULL 或 (void*)0。
 */
void demo_ref_bool()
{
    cout << "\n--- 引用 / bool ---\n";
    int a = 10;
    int &r = a;       // C++ 引用
    r = 20;
    bool ok = true;   // C++ bool
    cout << "a=" << a << ", ok=" << ok << '\n';
}

// ---------------------------------------------------------------------
// 5. STL 库
// ---------------------------------------------------------------------
/*
 * C 标准库：stdio.h、stdlib.h、string.h、math.h 等，以函数/宏为主。
 * C++ 标准库：C++ 标准库包含 STL（容器、算法、迭代器、仿函数）等，
 *   还提供 string、iostream、智能指针、异常、线程等。
 * 这是 C++ 开发效率高的重要原因。
 */
void demo_stl()
{
    cout << "\n--- STL ---\n";
    vector<int> v{3, 1, 2};
    sort(v.begin(), v.end());
    for (int x : v) cout << x << ' ';
    cout << '\n';
}

// ---------------------------------------------------------------------
// 常用对比表（注释）
// ---------------------------------------------------------------------
/*
 * 特性           C                          C++
 * -------------------------------------------------
 * 编程范式       面向过程                    面向对象 + 泛型 + 函数式
 * 头文件         stdio.h <-> <cstdio>       推荐 <cstdio> / <iostream>
 * 函数重载       不支持                       支持
 * 模板           无                           有
 * bool           C99 _Bool / int 0/非0       bool true/false
 * 字符串         字符数组 + string.h          std::string
 * 内存           malloc/free                  new/delete（更好用智能指针）
 * 引用           无                           有左值/右值引用
 * STL            无                           有 vector/map/algorithm 等
 * 异常           无                           有 try/catch
 */

int main()
{
    cout << "======== 11 C 和 C++ 区别 ========\n";
    demo_overload_template();
    demo_new_vs_malloc();
    demo_ref_bool();
    demo_stl();
    return 0;
}
