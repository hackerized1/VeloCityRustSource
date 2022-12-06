#pragma once

#include <cstdint>
#include <string.h>
#include <cstdio>

namespace fnv {
    constexpr uint32_t offsetBasis = 0x811c9dc5;
    constexpr uint32_t prime = 0x1000193;

    constexpr uint32_t hash(const char* str, const uint32_t value = offsetBasis) noexcept
    {
        return *str ? hash(str + 1, (value ^ *str) * static_cast<unsigned long long>(prime)) : value;
    }

    constexpr uint32_t hash_runtime(const char* str) noexcept
    {
        auto value = offsetBasis;

        while (*str) {
            value ^= *str++;
            value *= prime;
        }
        return value;
    }

    constexpr uint32_t hash_w(const wchar_t* wstr, const uint32_t value = offsetBasis) noexcept
    {
        return *wstr ? hash_w(wstr + 1, (value ^ *wstr) * static_cast<unsigned long long>(prime)) : value;
    }

    uint32_t hash_runtime_w(const wchar_t* str) noexcept
    {
        auto value = offsetBasis;

        while (*str) {
            value ^= *str++;
            value *= prime;
        }

        return value;
    }

    uint32_t hash_runtime_w_s(const wchar_t* str, int maxsize) noexcept
    {
        auto value = offsetBasis;

        for (int x = 0; x < maxsize; x++) {
            value ^= *str++;
            value *= prime;
        }

        return value;
    }
}