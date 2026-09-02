/**
 * =====================================================================
 * C++ 知识点笔记 —— 九、算法竞赛常用 C++ 知识点
 * =====================================================================
 *
 * 目录：
 *  1. 位运算全套：与或异或、移位、lowbit
 *  2. 高精度 __int128
 *  3. 结构体、结构体排序
 *  4. 自定义比较函数：cmp / lambda 给 sort 传参
 *  5. 内存、时间复杂度分析
 *  6. 常见坑：整数溢出、迭代器失效、vector 越界
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

// ---------------------------------------------------------------------
// 1. 位运算全套
// ---------------------------------------------------------------------
/*
 * &  按位与
 * |  按位或
 * ^  按位异或（相同为 0，不同为 1）
 * ~  按位取反
 * << 左移，等价乘 2^k（注意溢出）
 * >> 右移，等价除 2^k（有符号数多数是算术右移）
 *
 * 常用技巧：
 *   x & 1          判断奇偶
 *   x >> k & 1     取出第 k 位
 *   x | (1 << k)   把第 k 位置 1
 *   x & ~(1 << k)  把第 k 位清 0
 *   x ^ (1 << k)   把第 k 位取反
 *   lowbit(x)=x & -x  得到最低位 1 表示的数
 */
void demo_bit_ops()
{
    cout << "\n--- 位运算 ---\n";
    int x = 6;  // 110

    cout << "x&1=" << (x & 1) << '\n';
    cout << "第2位=" << ((x >> 2) & 1) << '\n';

    x |= (1 << 0);       // 110 -> 111
    cout << "置第0位后 x=" << x << '\n';

    x &= ~(1 << 1);      // 111 -> 101 (5)
    cout << "清第1位后 x=" << x << '\n';

    x ^= (1 << 2);       // 101 -> 001 (1)
    cout << "翻转第2位后 x=" << x << '\n';

    // lowbit
    int v = 12;          // 1100
    int low = v & (-v);  // 0100 = 4
    cout << "lowbit(12)=" << low << '\n';

    // 判断 2 的幂
    int p = 16;
    cout << "is_pow2(16)=" << ((p & (p - 1)) == 0) << '\n';

    // 交换两个数（了解即可，实际用 swap）
    int a = 3, b = 5;
    a ^= b; b ^= a; a ^= b;
    cout << "xor swap: a=" << a << ", b=" << b << '\n';
}

// ---------------------------------------------------------------------
// 2. 高精度 __int128
// ---------------------------------------------------------------------
/*
 * __int128 / unsigned __int128 是 GCC/Clang 扩展。
 * 支持乘法/加法/比较，但不能直接用 cin/cout 或 printf/scanf（标准不提供）。
 * 部分 OJ 支持，竞赛中大数要么用 __int128，要么手写高精度 vector<int>。
 */
#if defined(__SIZEOF_INT128__)
using i128 = __int128_t;
using u128 = __uint128_t;

// 简单打印 __int128 的函数
string toString128(i128 x)
{
    if (x == 0) return "0";
    bool neg = x < 0;
    if (neg) x = -x;
    string s;
    while (x > 0) {
        int digit = static_cast<int>(x % 10);
        s.push_back(char('0' + digit));
        x /= 10;
    }
    if (neg) s.push_back('-');
    reverse(s.begin(), s.end());
    return s;
}

void demo_int128()
{
    cout << "\n--- __int128 ---\n";
    i128 a = (i128)123456789 * 123456789;   // 乘法，不会 32 位溢出
    i128 b = (i128)1e18 * 1e18;             // 超过 long long 范围（理论 1e36）
    cout << toString128(a) << '\n';
    cout << toString128(b) << '\n';
}
#endif

// ---------------------------------------------------------------------
// 3. 结构体、结构体排序
// ---------------------------------------------------------------------
/*
 * 结构体是自定义数据类型；在竞赛中常用来组织多个相关属性。
 * 排序时需提供比较规则：operator< 或自定义 cmp/lambda。
 */
struct Student {
    string name;
    int score;
    int id;

    // 定义小于号后，sort 默认会按 score 降序、name 升序等自定义
    bool operator<(const Student &other) const {
        if (score != other.score) return score > other.score; // 分数高在前
        return name < other.name;                              // 姓名升序
    }
};

