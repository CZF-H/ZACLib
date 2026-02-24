//
// Created by wanjiangzhi on 2026/2/24.
//

// ReSharper disable CppNonExplicitConvertingConstructor
#ifndef ZACLIB_SINGLE_HPP
#define ZACLIB_SINGLE_HPP

#ifndef ZACLIB_HPP
#define ZACLIB_HPP

#include <vector>
#include <queue>

// 以下文件使用其他文件的头文件保护是为了放在include ZACLib_Types.hpp后又include此文件导致重复定义问题
// ZACLib_Types.hpp
//
// Created by wanjiangzhi on 2026/2/24.
//

// ReSharper disable CppNonExplicitConvertingConstructor
#ifndef ZACLIB_TOOLS_HPP
#define ZACLIB_TOOLS_HPP

#if __cplusplus >= 201703L
    #include <string_view>
#else
    #include <cstring>
#endif

#include <string>
#include <array>


namespace ZACLib {
    #if __cplusplus >= 201703L
    using ZAC_SV = std::string_view;
    #else
    class ZAC_SV {
        const char* m_data;
        const size_t m_size;
    public:
        ZAC_SV() : m_data(nullptr),
                   m_size(0) {}

        ZAC_SV(const char* d, const size_t s) : m_data(d),
                                          m_size(s) {}

        ZAC_SV(const std::string& s) : m_data(s.c_str()),
                                       m_size(s.size()) {} // 模仿std::string_view，不禁止隐式构造

        ZAC_SV(const char* d) : m_data(d),
                                m_size(d ? std::strlen(d) : 0) {} // 模仿std::string_view，不禁止隐式构造

        const char* data() const noexcept {
            return m_data;
        }

        std::size_t size() const noexcept {
            return m_size;
        }

        bool empty() const noexcept {
            return m_size == 0;
        }

        const char* begin() const { return m_data; }
        const char* end() const { return m_data + m_size; }
        const char* cbegin() const { return m_data; }
        const char* cend() const { return m_data + m_size; }

        const char& operator[](const size_t i) const { return m_data[i]; }
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
}

#endif //ZACLIB_TOOLS_HPP

// ZACLib_Types.hpp end

namespace ZACLib {
    class Replace {
    public:
        Replace() { trie.emplace_back(); }

        void AddRule(const ZAC_SV& from) {
            AddRule(from, ZAC_SV("", 0));
        }

        void AddRule(const ZAC_SV& from, const ZAC_SV& to) {
            if (from.empty()) return;
            int node = 0;
            for (const char i : from) {
                const auto c = static_cast<unsigned char>(i);
                if (trie[node].next[c] == -1) {
                    trie[node].next[c] = trie.size(); // NOLINT(*-narrowing-conversions)
                    trie.emplace_back();
                }
                node = trie[node].next[c];
            }
            if (from.size() > trie[node].match_len) {
                trie[node].output_id = outputs.size(); // NOLINT(*-narrowing-conversions)
                trie[node].match_len = from.size();
                outputs.emplace_back(to.data(), to.size());
            }
        }

        void AddReplaceRule(const ZAC_SV& from, const ZAC_SV& to) { AddRule(from, to); }
        void AddRemoveRule(const ZAC_SV& from) { AddRule(from); }

        void Build() {
            std::queue<int> q;
            for (int c = 0; c < 256; ++c) {
                int nxt = trie[0].next[c];
                if (nxt != -1) {
                    trie[nxt].fail = 0;
                    q.push(nxt);
                } else trie[0].next[c] = 0;
            }
            while (!q.empty()) {
                const int u = q.front();
                q.pop();
                for (int c = 0; c < 256; ++c) {
                    int v = trie[u].next[c];
                    if (v != -1) {
                        trie[v].fail = trie[trie[u].fail].next[c];
                        const int f = trie[v].fail;
                        if (trie[f].match_len > trie[v].match_len) {
                            trie[v].match_len = trie[f].match_len;
                            trie[v].output_id = trie[f].output_id;
                        }
                        q.push(v);
                    } else {
                        trie[u].next[c] = trie[trie[u].fail].next[c];
                    }
                }
            }
        }

