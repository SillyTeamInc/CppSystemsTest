//
// Created by emi on 10/21/26.
//

#pragma once
#include <memory>

#include "gsl/algorithm"
#include "sdk/mc/deps/ecs/EntityId.hpp"

template<class T>
struct GameRefs {
    using OwnerStorage = std::shared_ptr<T>;
    using StackResultStorage = std::shared_ptr<T>;
    using WeakStorage = std::weak_ptr<T>;
    using StackRef = T;
};

template<typename T>
class OwnerPtr : public GameRefs<T>::OwnerStorage {
public:
    using Base = GameRefs<T>::OwnerStorage;
    using Base::Base;

    constexpr OwnerPtr(Base const& ptr) : Base(ptr) {}
    constexpr OwnerPtr(Base&& ptr) : Base(std::move(ptr)) {}
};
template<typename T>
class StackRefResult : public GameRefs<T>::StackResultStorage {
public:
    using Base = GameRefs<T>::StackResultStorage;
    using Base::Base;

    constexpr StackRefResult(Base const& ptr) : Base(ptr) {}
    constexpr StackRefResult(Base&& ptr) : Base(std::move(ptr)) {}
};


template<class T>
class WeakRef : public GameRefs<T>::WeakStorage {
public:
    using Base = GameRefs<T>::WeakStorage;
    using Base::Base;

    constexpr WeakRef(GameRefs<T>::StackResultStorage const& ptr) : Base(ptr) {}
    constexpr WeakRef(Base const& ptr) : Base(ptr) {}
    constexpr WeakRef(Base&& ptr) : Base(std::move(ptr)) {}

    constexpr StackRefResult<T> lock() const {
        if constexpr (requires(Base t) { t.lock(); }) {
            return StackRefResult<T>{this->Base::lock()};
        } else {
            return StackRefResult<T>{*this};
        }
    }
};


class EntityContext;
class EntityRegistry;
class OwnerStorageEntity;
class StackResultStorageEntity;

class WeakStorageEntity {
public:
    WeakRef<EntityRegistry> mRegistry;
    EntityId mEntity{};
};

template<>
struct GameRefs<EntityContext> {
    using OwnerStorage = OwnerStorageEntity;
    using StackResultStorage = StackResultStorageEntity;
    using WeakStorage = WeakStorageEntity;
    using StackRef = EntityContext;
};

template<>
struct GameRefs<EntityContext const> {
    using OwnerStorage = OwnerStorageEntity;
    using StackResultStorage = StackResultStorageEntity;
    using WeakStorage = WeakStorageEntity;
    using StackRef = EntityContext const;
};

template<>
struct GameRefs<EntityId> {
    using OwnerStorage = OwnerStorageEntity;
    using StackResultStorage = StackResultStorageEntity;
    using WeakStorage = WeakStorageEntity;
    using StackRef = EntityContext;
};
