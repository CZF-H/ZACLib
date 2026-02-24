//
// Created by wanjiangzhi on 2026/2/24.
//

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
