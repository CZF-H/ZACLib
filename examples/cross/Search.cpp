//
// Created by wanjiangzhi on 2026/2/24.
//

struct Match {
    size_t pos;       // 匹配起始位置
    size_t len;       // 匹配长度
    size_t output_id; // 输出对应规则索引
};
···

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