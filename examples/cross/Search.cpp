//
// Created by wanjiangzhi on 2026/2/24.
//

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