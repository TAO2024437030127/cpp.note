/**
 * =====================================================================
 * C++ 知识点笔记 —— 一、C++ 基础语法
 * 适用：算法竞赛 / 考研复试 / 笔试面试
 * 说明：代码优先使用 vector；本文件为语法速查示例，可直接编译运行。
 * =====================================================================
 *
 * 学习目标：
 *  1. 基础数据类型
 *  2. 变量、常量、限定符
 *  3. 运算符与表达式
 *  4. 流程控制
 *  5. 函数
 *  6. 命名空间
 */

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

// ---------------------------------------------------------------------
// 1. 基础数据类型
// ---------------------------------------------------------------------
/*
 * 整型：
 *   int            通常 4 字节，约 [-2^31, 2^31-1]
 *   long long      通常 8 字节，约 [-2^63, 2^63-1]
 *   unsigned long long  0 ~ 2^64-1
 *   __int128       GCC/Clang 扩展，128 位整数，部分 OJ 可用
 *
 * 浮点：
 *   float          单精度，约 6~7 位有效数字
 *   double         双精度，约 15~16 位有效数字（算法竞赛常用）
 *   long double    更高精度，但运算较慢，慎用
 *
 * 字符 / 布尔 / void：
 *   char           1 字节，可存 ASCII
 *   bool           true / false，内存通常 1 字节
 *   void           “无类型”，用于函数返回值或泛型指针 void*
 */

void demo_basic_types()
{
    // 整型
    int a = 100;
    long long b = 123456789012345LL;       // 数字后加 LL 表示 long long
    unsigned long long c = 18446744073709551615ULL;
    (void)b; (void)c;
    // __int128 d = (__int128)1e36;        // 可直接定义，但不能直接 cout 输出

    // 浮点
    float f = 3.14f;                       // 单精度最好加 f 后缀
    double d = 3.14159265358979;
    long double ld = 3.14159265358979323846L;
    (void)f; (void)d; (void)ld;

    // 字符 / 布尔
    char ch = 'A';
    bool flag = true;

    cout << "int sizeof = " << sizeof(int) << " bytes\n";
    cout << "long long sizeof = " << sizeof(long long) << " bytes\n";
    cout << "bool sizeof = " << sizeof(bool) << " bytes\n";

    // 类型转换
    // (1) 隐式转换：低精度 -> 高精度，有时会丢失信息
    double x = a / 3;                      // 注意：a/3 先做整数除法，结果是 33
    double y = a / 3.0;                    // 希望得到小数，要有一个数为浮点
    (void)x; (void)y;

    // (2) static_cast：编译期安全转换，常见
    int ci = static_cast<int>(3.99);       // 结果是 3，截断
    double cd = static_cast<double>(a) / 3;

    // (3) const_cast：去掉 const / 加上 const
    const int cval = 10;
    // const_cast 多用于调用旧接口；正常代码不要修改真正的常量对象
    int *p = const_cast<int *>(&cval);     // 语法示例，一般不这样写
    (void)p;

    // (4) reinterpret_cast：低层重新解释，二进制位不变，慎用
    unsigned long long addr = reinterpret_cast<unsigned long long>(&a);

    cout << ci << ' ' << cd << ' ' << flag << ' ' << ch << ' ' << addr % 1000 << '\n';
}

// ---------------------------------------------------------------------
// 2. 变量、常量、限定符
// ---------------------------------------------------------------------

const int GLOBAL_CONST = 42;               // 全局常量，常量区/编译期常量
static int g_static = 0;                   // static 全局变量：仅本文件可见

constexpr int constexpr_val() { return 64; }

constexpr int square(int n) { return n * n; } // constexpr：编译期求值

void demo_qualifier()
{
    // const：不可修改
    const int N = 100;
    (void)N;
    // N = 200; // 错误：不能修改 const 变量

    // constexpr：编译期常量，可用于数组大小等
    constexpr int SIZE = 100;
    int data[SIZE];                        // C++11 起 constexpr 可作数组长度
    (void)data;

    // auto：自动推导类型，必须初始化
    auto num = 10;                         // int
    auto pi = 3.14;                        // double
    auto v = vector<int>{1, 2, 3};         // vector<int>
    cout << "auto: " << num << ' ' << pi << ' ' << v.size() << '\n';

    // decltype：用表达式类型来声明变量，不实际求值
    decltype(num) num2 = num;              // num2 也是 int
    cout << "decltype: " << num2 << '\n';
    cout << "constexpr square(8) = " << square(8) << '\n';
}

// 作用域演示：局部变量、全局变量、static 静态局部变量
int global_v = 1;

void scope_demo()
{
    static int call_count = 0;             // 静态局部变量：只初始化一次，函数结束后不销毁
    call_count++;
    cout << "scope_demo 第 " << call_count << " 次调用, global_v=" << global_v << '\n';
    global_v++;
}

// ---------------------------------------------------------------------
// 3. 运算符与表达式
// ---------------------------------------------------------------------
/*
 * 算术：+ - * / %
 * 逻辑：&& || !
 * 位运算：& | ^ ~ << >>（详细见专题文件）
 * 三目：cond ? a : b
 * 自增/自减：i++ 先用后加，++i 先加后用
 * 优先级：算术 > 关系 > 逻辑；不确定就加括号
 */
