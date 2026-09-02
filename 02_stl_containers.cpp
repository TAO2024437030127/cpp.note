/**
 * =====================================================================
 * C++ 知识点笔记 —— 二、STL 容器（算法竞赛高频）
 * =====================================================================
 *
 * 说明：
 *  - STL = Standard Template Library，标准模板库。
 *  - 算法竞赛中优先使用 vector 作为“动态数组”核心容器。
 *  - 本文件按顺序容器、关联容器、容器适配器、迭代器、常用算法演示。
 */

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <algorithm>
#include <string>
using namespace std;

// ---------------------------------------------------------------------
// 1. 顺序容器
// ---------------------------------------------------------------------
/*
 * vector<T>  动态数组：连续内存、随机访问 O(1)、尾部增删均摊 O(1)
 * deque<T>   双端队列：支持头尾 O(1) 插入删除，随机访问 O(1)
 * list<T>    双向链表：任意位置插入删除 O(1)（但已知位置），不支持快速随机访问
 */
void demo_sequence_containers()
{
    cout << "\n--- 顺序容器 ---\n";

    // vector
    vector<int> vec;
    vec.push_back(1);                 // 尾部插入
    vec.push_back(2);
    vec.push_back(3);
    vec.pop_back();                   // 删除尾部
    vec.insert(vec.begin(), 0);       // 头部插入（O(n)）
    vec.erase(vec.begin() + 1);       // 删除指定位置（O(n)）
    cout << "vector size = " << vec.size() << ", front = " << vec.front()
         << ", back = " << vec.back() << '\n';
    cout << "vec[0] = " << vec[0] << ", at(1) = " << vec.at(1) << '\n';

    // 遍历
    for (size_t i = 0; i < vec.size(); i++) cout << vec[i] << ' ';
    cout << '\n';
    for (int x : vec) cout << x << ' ';
    cout << '\n';

    // deque
    deque<int> dq;
    dq.push_back(10);
    dq.push_front(5);                 // 头部插入 O(1)
    cout << "deque: " << dq[0] << ' ' << dq[1] << '\n';

    // list：双向链表
    list<int> li;
    li.push_back(1);
    li.push_front(0);
    li.push_back(2);
    for (int x : li) cout << x << ' ';
    cout << '\n';

    // vector 扩容机制见专题十；简单记：capacity 不够时按约 1.5/2 倍扩容
    cout << "vector capacity=" << vec.capacity() << '\n';
}

// ---------------------------------------------------------------------
// 2. 关联容器
// ---------------------------------------------------------------------
/*
 * set / multiset        有序集合，红黑树实现，插入/删除/查找 O(log n)
 * map / multimap        有序键值对，红黑树实现，按键有序
 * unordered_set/map     哈希实现，平均 O(1)，无序，需要能哈希的类型
 *
 * 注意：
 *  - set 元素唯一，multiset 允许重复。
 *  - map operator[] 若 key 不存在会默认插入一个 value（OJ 中常踩坑）。
 */
void demo_associative_containers()
{
    cout << "\n--- 关联容器 ---\n";

    // set：自动去重并升序
    set<int> s;
    s.insert(3);
    s.insert(1);
    s.insert(2);
    s.insert(1);                      // 重复插入无效
    cout << "set: ";
    for (int x : s) cout << x << ' ';
    cout << '\n';

    // multiset：可重复
    multiset<int> ms;
    ms.insert(3);
    ms.insert(1);
    ms.insert(3);
    cout << "multiset count(3)=" << ms.count(3) << '\n';

    // map：键值对，按键升序
    map<string, int> mp;
    mp["apple"] = 3;
    mp["banana"] = 2;
    mp["apple"]++;                     // apple -> 4
    cout << "map size=" << mp.size() << ", apple=" << mp["apple"] << '\n';
    for (const auto &kv : mp)
        cout << kv.first << ':' << kv.second << ' ';   // apple:4 banana:2
    cout << '\n';

    // multimap：键可重复，没有 operator[]
    multimap<string, int> mmp;
    mmp.insert({"a", 1});
    mmp.insert({"a", 2});
    cout << "multimap count(a)=" << mmp.count("a") << '\n';

    // unordered_map：哈希，平均 O(1)
    unordered_map<string, int> ump;
    ump["x"] = 10;
    ump["y"] = 20;
    cout << "unordered_map size=" << ump.size() << '\n';

    // unordered_set
    unordered_set<int> us;
    us.insert(7);
    us.insert(7);
    us.insert(9);
    cout << "unordered_set count(7)=" << us.count(7) << '\n';
}

// ---------------------------------------------------------------------
// 3. 容器适配器
// ---------------------------------------------------------------------
/*
 * stack<T>        栈：后进先出 LIFO
 * queue<T>        普通队列：先进先出 FIFO
 * priority_queue<T> 优先队列：默认大根堆（堆顶最大）
 *   小根堆：priority_queue<int, vector<int>, greater<int>>
 */
