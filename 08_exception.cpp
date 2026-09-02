/**
 * =====================================================================
 * C++ 知识点笔记 —— 八、异常处理
 * =====================================================================
 *
 * 机制：try-catch-throw
 *   - throw 抛出一个异常对象（可以是 int/string/自定义类型/标准异常）。
 *   - catch 按类型捕获异常，可写多个 catch。
 *   - 如果异常没有被捕获，程序会调用 std::terminate。
 * 注意事项：
 *   - 不要用异常做普通流程控制。
 *   - 构造函数中抛异常时析构函数不会执行，要注意资源管理。
 *   - 现代 C++ 推荐使用智能指针，减少裸 new 导致异常时资源泄漏。
 */

#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

// 自定义异常类型：继承 std::exception 可被标准 catch(...) 体系处理
class MyException : public exception {
    string msg;
public:
    explicit MyException(string m) : msg(m) {}
    const char *what() const noexcept override {
        return msg.c_str();
    }
};

double divide(double a, double b)
{
    if (b == 0) {
        // 抛出标准库异常
        throw runtime_error("除数不能为 0");
    }
    return a / b;
}

int getAge()
{
    int age;
    cout << "输入年龄：";
    cin >> age;
    if (age < 0 || age > 150) {
        throw MyException("年龄不合法");
    }
    return age;
}

void demo_try_catch()
{
    try {
        double r = divide(10, 0);      // 会抛 runtime_error
        cout << r << '\n';
    } catch (const runtime_error &e) {
        cout << "捕获 runtime_error: " << e.what() << '\n';
    }

    try {
        int age = getAge();
        cout << "年龄 = " << age << '\n';
    } catch (const MyException &e) {
        cout << "捕获 MyException: " << e.what() << '\n';
    } catch (...) {
        // 捕获所有异常；通常用于兜底
        cout << "未知异常\n";
    }

    // 嵌套 try 示例
    try {
        try {
            throw string("inner error");
        } catch (const string &s) {
            cout << "内层捕获: " << s << '\n';
            throw;                  // 重新抛出，交给外层
        }
    } catch (const string &s) {
        cout << "外层捕获: " << s << '\n';
    }
}

// 如果 try 块内需要管理裸资源，请用 RAII/智能指针
void demo_no_leak_when_throw()
{
    // 若使用裸 new，在 new 和 delete 之间抛异常会泄漏
    // 推荐智能指针：
    // auto p = make_shared<int>(1);
    // throw ... 也不泄漏，因为 shared_ptr 析构会释放
    cout << "异常机制与 RAII 结合可避免资源泄漏\n";
}

int main()
{
    cout << "======== 08 异常处理 ========\n";
    demo_try_catch();
    demo_no_leak_when_throw();
    return 0;
}