void demo_operators()
{
    int a = 5, b = 2;
    cout << (a + b) << ' ' << (a - b) << ' ' << (a * b) << ' '
         << (a / b) << ' ' << (a % b) << '\n';   // 7 3 10 2 1

    bool logic = (a > b) && (b != 0);
    cout << boolalpha << logic << noboolalpha << '\n';

    // 位运算
    cout << (5 & 3) << ' ' << (5 | 3) << ' ' << (5 ^ 3) << ' '
         << (1 << 4) << ' ' << (16 >> 2) << '\n'; // 1 7 6 16 4

    // 三目运算符
    int maxv = a > b ? a : b;
    cout << "max=" << maxv << '\n';

    // 自增自减：注意副作用
    int i = 1;
    int t1 = i++;       // t1=1, i=2
    int t2 = ++i;       // i=3, t2=3
    cout << t1 << ' ' << t2 << ' ' << i << '\n';
}

// ---------------------------------------------------------------------
// 4. 流程控制
// ---------------------------------------------------------------------
void demo_flow()
{
    // if-else
    int score = 85;
    if (score >= 90)      cout << "A\n";
    else if (score >= 80) cout << "B\n";
    else                  cout << "C\n";

    // switch
    int x = 2;
    switch (x) {
        case 1: cout << "one\n"; break;
        case 2: cout << "two\n"; break;
        default: cout << "other\n";
    }

    // for
    for (int i = 0; i < 3; i++) cout << i << ' ';
    cout << '\n';

    // while
    int j = 3;
    while (j > 0) { cout << "while "; j--; }
    cout << '\n';

    // do-while：至少执行一次
    int k = 0;
    do { cout << "do-while "; k++; } while (k < 2);
    cout << '\n';

    // break / continue
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) continue;   // 跳过偶数
        if (i > 5) break;           // i=7 时停止
        cout << i << ' ';           // 1 3 5
    }
    cout << '\n';

    // goto：嵌套深时偶尔能快速跳出；不推荐滥用
    for (int i = 0; i < 3; i++)
        for (int j2 = 0; j2 < 3; j2++) {
            if (i == 1 && j2 == 1) goto out;
        }
out:
    cout << "goto out\n";
}

// ---------------------------------------------------------------------
// 5. 函数
// ---------------------------------------------------------------------
// 函数声明（可放在调用处之前；定义也可以）
int add(int a, int b);

// 函数定义 + 默认参数（默认参数通常写在声明/定义处一次）
void print_sum(int a, int b = 10)
{
    cout << "sum = " << a + b << '\n';
}

// 值传递：拷贝实参，内部修改不影响外部
void change_value(int x) { (void)x; x = 100; }

// 引用传递：直接操作实参，推荐用来减少拷贝
void change_ref(int &x) { x = 100; }

// 指针传递：传地址，也能修改原变量
void change_ptr(int *x) { if (x) *x = 200; }

// 函数重载：函数名相同、参数列表不同（不能仅按返回值区分）
void show(int x) { cout << "show(int): " << x << '\n'; }
void show(double x) { cout << "show(double): " << x << '\n'; }
void show(int a, int b) { cout << "show(int,int): " << a << ',' << b << '\n'; }

// inline 内联函数：建议短小函数，减少调用开销（编译器可能不采用）
inline int mul(int a, int b) { return a * b; }

// 递归：自己调用自己，必须有终止条件
long long fib(int n)
{
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int add(int a, int b) { return a + b; }

void demo_function()
{
    cout << add(1, 2) << '\n';
    print_sum(3);            // 默认参数生效：b=10
    print_sum(3, 5);

    int v = 1;
    change_value(v);         // v 不变
    change_ref(v);           // v = 100
    change_ptr(&v);          // v = 200
    cout << "v = " << v << '\n';

    show(1);
    show(1.5);
    show(1, 2);

    cout << "inline mul: " << mul(3, 4) << '\n';
    cout << "fib(10) = " << fib(10) << '\n';
}

// ---------------------------------------------------------------------
// 6. 命名空间 namespace
// ---------------------------------------------------------------------
/*
 * std 是 C++ 标准库命名空间。
 * using namespace std; 方便但会把大量名字引入当前作用域，
 * 在大型项目/面试中常提到：可能造成命名冲突，笔试小代码常用。
 * 建议：小练习 using namespace std;，工程中改用 using std::cout; 等。
 */

namespace my_space {
    int value = 99;
    void hello() { cout << "hello from my_space\n"; }
}

namespace my_space {
    // 命名空间可以分块、重复打开
    void world() { cout << "world\n"; }
}

void demo_namespace()
{
    cout << my_space::value << '\n';
    my_space::hello();
    my_space::world();

    // using 声明：只引入一个名字
    using std::vector;
    vector<int> v{1, 2, 3};
    cout << "using std::vector: " << v.size() << '\n';
}

// main 函数：演示以上知识点
int main()
{
    cout << "======== 01 C++ 基础语法 ========\n";
    demo_basic_types();
    demo_qualifier();
    scope_demo();
    scope_demo();
    demo_operators();
    demo_flow();
    demo_function();
    demo_namespace();
    return 0;
}
