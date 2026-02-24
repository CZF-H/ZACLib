//
// Created by wanjiangzhi on 2026/2/24.
//

// ReSharper disable CppNonExplicitConvertingConstructor
#ifndef ZACLIB_HPP
#define ZACLIB_HPP

#include <array>
#include <vector>
#include <string>
#include <cstring>

namespace ZACLib {

    #if __cplusplus >= 201703L
    #include <string_view>
    using ZAC_SV = std::string_view;
    #else
    struct ZAC_SV {
        const char* data;
        size_t size;

        ZAC_SV() : data(nullptr),
                   size(0) {}

        ZAC_SV(const char* d, const size_t s) : data(d),
                                          size(s) {}

        ZAC_SV(const std::string& s) : data(s.c_str()),
                                       size(s.size()) {}

        ZAC_SV(const char* d) : data(d),
                                size(d ? std::strlen(d) : 0) {}
        const char& operator[](const size_t i) const { return data[i]; }
    };
    #endif

    struct Node {
        std::array<int, 256> next{};
        int fail;
        int output_id;
        size_t match_len;

        Node() : fail(0),
                 output_id(-1),
                 match_len(0) { next.fill(-1); }
    };

    class Replace {
    public:
        Replace();
        void AddRule(const ZAC_SV& from);
        void AddRule(const ZAC_SV& from, const ZAC_SV& to);
        void AddReplaceRule(const ZAC_SV& from, const ZAC_SV& to);
        void AddRemoveRule(const ZAC_SV& from);
        void Build();
        std::string Do(const ZAC_SV& input) const;

    private:
        std::vector<Node> trie;
        std::vector<std::string> outputs;
    };

    class Search {
    public:
        struct Match {
            size_t pos;
            size_t len;
            size_t ruleId;
        };

        Search();
        void AddRule(const ZAC_SV& from);
        void Build();
        std::vector<Match> Do(const ZAC_SV& input) const;

    private:
        std::vector<Node> trie;
        std::vector<std::string> outputs;
    };

    class Has {
    public:
        Has();
        void AddRule(const ZAC_SV& from);
        void Build();
        bool Do(const ZAC_SV& input) const;

    private:
        std::vector<Node> trie;
    };

} // namespace ZACLIB

#endif // ZACLIB_HPP