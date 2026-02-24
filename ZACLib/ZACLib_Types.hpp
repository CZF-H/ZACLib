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
                                       m_size(s.size()) {}

        ZAC_SV(const char* d) : m_data(d),
                                m_size(d ? std::strlen(d) : 0) {}

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
