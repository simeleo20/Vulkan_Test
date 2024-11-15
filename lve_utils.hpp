#pragma once

#include <functional>

namespace lve
{
    template <typename T, typename... Rest>
    void hash_combine(std::size_t &seed, const T &v, Rest &...rest)
    {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (hash_combine(seed, rest), ...);
    }

}