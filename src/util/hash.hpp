//
// Created by emi on 10/16/26.
//
#pragma once
#include <cstdint>
#include <string_view>

namespace util::hash {
    constexpr std::uint32_t fnv1a(std::string_view s) {
        std::uint32_t hash = 2166136261u;
        for (const char c: s) {
            hash ^= static_cast<unsigned char>(c);
            hash *= 16777619u;
        }
        return hash;
    }

    template<typename T>
    constexpr std::uint32_t type_id() {
        return fnv1a(entt::internal::stripped_type_name<T>());
    }
}