        std::string Do(const ZAC_SV& input) const {
            std::string result;
            result.reserve(input.size());
            int state = 0;
            size_t i = 0;
            while (i < input.size()) {
                const auto c = static_cast<unsigned char>(input[i]);
                state = trie[state].next[c];
                if (trie[state].output_id != -1) {
                    result.append(outputs[trie[state].output_id]);
                    i += trie[state].match_len;
                    state = 0;
                } else {
                    result.push_back(input[i]);
                    ++i;
                }
            }
            return result;
        }

    private:
        std::vector<Node> trie{Node{}};
        std::vector<std::string> outputs;
    };


    class Search {
    public:
        struct Match {
            size_t pos, len;
            size_t ruleId;
        };

        Search() { trie.emplace_back(); }

        void AddRule(const ZAC_SV& from) {
            if (from.empty()) return;
            int node = 0;
            for (const char i : from) {
                const auto c = static_cast<unsigned char>(i);
                if (trie[node].next[c] == -1) {
                    trie[node].next[c] = trie.size(); // NOLINT(*-narrowing-conversions)
                    trie.emplace_back();
                }
                node = trie[node].next[c];
            }
            if (from.size() > trie[node].match_len) {
                trie[node].output_id = outputs.size(); // NOLINT(*-narrowing-conversions)
                trie[node].match_len = from.size();
                outputs.emplace_back(from.data(), from.size());
            }
        }

        void Build() {
            std::queue<int> q;
            for (int c = 0; c < 256; ++c) {
                int nxt = trie[0].next[c];
                if (nxt != -1) {
                    trie[nxt].fail = 0;
                    q.push(nxt);
                } else trie[0].next[c] = 0;
            }
            while (!q.empty()) {
                const int u = q.front();
                q.pop();
                for (int c = 0; c < 256; ++c) {
                    int v = trie[u].next[c];
                    if (v != -1) {
                        trie[v].fail = trie[trie[u].fail].next[c];
                        const int f = trie[v].fail;
                        if (trie[f].match_len > trie[v].match_len) {
                            trie[v].match_len = trie[f].match_len;
                            trie[v].output_id = trie[f].output_id;
                        }
                        q.push(v);
                    } else {
                        trie[u].next[c] = trie[trie[u].fail].next[c];
                    }
                }
            }
        }

        std::vector<Match> Do(const ZAC_SV& input) const {
            std::vector<Match> result;
            int state = 0;
            for (size_t i = 0; i < input.size(); ++i) {
                const auto c = static_cast<unsigned char>(input[i]);
                state = trie[state].next[c];
                if (trie[state].output_id != -1) {
                    result.push_back(Match{i + 1 - trie[state].match_len, trie[state].match_len, static_cast<size_t>(trie[state].output_id)});
                }
            }
            return result;
        }

    private:
        std::vector<Node> trie{Node{}};
        std::vector<std::string> outputs;
    };


    class Has {
    public:
        Has() { trie.emplace_back(); }

        void AddRule(const ZAC_SV& from) {
            if (from.empty()) return;
            int node = 0;
            for (const unsigned char c : from) {
                if (trie[node].next[c] == -1) {
                    trie[node].next[c] = trie.size(); // NOLINT(*-narrowing-conversions)
                    trie.emplace_back();
                }
                node = trie[node].next[c];
            }
            trie[node].output_id = 0;
        }

        void Build() {
            std::queue<int> q;
            for (int c = 0; c < 256; ++c) {
                int nxt = trie[0].next[c];
                if (nxt != -1) {
                    trie[nxt].fail = 0;
                    q.push(nxt);
                } else {
                    trie[0].next[c] = 0;
                }
            }

            while (!q.empty()) {
                const int u = q.front(); q.pop();
                for (int c = 0; c < 256; ++c) {
                    int v = trie[u].next[c];
                    if (v != -1) {
                        trie[v].fail = trie[trie[u].fail].next[c];
                        q.push(v);
                    } else {
                        trie[u].next[c] = trie[trie[u].fail].next[c];
                    }
                }
            }
        }

        bool Do(const ZAC_SV& input) const {
            int state = 0;
            for (const unsigned char c : input) {
                state = trie[state].next[c];

                int s = state;
                while (s != 0) {
                    if (trie[s].output_id != -1) return true;
                    s = trie[s].fail;
                }
            }
            return false;
        }
    private:
        std::vector<Node> trie{Node{}};
    };
} // namespace ZACLIB

#endif // ZACLIB_HPP
#endif //ZACLIB_SINGLE_HPP
