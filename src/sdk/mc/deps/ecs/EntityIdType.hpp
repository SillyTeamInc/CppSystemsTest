//
// Created by emi on 10/29/2026.
//

#pragma once

struct EntityId {
    std::uint32_t rawId;

    [[nodiscard]] constexpr bool operator==(const EntityId& other) const = default;

    [[nodiscard]] constexpr operator std::uint32_t() const { return this->rawId; }
};