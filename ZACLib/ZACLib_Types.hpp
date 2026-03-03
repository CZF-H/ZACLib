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

#if defined(__ANDROID__) || defined(__arm__) || defined(__aarch64__)
using ZAC_CHAR = unsigned char;
#else
using ZAC_CHAR = char;
#endif


namespace ZACLib {
    #if __cplusplus >= 201703L
    class ZAC_SV : public std::basic_string_view<ZAC_CHAR> {
    public:
        using std::basic_string_view<ZAC_CHAR>::basic_string_view;
        ZAC_SV() = default;

        // ReSharper disable once CppRedundantCastExpression
        ZAC_SV(const std::string& s) noexcept
            : std::basic_string_view<ZAC_CHAR>(reinterpret_cast<const ZAC_CHAR*>(s.data()), s.size()) {}

#if defined(__ANDROID__) || defined(__arm__) || defined(__aarch64__)
        ZAC_SV(const char* s, const size_t n)
            : std::basic_string_view<ZAC_CHAR>(reinterpret_cast<const ZAC_CHAR*>(s), n) {}

        ZAC_SV(const char* s)
            : std::basic_string_view<ZAC_CHAR>(
                reinterpret_cast<const ZAC_CHAR*>(s),
                s ? std::char_traits<char>::length(s) : 0) {}
#endif
    };
    #else
    class ZAC_SV {
        const ZAC_CHAR* m_data;
        const size_t m_size;
    public:
        ZAC_SV() : m_data(nullptr),
                   m_size(0) {}

        ZAC_SV(const ZAC_CHAR* d, const size_t s) : m_data(d),
                                          m_size(s) {}

#if defined(__ANDROID__) || defined(__arm__) || defined(__aarch64__)
        ZAC_SV(const char* d, const size_t s) : m_data(reinterpret_cast<const ZAC_CHAR*>(d)),
                                                m_size(s) {}
#endif

        // ReSharper disable once CppRedundantCastExpression
        ZAC_SV(const std::string& s) : m_data(reinterpret_cast<const ZAC_CHAR*>(s.c_str())),
                                       m_size(s.size()) {} // 模仿std::string_view，不禁止隐式构造

        ZAC_SV(const ZAC_CHAR* d) : m_data(d),
                                m_size(d ? std::strlen(reinterpret_cast<const char*>(d)) : 0) {} // 模仿std::string_view，不禁止隐式构造

#if defined(__ANDROID__) || defined(__arm__) || defined(__aarch64__)
        ZAC_SV(const char* d) : m_data(reinterpret_cast<const ZAC_CHAR*>(d)),
                                m_size(d ? std::strlen(d) : 0) {}
#endif

        const ZAC_CHAR* data() const noexcept {
            return m_data;
        }

        std::size_t size() const noexcept {
            return m_size;
        }

        bool empty() const noexcept {
            return m_size == 0;
        }

        const ZAC_CHAR* begin() const { return m_data; }
        const ZAC_CHAR* end() const { return m_data + m_size; }
        const ZAC_CHAR* cbegin() const { return m_data; }
        const ZAC_CHAR* cend() const { return m_data + m_size; }

        const ZAC_CHAR& operator[](const size_t i) const { return m_data[i]; }
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
