//
// Created by emi on 10/17/26.
//
#pragma once
#include <cstdint>
#include <entt/entt.hpp>

#include "sdk/mc/deps/core/utility/optional_ref.hpp"
#include "sdk/mc/deps/minecraft_camera/components/CameraBlendStateComponent.hpp"
#include "sdk/mc/deps/minecraft_camera/components/CameraRenderFirstPersonObjectsComponent.hpp"
#include "sdk/mc/deps/vanilla_camera/VanillaCameraApiComponent.hpp"
#include "sdk/mc/entity/components/CameraComponent.hpp"
#include "sdk/mc/entity/components/CameraCustomFovComponent.hpp"
#include "sdk/mc/entity/components/CameraDirectLookComponent.hpp"
#include "sdk/mc/entity/components/CameraOrbitComponent.hpp"


#include "sdk/mc/entity/components/FlagComponent.hpp"
#include "util/types.hpp"

// see https://bedrock-native-modding.github.io/wiki/entities/entt.html

struct EntityId {
    std::uint32_t rawId;

    [[nodiscard]] constexpr bool operator==(const EntityId& other) const = default;

    [[nodiscard]] constexpr operator std::uint32_t() const { return this->rawId; }
};

namespace std {
    template<>
    struct hash<EntityId> {
        [[nodiscard]] std::size_t operator()(const EntityId& id) const noexcept {
            return std::hash<std::uint32_t>{}(id.rawId);
        }
    };
} // namespace std

struct EntityIdTraits {
    using value_type = EntityId;

    using entity_type = std::uint32_t;
    using version_type = std::uint16_t;

    static constexpr entity_type entity_mask = 0x3FFFF;
    // i don't know if this even makes a difference tbh
    // static constexpr entity_type version_mask = 0x3FFF;
    static constexpr entity_type version_mask = 0x7FF;
};

#define ENTT_COMPONENT_TRAITS(Type)                                                                                    \
    template<>                                                                                                         \
    struct entt::component_traits<Type, EntityId> {                                                                    \
        using element_type = Type;                                                                                     \
        using entity_type = EntityId;                                                                                  \
        static constexpr bool in_place_delete = true;                                                                  \
        static constexpr std::size_t page_size = 128;                                                                  \
    };

ENTT_COMPONENT_TRAITS(MinecraftCamera::CameraDirectLookComponent)
ENTT_COMPONENT_TRAITS(MinecraftCamera::CameraComponent)
ENTT_COMPONENT_TRAITS(VanillaCamera::UpdatePlayerFromCameraComponent)
ENTT_COMPONENT_TRAITS(MinecraftCamera::CameraOrbitComponent)
ENTT_COMPONENT_TRAITS(PlayerStateAffectsRenderingComponent)
ENTT_COMPONENT_TRAITS(MinecraftCamera::CameraCustomFovComponent)
ENTT_COMPONENT_TRAITS(MinecraftCamera::CameraRenderFirstPersonObjectsComponent)

ENTT_COMPONENT_TRAITS(CameraAPIComponent)


struct RenderRotationComponent {
public:
    glm::vec2 mRot;
};

struct ActorRotationComponent {
public:
    glm::vec2 mRot;
    glm::vec2 mRotPrev;
};


ENTT_COMPONENT_TRAITS(ActorRotationComponent)
ENTT_COMPONENT_TRAITS(RenderRotationComponent)
ENTT_COMPONENT_TRAITS(MinecraftCamera::CameraBlendStateComponent);


#undef ENTT_COMPONENT_TRAITS


template<>
struct entt::entt_traits<EntityId> : entt::basic_entt_traits<EntityIdTraits> {
    static constexpr std::size_t page_size = 2048;
};

// this took fucking forever to figure out
template<typename Type, typename Allocator>
    requires(!std::is_same_v<Type, EntityId>)
struct entt::storage_type<Type, EntityId, Allocator> {
    using type = entt::basic_storage<Type, EntityId, Allocator>;
};

static_assert(
        sizeof(entt::basic_registry<EntityId>::storage_for_type<VanillaCamera::UpdatePlayerFromCameraComponent>) ==
        0x68);

// i cba to move this lol its fine
class EntityRegistry {
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

    template<class T>
    [[nodiscard]] inline optional_ref<T const> tryGetComponent() const {
        return getRegistry().try_get<T>(mEntity);
    }

    template<class T>
    [[nodiscard]] inline optional_ref<T> tryGetComponent() {
        return const_cast<T*>(std::as_const(getRegistry()).try_get<T>(mEntity));
    }

    template<class T>
    [[nodiscard]] inline bool hasComponent() const {
        return getRegistry().all_of<T>(mEntity);
    }

    template<class T>
    inline bool removeComponent() {
        return getRegistry().remove<T>(mEntity);
    }

    template<class T, typename... Args>
    [[nodiscard]] inline T& getOrAddComponent(Args&&... args) {
        return getRegistry().get_or_emplace<T>(mEntity, std::forward<Args>(args)...);
    }

    template<typename Type>
    [[nodiscard]] inline bool has_flag() const {
        return getRegistry().all_of<Type>(mEntity);
    }

    template<typename Type>
    [[nodiscard]] inline bool has_flag(EntityId entity) const {
        return getRegistry().all_of<Type>(entity);
    }

    template<typename Type>
    inline void set_flag(bool state) {
        if (state) {
            getRegistry().emplace<Type>(mEntity);
        } else {
            getRegistry().remove<Type>(mEntity);
        }
    }

    template<typename Type>
    inline void set_flag(EntityId entity, bool state) {
        if (state) {
            getRegistry().emplace<Type>(entity);
        } else {
            getRegistry().remove<Type>(entity);
        }
    }

    template<typename Type>
    [[nodiscard]] inline entt::basic_registry<EntityId>::storage_for_type<Type>* find_storage() {
        return static_cast<entt::basic_registry<EntityId>::storage_for_type<Type>*>(
                mEnTTRegistry.storage(entt::type_hash<Type>::value()));
    }

public:
    EntityRegistry* mRegistry;
    entt::basic_registry<EntityId>& mEnTTRegistry;
    EntityId const mEntity;
};