void demo_struct_sort()
{
    cout << "\n--- 结构体排序 ---\n";
    vector<Student> students = {
        {"Alice", 90, 1},
        {"Bob",   85, 2},
        {"Cindy", 95, 3},
        {"David", 90, 4}
    };

    // 方式一：使用结构体内部 operator<
    sort(students.begin(), students.end());

    for (const auto &s : students)
        cout << s.name << ' ' << s.score << ' ' << s.id << '\n';
}

// ---------------------------------------------------------------------
// 4. 自定义比较函数：cmp / lambda 给 sort 传参
// ---------------------------------------------------------------------
struct Point2D {
    int x, y;
};

// 普通函数 cmp：必须满足严格弱序
bool cmpPoint(const Point2D &a, const Point2D &b)
{
    if (a.x != b.x) return a.x < b.x;   // 按 x 升序
    return a.y < b.y;                   // x 相同按 y 升序
}

void demo_cmp_and_lambda()
{
    cout << "\n--- 自定义比较 ---\n";
    vector<Point2D> pts = {{2, 3}, {1, 5}, {2, 1}, {1, 2}};

    // 普通函数 cmp
    sort(pts.begin(), pts.end(), cmpPoint);
    for (const auto &p : pts) cout << "(" << p.x << "," << p.y << ") ";
    cout << '\n';

    // lambda 比较：例如按 y 降序
    vector<Point2D> pts2 = {{2, 3}, {1, 5}, {2, 1}};
    sort(pts2.begin(), pts2.end(), [](const Point2D &a, const Point2D &b) {
        return a.y > b.y;
    });
    for (const auto &p : pts2) cout << "(" << p.x << "," << p.y << ") ";
    cout << '\n';

    // 给 priority_queue 用 lambda 需要 decltype 声明（了解）
}

// ---------------------------------------------------------------------
// 5. 内存、时间复杂度分析
// ---------------------------------------------------------------------
/*
 * 常用估算：
 *   1 int = 4B，1 long long = 8B
 *   1MB ≈ 2^20 B，10^7 个 int 约 40MB
 *   256MB 约能开 6e7 个 int 或 3e7 个 long long
 * 算法时间复杂度经验：
 *   O(n)        约 1e8 内可跑过（1 秒量级，取决于常数）
 *   O(n log n)  约 1e6 以内常见
 *   O(n^2)      约 5e3 ~ 1e4 需谨慎
 *   O(n^3)      约 500 以内
 * 实际以 OJ 时限/机器为准。
 */
void demo_complexity_notes()
{
    cout << "\n--- 复杂度与内存 ---\n";
    cout << "sizeof(int)=" << sizeof(int) << " sizeof(long long)=" << sizeof(long long) << '\n';

    // 大数组如果放在局部可能栈溢出；大数组建议用全局或 vector 堆区
    // static int big[10000000];   // 全局/静态区可申请较大空间
}

// ---------------------------------------------------------------------
// 6. 常见坑
// ---------------------------------------------------------------------
/*
 * 1. 整数溢出：int 相乘可能爆，运算前转 long long 或直接用 long long。
 * 2. 迭代器失效：vector 扩容、insert/erase 会失效，删除元素用返回值接住。
 * 3. vector 越界：operator[] 不检查，越界是未定义行为；调试可用 at()。
 * 4. 浮点相等判断：不要直接 ==，用 eps。
 * 5. 排序比较函数必须是严格弱序：a<b 为 true 不代表 b<a 为 false 时会出问题，
 *    不能“a <= b”式写法。
 * 6. 除 0、数组开小、字符串输入空格等。
 */
void demo_common_pitfalls()
{
    cout << "\n--- 常见坑 ---\n";

    // 防溢出
    int a = 100000, b = 100000;
    // int c = a * b;           // 可能溢出
    long long c = 1LL * a * b; // 先乘 1LL 提升为 long long
    cout << "safe mul = " << c << '\n';

    // at() 越界会抛异常，适合 debug
    vector<int> v{1, 2, 3};
    try {
        cout << v.at(10) << '\n';
    } catch (const out_of_range &) {
        cout << "at() 越界抛异常\n";
    }

    // 浮点比较
    double x = 0.1 + 0.2;
    double eps = 1e-9;
    cout << "浮点判断: " << (abs(x - 0.3) < eps) << '\n';
}

int main()
{
    cout << "======== 09 算法竞赛常用 C++ 知识点 ========\n";
    demo_bit_ops();
#if defined(__SIZEOF_INT128__)
    demo_int128();
#endif
    demo_struct_sort();
    demo_cmp_and_lambda();
    demo_complexity_notes();
    demo_common_pitfalls();
    return 0;
}
