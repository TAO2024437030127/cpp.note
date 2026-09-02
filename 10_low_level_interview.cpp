/**
 * =====================================================================
 * C++ 知识点笔记 —— 十、底层 & 面试考点
 * =====================================================================
 *
 * 目录：
 *  1. 虚函数表原理
 *  2. 多态底层实现
 *  3. 智能指针底层原理
 *  4. STL 容器底层：vector 扩容、unordered_map 哈希冲突
 *  5. 编译链接过程：预处理-编译-汇编-链接
 *  6. 头文件保护 #ifndef / #define / #endif
 */

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
using namespace std;

// ---------------------------------------------------------------------
// 1. 虚函数表原理
// ---------------------------------------------------------------------
/*
 * 每个包含虚函数的类都会有一个虚函数表（vtable），
 * 表中按声明顺序保存虚函数地址。
 * 每个对象内部有一个虚表指针（vptr），指向所属类的 vtable。
 *
 * 构造过程：
 *   基类构造时 vptr 指向基类 vtable；
 *   派生类构造时 vptr 更新为派生类 vtable。
 *
 * 面试常问：
 *   - 虚函数会增加 4/8 字节（vptr）。
 *   - 纯虚函数类也有 vtable。
 *   - 构造函数不能是虚函数；析构函数建议是虚函数。
 */
class VBase {
public:
    virtual void f1() { cout << "VBase::f1\n"; }
    virtual void f2() { cout << "VBase::f2\n"; }
};

class VDerived : public VBase {
public:
    void f1() override { cout << "VDerived::f1\n"; }
    void f2() override { cout << "VDerived::f2\n"; }
};

void demo_vptr()
{
    cout << "\n--- 虚函数表概念 ---\n";
    VBase b;
    VDerived d;
    VBase *p = &d;   // 运行时通过 vptr 找到 VDerived::f1
    p->f1();
    (void)b;
    // 实际看 vptr 可打印对象地址，但不建议手动操作虚表，这里只做概念说明
}

// ---------------------------------------------------------------------
// 2. 多态底层实现
// ---------------------------------------------------------------------
/*
 * 动态多态 = 虚函数 + 继承 + 基类指针/引用调用。
 * 编译时不能确定最终调用哪个函数，运行时根据对象 vptr 查找 vtable，
 * 再取出对应函数指针调用，这就是动态绑定。
 *
 * 静态绑定：普通函数重载、非虚函数在编译期确定。
 */
class NonVirtual {
public:
    void f() { cout << "NonVirtual::f\n"; }
};

class NonVirtualDerived : public NonVirtual {
public:
    void f() { cout << "NonVirtualDerived::f\n"; }
};

void demo_dynamic_binding()
{
    cout << "\n--- 动态绑定 vs 静态绑定 ---\n";
    VDerived d;
    VBase &rb = d;        // 虚函数：动态绑定
    rb.f1();              // VDerived::f1

    NonVirtualDerived nd;
    NonVirtual &rn = nd;  // 非虚：静态绑定
    rn.f();               // NonVirtual::f
}

// ---------------------------------------------------------------------
// 3. 智能指针底层原理
// ---------------------------------------------------------------------
/*
 * unique_ptr 底层：
 *   独占裸指针，禁止拷贝，只有移动。析构时 delete。
 *
 * shared_ptr 底层：
 *   对象 + 控制块（强引用计数、弱引用计数、删除器等）。
 *   拷贝构造：strong_count++；析构：strong_count--，为 0 时 delete 对象。
 *   线程安全：引用计数本身原子操作；但指向的对象不保证线程安全。
 *
 * weak_ptr 底层：
 *   观察 shared_ptr，弱引用计数 +1 不拥有对象。
 *   通过 lock() 提升为 shared_ptr，若对象已释放则返回空。
 */
