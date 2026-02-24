#include <iostream>

#include "windows.h"
#include "../../ZACLib/ZACLib_single.hpp"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    static ZACLib::Replace PickupReplacer;
    static bool InitPickupReplacer = [] {
        auto& r = PickupReplacer;

        // ===== 纯删除规则 =====
        r.AddRemoveRule("BP_");
        r.AddRemoveRule("Other_");
        r.AddRemoveRule("Pickup_");     // 改成正确大小写
        r.AddRemoveRule("PickUp_");     // 如果两种都有就都加
        r.AddRemoveRule("_Pickup");
        r.AddRemoveRule("_Wrapper");
        r.AddRemoveRule("WEP_");
        r.AddRemoveRule("_Weapon");

        // ===== 等级 =====
        r.AddReplaceRule("Lv1_B", "Lv1");
        r.AddReplaceRule("Lv2_B", "Lv2");
        r.AddReplaceRule("Lv3_B", "Lv3");

        // ===== 长规则优先 =====
        r.AddReplaceRule("MZJ_QX", "Holographic Sight");
        r.AddReplaceRule("MZJ_HD", "RedDot Sight");
        r.AddReplaceRule("MZJ_CM", "Side Scope");
        r.AddReplaceRule("QT_A", "TacticalStock");
        r.AddReplaceRule("QT_H", "HeavyStock");
        r.AddReplaceRule("GL_A", "AutomaticBolt");
        r.AddReplaceRule("DB_A", "MG-SideShield");

        // ===== 再加短规则 =====
        r.AddReplaceRule("MZJ", "Scope");
        r.AddReplaceRule("QT", "Stock");
        r.AddReplaceRule("QK", "Muzzle");
        r.AddReplaceRule("WB", "Grip");
        r.AddReplaceRule("DJ", "Magazine");
        r.AddReplaceRule("ZDD", "BulletPouch");

        r.AddReplaceRule("Shoulei", "FragGrenade");

        r.Build();
        return true;
    }();

    std::cout << PickupReplacer.Do("BP_PickUp_Shoulei_C_1") << std::endl;

    return 0;
}