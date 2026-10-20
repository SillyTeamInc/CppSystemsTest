//
// Created by emi on 10/17/26.
//
#pragma once
#include <cstdint>
#include <entt/entt.hpp>

#include "sdk/mc/deps/core/utility/optional_ref.hpp"
#include "util/types.hpp"

// see https://bedrock-native-modding.github.io/wiki/entities/entt.html

struct EntityId {
    std::uint32_t rawId;

    [[nodiscard]] constexpr bool operator==(const EntityId& other) const = default;

    [[nodiscard]] constexpr operator std::uint32_t() const {
        return this->rawId;
    }
};

struct EntityIdTraits {
    using value_type = EntityId;

    using entity_type = std::uint32_t;
    using version_type = std::uint16_t;

    static constexpr entity_type entity_mask = 0x3FFFF;
    static constexpr entity_type version_mask = 0x3FFF;
};

template<>
struct entt::entt_traits<EntityId> : entt::basic_entt_traits<EntityIdTraits> {
    static constexpr std::size_t page_size = 2048;
};

// i cba to move this lol its fine
class EntityRegistry
{
    std::string mDebugName;
    entt::basic_registry<EntityId> mRegistry;
    uint mId{};
    std::function<void(EntityId)> mPreEntityInvoke;
    std::function<void(EntityId)> mPostEntityInvoke;
};

class EntityContext {
public:

public:
    [[nodiscard]] inline entt::basic_registry<EntityId>& getRegistry() noexcept { return mEnTTRegistry; }

    [[nodiscard]] inline entt::basic_registry<EntityId> const& getRegistry() const noexcept { return mEnTTRegistry; }

    template <class T>
    [[nodiscard]] inline optional_ref<T const> tryGetComponent() const {
        return getRegistry().try_get<T>(mEntity);
    }

    template <class T>
    [[nodiscard]] inline optional_ref<T> tryGetComponent() {
        return const_cast<T*>(std::as_const(getRegistry()).try_get<T>(mEntity));
    }

    template <class T>
    [[nodiscard]] inline bool hasComponent() const {
        return getRegistry().all_of<T>(mEntity);
    }

    template <class T>
    inline bool removeComponent() {
        return getRegistry().remove<T>(mEntity);
    }

    template <class T, typename... Args>
    [[nodiscard]] inline T& getOrAddComponent(Args&&... args) {
        return getRegistry().get_or_emplace<T>(mEntity, std::forward<Args>(args)...);
    }

public:
    EntityRegistry* mRegistry;
    entt::basic_registry<EntityId>& mEnTTRegistry;
    EntityId const mEntity;
};