void demo_smart_ptr_impl_notes()
{
    cout << "\n--- 智能指针底层 ---\n";
    shared_ptr<int> sp = make_shared<int>(42);
    weak_ptr<int> wp = sp;          // 弱引用计数增加，强引用计数不变
    cout << "use_count=" << sp.use_count() << '\n';
    if (auto locked = wp.lock()) {  // 安全获取
        cout << "lock: " << *locked << '\n';
    }
}

// ---------------------------------------------------------------------
// 4. STL 容器底层
// ---------------------------------------------------------------------
/*
 * vector 底层：
 *   - 连续内存的数组；size 表示元素数，capacity 表示容量。
 *   - push_back 时 capacity 不够，会重新分配新内存（常为 1.5/2 倍），
 *     拷贝/移动旧元素到新内存，再释放旧内存。
 *   - reserve() 可提前扩容减少多次拷贝。
 *
 * unordered_map 底层：
 *   - 哈希表（bucket array）。
 *   - 哈希冲突常见解决：开链法（拉链法），每个桶挂链表或红黑树；
 *     当负载因子过大时 rehash，重新分配桶数组。
 *   - 平均 O(1)，最坏退化为 O(n)。
 */
void demo_vector_capacity()
{
    cout << "\n--- vector 扩容 ---\n";
    vector<int> v;
    size_t lastCap = 0;
    for (int i = 0; i < 20; i++) {
        v.push_back(i);
        if (v.capacity() != lastCap) {
            cout << "size=" << v.size() << " capacity=" << v.capacity()
                 << " (扩容约 " << v.capacity() << ")\n";
            lastCap = v.capacity();
        }
    }
    v.reserve(1000);
    cout << "reserve(1000) capacity=" << v.capacity() << '\n';
}

void demo_unordered_map_notes()
{
    cout << "\n--- unordered_map 概念 ---\n";
    unordered_map<string, int> mp;
    for (int i = 0; i < 10; i++) mp["key" + to_string(i)] = i;
    cout << "bucket_count=" << mp.bucket_count()
         << ", load_factor=" << mp.load_factor() << '\n';
    mp.reserve(1000);   // 预留桶，减少 rehash
    cout << "after reserve bucket_count=" << mp.bucket_count() << '\n';
}

// ---------------------------------------------------------------------
// 5. 编译链接过程
// ---------------------------------------------------------------------
/*
 * 1. 预处理：展开 #include、宏替换、条件编译，生成 .i
 * 2. 编译：把 C++ 翻译成汇编代码，生成 .s
 * 3. 汇编：把汇编转成机器码，生成 .o / .obj
 * 4. 链接：把多个 .o 和库合并为可执行文件；处理符号解析、重定位
 *
 * 常用命令：
 *   g++ -E main.cpp -o main.i
 *   g++ -S main.cpp -o main.s
 *   g++ -c main.cpp -o main.o
 *   g++ main.o -o main
 */
void demo_compile_link_notes()
{
    cout << "\n--- 编译链接 ---\n";
    cout << "见注释：预处理->编译->汇编->链接\n";
}

// ---------------------------------------------------------------------
// 6. 头文件保护
// ---------------------------------------------------------------------
/*
 * 防止同一个头文件被多次包含。
 *
 * #ifndef _MY_HEADER_H
 * #define _MY_HEADER_H
 * ...
 * #endif
 *
 * 也可以用 #pragma once（非标准但大多数编译器支持）。
 * 下面在 .cpp 中展示宏，头文件中应放在文件最外层。
 */
#ifndef _CPP_NOTE_DEMO_H
#define _CPP_NOTE_DEMO_H
void demo_header_guard_notes()
{
    cout << "\n--- 头文件保护 ---\n";
    cout << "应在 .h 文件顶部写 #ifndef/#define/#endif 或 #pragma once\n";
}
#endif // _CPP_NOTE_DEMO_H

int main()
{
    cout << "======== 10 底层 & 面试考点 ========\n";
    demo_vptr();
    demo_dynamic_binding();
    demo_smart_ptr_impl_notes();
    demo_vector_capacity();
    demo_unordered_map_notes();
    demo_compile_link_notes();
    demo_header_guard_notes();
    return 0;
}
