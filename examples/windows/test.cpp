#include <iostream>

/*#include "windows.h"*/
#include "../../ZACLib/Deprecated/ZACLib_single.hpp"
#include <unordered_map>
#include <string>
#include <iostream>


int main() {
    /*SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);*/

    ZACLib::Search GrenadeSearcher;

    // 添加规则
    GrenadeSearcher.AddRule("FragGrenade");
    GrenadeSearcher.AddRule("SmokeBomb");
    GrenadeSearcher.AddRule("BurnGrenade");
    GrenadeSearcher.AddRule("StunGrenade");

    // 构建 Trie
    GrenadeSearcher.Build();

    std::vector<std::string> testClasses = {"BP_Projectile_FragGrenade_C_2", "SmokeBombY", "RecycledGrenade"};
    ZACLib::ZAC_SV input;

    const std::unordered_map<std::string, std::string> GrenadeType = {
        {"FragGrenade", "Grenade.FragGrenade"},
        {"SmokeBomb", "Grenade.SmokeBomb"},
        {"BurnGrenade", "Grenade.BurnGrenade"},
        {"StunGrenade", "Grenade.StunGrenade"},
    };

    for (auto& ClassName : testClasses) {
        if (ClassName.find("Recycled") != std::string::npos) continue;

        std::string GrenadeName = "Grenade.Default";

        // 使用 ZACLib::Search
        auto matches = GrenadeSearcher.Do(ClassName);
        for (auto &m : matches) {
            auto key = ClassName.substr(m.pos, m.len);
            auto it = GrenadeType.find(key);
            if (it != GrenadeType.end()) {
                GrenadeName = it->second;
                break;
            }
        }

        std::cout << ClassName << " -> " << GrenadeName << "\n";
    }

    return 0;
}