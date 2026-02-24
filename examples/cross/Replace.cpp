//
// Created by wanjiangzhi on 2026/2/24.
//

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