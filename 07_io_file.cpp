/**
 * =====================================================================
 * C++ 知识点笔记 —— 七、输入输出 & 文件操作
 * =====================================================================
 *
 * 目录：
 *  1. cin/cout 流式输入输出
 *  2. scanf/printf C 风格输入输出
 *  3. 关闭同步加速 cin
 *  4. ifstream / ofstream / fstream 文件流
 */

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
using namespace std;

// ---------------------------------------------------------------------
// 1. cin / cout
// ---------------------------------------------------------------------
/*
 * cin >> 变量：跳过空白，遇到空白停止。
 * cout << 表达式：可链式输出。
 * 注意：cin/cout 与 C 标准 IO 同步时较慢；算法竞赛常关闭同步加速。
 */
void demo_cin_cout()
{
    int n;
    cout << "请输入一个整数：";   // 运行时输入
    cin >> n;
    cout << "你输入的是: " << n << '\n';

    string s;
    cout << "请输入一个单词：";
    cin >> s;
    cout << "单词: " << s << '\n';

    // 读取整行前，如果前面用过 cin >>，需要先吃掉换行
    cin.ignore(1024, '\n');
    string line;
    cout << "请输入一行文本：";
    getline(cin, line);
    cout << "整行: " << line << '\n';
}

// ---------------------------------------------------------------------
// 2. scanf / printf C 风格
// ---------------------------------------------------------------------
/*
 * scanf 格式：%d int, %lld long long, %lf double, %f float, %s char*
 * printf 格式类似。
 * 优点：速度快、格式化方便；缺点：类型不安全、可读性一般。
 */
void demo_scanf_printf()
{
    int a;
    long long b;
    printf("请输入 int 和 long long：");
    scanf("%d%lld", &a, &b);
    printf("a=%d, b=%lld\n", a, b);
}

// ---------------------------------------------------------------------
// 3. 加速 cin
// ---------------------------------------------------------------------
/*
 * ios::sync_with_stdio(false);
 * cin.tie(nullptr);
 * 两行放 main 开头。
 * 原理：关闭 cin 与 stdio 的同步，让 cin 不再为兼容 scanf 而慢。
 * 注意：关闭后不要混用 cin 和 scanf/printf 处理同一流。
 */
void demo_fast_io_setup()
{
    // 实际使用时放到 main 最前面：
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int x;
    cin >> x;
    cout << x * 2 << '\n';
}

// ---------------------------------------------------------------------
// 4. 文件流
// ---------------------------------------------------------------------
/*
 * ifstream 读文件（in）
 * ofstream 写文件（out，默认覆盖）
 * fstream  读写文件，可指定 ios::in/out/app/trunc 等
 */
void demo_file_io()
{
    const char *path = "demo_file.txt";

    // 写文件
    {
        ofstream fout(path);          // 默认截断写入
        if (!fout) {
            cerr << "文件打开失败\n";
            return;
        }
        fout << "hello\n";
        fout << 123 << ' ' << 3.14 << '\n';
        // fout 离开作用域自动 close
    }

    // 读文件
    {
        ifstream fin(path);
        if (!fin.is_open()) {
            cerr << "读取失败\n";
            return;
        }
        string s;
        int n;
        double d;
        fin >> s >> n >> d;
        cout << "从文件读到: " << s << ' ' << n << ' ' << d << '\n';

        string line;
        // 清空错误状态并回到文件开头演示按行读
        fin.clear();
        fin.seekg(0, ios::beg);
        cout << "按行读：\n";
        while (getline(fin, line)) {
            if (!line.empty()) cout << line << '\n';
        }
    }

    // 删除临时文件（unix 下可用 remove）
    remove(path);
}

int main()
{
    cout << "======== 07 输入输出 & 文件操作 ========\n";
    cout << "1) 请按提示输入（运行才可见）\n";

    // 注意：如果直接运行本程序，前两个函数会等待键盘输入。
    // 可取消注释测试；这里为避免无人值守阻塞，只演示文件流。
    // demo_cin_cout();
    // demo_scanf_printf();
    // demo_fast_io_setup();

    demo_file_io();

    // 说明：OJ/笔试中，最常用的加速写法如下
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}
