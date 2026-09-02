# C++ 知识点学习笔记（示范代码）

> 面向算法竞赛 / 考研复试 / 笔试面试，重点突出，可直接用来复盘刷题。
> 每个 `.cpp` 文件既是学习笔记（大量中文注释），也是可编译运行的示范代码。
> 代码优先使用 `vector` 容器。

## 文件与大纲对应

| 编号 | 文件 | 大纲章节 |
| ---- | ---- | ---- |
| 01 | `01_basic_syntax.cpp` | 一、C++ 基础语法 |
| 02 | `02_stl_containers.cpp` | 二、STL 容器 |
| 03 | `03_pointer_reference_memory.cpp` | 三、指针 & 引用 & 内存 |
| 04 | `04_oop.cpp` | 四、面向对象 OOP |
| 05 | `05_template_generic.cpp` | 五、模板与泛型编程 |
| 06 | `06_cpp11_features.cpp` | 六、C++11 及以后核心特性 |
| 07 | `07_io_file.cpp` | 七、输入输出 & 文件操作 |
| 08 | `08_exception.cpp` | 八、异常处理 |
| 09 | `09_algorithm_common.cpp` | 九、算法竞赛常用 C++ 知识点 |
| 10 | `10_low_level_interview.cpp` | 十、底层 & 面试考点 |
| 11 | `11_c_vs_cpp.cpp` | 十一、C 和 C++ 区别 |

## 编译运行示例

```bash
# Linux / macOS / Git Bash / Windows WSL 等
g++ -std=c++17 -Wall -Wextra -o 01 01_basic_syntax.cpp && ./01

# 所有文件逐个编译检查
for f in *.cpp; do
  g++ -std=c++17 -Wall -Wextra -fsyntax-only "$f"
done
```

> 说明：07 文件中的 `cin/cout`、`scanf/printf` 需要交互输入，默认已注释演示调用；
> 08 文件中 `getAge()` 会等待输入。
