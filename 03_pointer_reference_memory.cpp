/**
 * =====================================================================
 * C++ 知识点笔记 —— 三、指针 & 引用 & 内存
 * =====================================================================
 *
 * 目录：
 *  1. 指针基础：指针变量、取地址 &、解引用 *、nullptr、野指针
 *  2. 引用：左值引用、右值引用 &&、引用与指针区别
 *  3. 内存分区：栈、堆、全局/静态区、常量区；new/delete；内存泄漏
 *  4. 字符串 string
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

// ---------------------------------------------------------------------
// 1. 指针基础
// ---------------------------------------------------------------------
/*
 * 指针存放变量的地址。
 *   int a = 5;
 *   int *p = &a;    // p 保存 a 的地址
 *   *p = 10;        // 解引用：通过地址修改 a
 *
 * 空指针 nullptr：表示指针不指向任何对象，推荐 C++11 后使用 nullptr。
 * 野指针：指向已释放或无效内存的指针，访问会造成未定义行为。
 * 悬垂指针：指向的内存已被释放，需要避免。
 */
void demo_pointer_basic()
{
    int a = 5;
    int *p = &a;                 // 取地址 &
    cout << "a = " << a << ", &a = " << p << '\n';

    *p = 10;                     // 解引用 *
    cout << "after *p = 10, a = " << a << '\n';

    int *np = nullptr;           // 空指针
    if (np == nullptr) {
        cout << "np is nullptr\n";
    }

    // 野指针演示：不要这样写！这里只展示危害
    int *bad;                    // 未初始化：野指针
    // cout << *bad;            // 危险！不要执行
    (void)bad;

    // 指针运算：数组/vector 中常用于遍历（连续内存）
    int arr[3] = {1, 2, 3};
    int *ap = arr;               // 数组名退化为首元素地址
    cout << "pointer arithmetic: ";
    for (int i = 0; i < 3; i++) cout << *(ap + i) << ' ';
    cout << '\n';
}

// ---------------------------------------------------------------------
// 2. 引用
// ---------------------------------------------------------------------
/*
 * 引用是变量的别名，定义时必须初始化，不能重新绑定。
 * 左值引用：T& 绑定到左值（有名字/可取地址）。
 * 右值引用：T&& 绑定到右值（临时对象），主要用于移动语义。
 */
void demo_lvalue_ref()
{
    int a = 10;
    int &ref = a;                // ref 是 a 的别名
    ref = 20;
    cout << "a = " << a << ", ref = " << ref << '\n';
    // int &x; // 错误：引用必须初始化
}

void demo_rvalue_ref()
{
    int a = 1;
    // int &&rr = a;          // 错误：a 是左值，不能绑定到右值引用
    int &&rr = 1;              // OK：字面量是右值
    int &&rr2 = a + 1;         // OK：表达式结果是临时右值
    rr += 10;
    cout << "rvalue ref rr = " << rr << ", rr2 = " << rr2 << '\n';

    string s = "hello";
    string &&t = s + " world"; // 临时 string 被延长生命周期
    cout << t << '\n';
}

/*
 * 引用 vs 指针（面试常问）：
 * 1. 引用必须初始化，指针可以不用初始化。
 * 2. 引用不能改变指向，指针可以改变指向。
 * 3. 没有“空引用”，指针可以有 nullptr。
 * 4. 引用使用更安全、语法更简洁；指针更灵活，但更易出错。
 * 5. 底层实现都有地址，但 C++ 语言层面语义不同。
 */

// ---------------------------------------------------------------------
// 3. 内存分区
// ---------------------------------------------------------------------
/*
 * 典型进程内存布局：
 *   栈区：局部变量、函数参数，自动分配/释放，容量小。
 *   堆区：new/malloc 分配，需手动释放，容量大。
 *   全局/静态区：全局变量、static 变量，程序结束后释放。
 *   常量区：字符串字面量、const 全局常量等，只读。
 *   代码区：存放函数机器码。
 *
 * new / delete：
 *   1. new 分配内存并调用构造函数。
 *   2. delete 调用析构函数并释放内存。
 *   3. new[] / delete[] 配套使用。
 *   4. C 的 malloc/free 不调用构造/析构函数。
 *
 * 内存泄漏：new 了但没 delete，程序长期运行内存持续增加。
 */
int g_global_var = 1;              // 全局/静态区
static int g_static_var = 2;       // 全局/静态区

struct Data {
    int x;
    Data(int v = 0) : x(v) { cout << "Data 构造: " << x << '\n'; }
    ~Data() { cout << "Data 析构: " << x << '\n'; }
};

void demo_memory()
{
    int local = 0;                 // 栈区
    (void)local;

    // 动态数组（对象）单个
    Data *pd = new Data(10);       // 调用构造函数
    delete pd;                     // 调用析构函数并释放

    // new[] / delete[]
    int *arr = new int[10];        // 基本类型数组：没有构造/析构
    arr[0] = 1;
    delete[] arr;

    // new[] 对象数组：默认构造
    Data *parr = new Data[3];      // 需要可默认构造
    delete[] parr;

    // 二维动态数组示例（算法竞赛中一般优先 vector<vector<int>>）
    int rows = 2, cols = 3;
    int **m = new int*[rows];
    for (int i = 0; i < rows; i++) m[i] = new int[cols];
    m[0][1] = 99;
    for (int i = 0; i < rows; i++) delete[] m[i];
    delete[] m;

    cout << "dynamic memory demo done\n";
}

// ---------------------------------------------------------------------
// 4. string 字符串
// ---------------------------------------------------------------------
void demo_string()
{
    cout << "\n--- string 常用操作 ---\n";
    string s1 = "hello";
    string s2 = "world";

    // 拼接
    string s3 = s1 + " " + s2;
    cout << "拼接: " << s3 << '\n';

    // 长度/容量
    cout << "size=" << s3.size() << ", length=" << s3.length() << '\n';

    // 子串
    string sub = s3.substr(0, 5);
    cout << "substr(0,5) = " << sub << '\n';

    // 查找
    size_t pos = s3.find("world");
    if (pos != string::npos) cout << "find world at " << pos << '\n';

    // 替换
    string rep = s3;
    rep.replace(6, 5, "C++");       // 从位置6开始替换5个字符
    cout << "replace: " << rep << '\n';

    // c_str()：返回 C 风格 const char*
    const char *cc = rep.c_str();
    cout << "c_str: " << cc << '\n';

    // 字符串输入输出：
    // cin >> 读到空白停止；getline(cin, str) 读一整行
    // 在需要大量输入时可配合 ios::sync_with_stdio(false) 加速

    // 数字转字符串 / 字符串转数字
    string numStr = to_string(12345);
    int num = stoi("6789");
    cout << "to_string: " << numStr << ", stoi: " << num << '\n';

    // 按字符遍历
    cout << "字符遍历: ";
    for (char c : rep) cout << c;
    cout << '\n';
}

int main()
{
    cout << "======== 03 指针 & 引用 & 内存 ========\n";
    demo_pointer_basic();
    demo_lvalue_ref();
    demo_rvalue_ref();
    demo_memory();
    demo_string();
    return 0;
}
