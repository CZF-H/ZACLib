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

// ZACLib_Types.hpp
//
// Created by wanjiangzhi on 2026/2/24.
//

// ReSharper disable CppNonExplicitConvertingConstructor
#ifndef ZACLIB_TYPES_HPP
#define ZACLIB_TYPES_HPP

#if __cplusplus >= 201703L
#include <string_view>
#else
#include <cstring>
#endif

#include <string>
#include <array>
#include <limits>


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
        // ReSharper disable once CppVariableCanBeMadeConstexpr
        static const auto kInvalidOutput = std::numeric_limits<size_t>::max();
        size_t output_id;
        size_t match_len;

        Node() : fail(0),
                 output_id(kInvalidOutput),
                 match_len(0) { next.fill(-1); }
    };
}

#endif //ZACLIB_TYPES_HPP

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
            if (from.size() > max_rule_len) max_rule_len = from.size();
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

            if (input.empty()) return result;
            if (max_rule_len == 0) {
                result.append(input.data(), input.size());
                return result;
            }

            const auto invalid_output = Node::kInvalidOutput;
            const size_t ring_size = max_rule_len + 1;
            std::vector<size_t> pending_start(ring_size, invalid_output);
            std::vector<size_t> pending_len(ring_size, 0);
            std::vector<size_t> pending_output(ring_size, invalid_output);

            auto get_best = [&](const size_t start, size_t& best_len, size_t& best_output) {
                const size_t idx = start % ring_size;
                if (pending_start[idx] == start) {
                    best_len = pending_len[idx];
                    best_output = pending_output[idx];
                } else {
                    best_len = 0;
                    best_output = invalid_output;
                }
            };

            auto set_best = [&](const size_t start, const size_t len, const size_t output_id) {
                const size_t idx = start % ring_size;
                if (pending_start[idx] != start) {
                    pending_start[idx] = start;
                    pending_len[idx] = 0;
                    pending_output[idx] = invalid_output;
                }
                if (len > pending_len[idx]) {
                    pending_len[idx] = len;
                    pending_output[idx] = output_id;
                }
            };

            int state = 0;
            size_t last_pos = 0;
            size_t cursor = 0;

            auto emit_until = [&](const size_t upper_bound) {
                while (cursor < upper_bound) {
                    size_t best_len = 0;
                    size_t best_output = invalid_output;
                    get_best(cursor, best_len, best_output);
                    if (best_len == 0) {
                        ++cursor;
                        continue;
                    }

                    result.append(input.data() + last_pos, cursor - last_pos);
                    result.append(outputs[best_output]);
                    cursor += best_len;
                    last_pos = cursor;
                }
            };

            for (size_t i = 0; i < input.size(); ++i) {
                const unsigned char c = input[i];
                state = trie[state].next[c];

                if (trie[state].output_id != invalid_output) {
                    const size_t match_len = trie[state].match_len;
                    const size_t match_start = i + 1 - match_len;
                    set_best(match_start, match_len, trie[state].output_id);
                }

                const size_t finalize_before = (i + 1 > max_rule_len) ? (i + 1 - max_rule_len) : 0;
                emit_until(finalize_before);
            }

            emit_until(input.size());

            if (last_pos < input.size()) {
                result.append(input.data() + last_pos, input.size() - last_pos);
            }

            return result;
        }

    private:
        std::vector<Node> trie{Node{}};
        std::vector<std::string> outputs;
        size_t max_rule_len = 0;
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
                if (trie[state].output_id != Node::kInvalidOutput) {
                    result.push_back(Match{i + 1 - trie[state].match_len, trie[state].match_len, trie[state].output_id});
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
                const int u = q.front();
                q.pop();
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
                    if (trie[s].output_id != Node::kInvalidOutput) return true;
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
