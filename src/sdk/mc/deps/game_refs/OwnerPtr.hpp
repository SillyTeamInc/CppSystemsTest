//
// Created by emi on 10/21/26.
//

#pragma once
#include "gsl/algorithm"

template <class T>
struct GameRefs {
    using OwnerStorage       = std::shared_ptr<T>;
    using StackResultStorage = std::shared_ptr<T>;
    using WeakStorage        = std::weak_ptr<T>;
    using StackRef           = T;
};

template <typename T>
class OwnerPtr : public GameRefs<T>::OwnerStorage {
public:
    using Base = GameRefs<T>::OwnerStorage;
    using Base::Base;

    constexpr OwnerPtr(Base const& ptr) : Base(ptr) {}
    constexpr OwnerPtr(Base&& ptr) : Base(std::move(ptr)) {}
};
