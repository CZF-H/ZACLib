#include <iostream>
#include <windows.h>
#include "../../ZACLib/ZACLib.hpp"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    using namespace ZACLib;

    Replace replacer;
    replacer.AddReplaceRule("1", "一");
    replacer.AddReplaceRule("2", "二");
    replacer.AddRemoveRule("_");
    replacer.Build();

    const std::string name = "1221_2112";
    const std::string replaced = replacer.Do(name);
    std::cout << "原始: " << name << "\n";
    std::cout << "替换后: " << replaced << "\n\n";

    Search searcher;
    searcher.AddRule("二");
    searcher.AddRule("三");
    searcher.Build();

    const std::string& text = "一一二二三三";
    const auto matches = searcher.Do(text);
    std::cout << "Search 匹配结果:\n";
    for (auto &m : matches) {
        std::cout << "匹配: " << text.substr(m.pos, m.len)
                  << " 位置: " << m.pos
                  << " 规则ID: " << m.ruleId << "\n";
    }
    std::cout << "\n";

    Has hasCheck;
    hasCheck.AddRule("一");
    hasCheck.AddRule("三");
    hasCheck.Build();

    std::cout << "Has 检查:\n";
    std::cout << "是否包含 '一' 或 '三'? "
              << (hasCheck.Do("一三") ? "是" : "否") << "\n";

    std::cout <<  "是否包含 '四'? "
              << (hasCheck.Do("四") ? "是" : "否") << "\n";

    return 0;
}
