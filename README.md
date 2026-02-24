# ZACLib

**ZACLib** 是一个用 C++ 实现的 **Aho-Corasick 自动机** 库，用于高效的字符串查找、匹配和替换。适用于多模式匹配场景，例如文本过滤、关键词替换等。

---

## 功能模块

### 1. Replace

实现字符串替换功能：

- **AddRule(from, to)**: 添加替换规则，把 `from` 替换为 `to`  
- **AddReplaceRule(from, to)**: 等同于 `AddRule`  
- **AddRemoveRule(from)**: 删除匹配的字符串  
- **Build()**: 构建自动机  
- **Do(input)**: 执行替换操作，返回替换后的字符串  

示例代码:

```cpp
#include <iostream>
#include "ZACLib.hpp"

int main() {
    ZACLib::Replace replacer;

    // 添加替换规则
    replacer.AddReplaceRule("abc", "123");
    replacer.AddReplaceRule("hello", "hi");
    replacer.AddReplaceRule("world", "earth");

    // 构建自动机
    replacer.Build();

    // 输入文本
    std::string text = "abc hello abc world!";
    std::cout << "原文本: " << text << std::endl;

    // 执行替换
    std::string result = replacer.Do(text);
    std::cout << "替换后: " << result << std::endl;

    return 0;
}
```

### 2. Search

实现字符串匹配功能，返回匹配的位置和长度：

- **AddRule(from)**: 添加匹配规则  
- **Build()**: 构建自动机  
- **Do(input)**: 返回匹配结果 `std::vector<Match>`  

`Match` 结构体:

```cpp
struct Match {
    size_t pos;       // 匹配起始位置
    size_t len;       // 匹配长度
    size_t output_id; // 输出对应规则索引
};
```

示例代码:

```cpp
#include "ZACLib.hpp"
#include <iostream>

int main() {
    ZACLib::Search searcher;

    // 添加匹配规则
    searcher.AddRule("abc");
    searcher.AddRule("hello");

    // 构建自动机
    searcher.Build();

    std::string text = "abc hello abc world!";
    auto matches = searcher.Do(text);

    std::cout << "匹配结果:" << std::endl;
    for (const auto &m : matches) {
        std::cout << "位置: " << m.pos
                  << ", 长度: " << m.len
                  << ", 输出ID: " << m.output_id
                  << ", 文本: " << text.substr(m.pos, m.len)
                  << std::endl;
    }

    return 0;
}
```

### 3. Has

实现字符串是否包含功能（快速判断是否存在匹配）：

- **AddRule(from)**: 添加匹配规则  
- **Build()**: 构建自动机  
- **Do(input)**: 返回 `true` / `false`  

示例代码:

```cpp
#include "ZACLib.hpp"
#include <iostream>

int main() {
    ZACLib::Has haser;

    // 添加匹配规则
    haser.AddRule("abc");
    haser.AddRule("hello");

    // 构建自动机
    haser.Build();

    std::string text = "xyz abc world!";
    bool exists = haser.Do(text);

    std::cout << "是否存在匹配: " << (exists ? "是" : "否") << std::endl;

    return 0;
}
```

---

## 特点

- 使用 **Aho-Corasick 算法**，支持多模式匹配  
- 高效构建自动机，支持快速查找和替换  
- 轻量级 C++ 实现，无额外依赖  
- 支持 ASCII 256 字符集  

---

## 构建

使用 CMake 构建：

```bash
mkdir build
cd build
cmake ..
cmake --build .
···

## 注意事项

- `ZAC_SV` 是自定义的轻量字符串类型，在C++17后为std::string_view，需要注意view视图的生命周期
- 建议在添加完所有规则后调用 `Build()` 构建自动机，避免卡顿
- 匹配规则和替换规则尽量避免重复，以免覆盖错误
- 对于大文本或大量规则，注意内存占用和性能开销
- Replace 模块在替换时会覆盖最长匹配，避免意外覆盖短匹配
