//
// Created by wanjiangzhi on 2026/2/24.
//

// ReSharper disable CppClassNeverUsed
#include "ZACLib.hpp"
#include <array>
#include <queue>

namespace ZACLib {
    Replace::Replace() {
        trie.emplace_back();
    }

    void Replace::AddRule(const ZAC_SV& from) {
        AddRule(from, ZAC_SV("", 0));
    }

    void Replace::AddRule(const ZAC_SV& from, const ZAC_SV& to) {
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

    void Replace::AddReplaceRule(const ZAC_SV& from, const ZAC_SV& to) {
        AddRule(from, to);
    }

    void Replace::AddRemoveRule(const ZAC_SV& from) {
        AddRule(from);
    }

    void Replace::Build() {
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

                    int f = trie[v].fail;
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

    std::string Replace::Do(const ZAC_SV& input) const {
        std::string result;
        result.reserve(input.size());

        int state = 0;
        size_t i = 0;
        while (i < input.size()) {
            const auto c = static_cast<unsigned char>(input[i]);
            state = trie[state].next[c];

            if (trie[state].output_id != -1) {
                const std::string& rep = outputs[trie[state].output_id];
                result.append(rep);
                i += trie[state].match_len;
                state = 0;
            } else {
                result.push_back(input[i]);
                ++i;
            }
        }
        return result;
    }


    Search::Search() {
        trie.emplace_back();
    }

    void Search::AddRule(const ZAC_SV& from) {
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

    void Search::Build() {
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

                    int f = trie[v].fail;
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

    std::vector<Search::Match> Search::Do(const ZAC_SV& input) const {
        std::vector<Match> result;
        int state = 0;
        for (size_t i = 0; i < input.size(); ++i) {
            const auto c = static_cast<unsigned char>(input[i]);
            state = trie[state].next[c];

            if (trie[state].output_id != -1) {
                result.push_back(
                    Match{
                        i + 1 - trie[state].match_len,
                        trie[state].match_len,
                        static_cast<size_t>(trie[state].output_id)
                    }
                );
            }
        }
        return result;
    }


    Has::Has() { trie.emplace_back(); }

    void Has::AddRule(const ZAC_SV& from) {
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

    void Has::Build() {
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

    bool Has::Do(const ZAC_SV& input) const {
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
} // namespace ZACLIB
