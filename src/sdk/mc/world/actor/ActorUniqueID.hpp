//
// Created by emi on 10/17/26.
//

#pragma once
#include <compare>

#include "util/types.hpp"

struct ActorUniqueID {
public:
    int64 rawID;

    [[nodiscard]] constexpr bool operator==(ActorUniqueID const &other) const noexcept { return rawID == other.rawID; }

    [[nodiscard]] constexpr std::strong_ordering operator<=>(ActorUniqueID const &other) const noexcept {
        return rawID <=> other.rawID;
    }
};

namespace std {
    template<>
    struct hash<ActorUniqueID> {
        size_t operator()(ActorUniqueID const &id) const noexcept {
            static std::hash<std::int64_t> hasher;
            std::size_t seed = 0;
            seed ^= hasher(id.rawID >> 32) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hasher(id.rawID & 0xffffffff) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}