void demo_adapters()
{
    cout << "\n--- 容器适配器 ---\n";

    // stack
    stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    cout << "stack top = " << st.top() << '\n'; // 3
    st.pop();
    cout << "after pop top = " << st.top() << '\n'; // 2

    // queue
    queue<int> q;
    q.push(1);
    q.push(2);
    cout << "queue front = " << q.front() << ", back = " << q.back() << '\n';
    q.pop();
    cout << "after pop front = " << q.front() << '\n';

    // priority_queue 默认大根堆
    priority_queue<int> pq;
    pq.push(5);
    pq.push(1);
    pq.push(9);
    cout << "priority_queue top = " << pq.top() << '\n'; // 9

    // 小根堆
    priority_queue<int, vector<int>, greater<int>> minq;
    minq.push(5);
    minq.push(1);
    minq.push(9);
    cout << "min heap top = " << minq.top() << '\n'; // 1
}

// ---------------------------------------------------------------------
// 4. 通用迭代器
// ---------------------------------------------------------------------
/*
 * 迭代器可理解为“容器的指针”，通过 *it 访问元素，it++ 移动。
 * begin() end() rbegin() rend()
 *  - rbegin/rend 反向迭代。
 *  - const_iterator 只读。
 *  - 迭代器失效：vector 扩容会使所有迭代器失效；
 *    erase 会使被删位置及其后的迭代器失效（list 除外）；
 *    unordered_map 的 rehash 会使迭代器失效。
 */
void demo_iterators()
{
    cout << "\n--- 迭代器 ---\n";
    vector<int> v{1, 2, 3, 4, 5};

    cout << "正向: ";
    for (auto it = v.begin(); it != v.end(); ++it) cout << *it << ' ';
    cout << '\n';

    cout << "反向: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it) cout << *it << ' ';
    cout << '\n';

    // 删除偶数：注意 erase 返回下一个有效迭代器，避免失效
    for (auto it = v.begin(); it != v.end(); ) {
        if (*it % 2 == 0) it = v.erase(it);
        else ++it;
    }
    cout << "删除偶数后: ";
    for (int x : v) cout << x << ' ';
    cout << '\n';
}

// ---------------------------------------------------------------------
// 5. STL 常用算法 <algorithm>
// ---------------------------------------------------------------------
void demo_algorithms()
{
    cout << "\n--- 常用算法 ---\n";

    vector<int> v{4, 1, 3, 3, 2, 5};

    // sort 升序排序
    sort(v.begin(), v.end());
    cout << "sort: ";
    for (int x : v) cout << x << ' ';
    cout << '\n';

    // sort 降序排序
    sort(v.begin(), v.end(), greater<int>());
    cout << "sort greater: ";
    for (int x : v) cout << x << ' ';
    cout << '\n';

    // reverse 反转
    reverse(v.begin(), v.end());
    cout << "reverse: ";
    for (int x : v) cout << x << ' ';
    cout << '\n';

    // find 查找（O(n)）
    auto it = find(v.begin(), v.end(), 3);
    if (it != v.end()) cout << "find 3 at index " << (it - v.begin()) << '\n';

    // binary_search 二分查找（要求有序）
    sort(v.begin(), v.end());          // 1 2 3 3 4 5
    cout << "binary_search(3) = " << binary_search(v.begin(), v.end(), 3) << '\n';

    // lower_bound: >= x 的第一个位置
    // upper_bound: > x 的第一个位置
    auto low = lower_bound(v.begin(), v.end(), 3);
    auto up  = upper_bound(v.begin(), v.end(), 3);
    cout << "lower_bound(3) index=" << (low - v.begin())
         << ", upper_bound(3) index=" << (up - v.begin()) << '\n'; // 2 和 4

    // max_element / min_element
    auto mx = max_element(v.begin(), v.end());
    auto mn = min_element(v.begin(), v.end());
    cout << "max=" << *mx << ", min=" << *mn << '\n';

    // unique 去重（通常先排序；返回去重后新末尾）
    vector<int> u{1, 1, 2, 2, 3, 3, 3};
    auto end = unique(u.begin(), u.end());
    u.erase(end, u.end());
    cout << "unique: ";
    for (int x : u) cout << x << ' ';
    cout << '\n';

    // swap 交换
    int a = 1, b = 2;
    swap(a, b);
    cout << "swap: a=" << a << ", b=" << b << '\n';
}

int main()
{
    cout << "======== 02 STL 容器与算法 ========\n";
    demo_sequence_containers();
    demo_associative_containers();
    demo_adapters();
    demo_iterators();
    demo_algorithms();
    return 0;
}
