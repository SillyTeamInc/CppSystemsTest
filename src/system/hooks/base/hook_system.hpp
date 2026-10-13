//
// Created by emi on 10/12/2026.
//
#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>

#include "system/complex_system.hpp"
#include "system/hooks/base/hook_base.hpp"

class hook_system_base : public complex_system {
    std::vector<hook_base*> m_hooks;
    bool m_minhook_ready = false;

    bool init_minhook();

protected:
    virtual void add_hooks() = 0;

    template<typename T>
    void add_hook() {
        m_hooks.push_back(&add_subsystem<T>());
    }

public:
    void initialize_subsystems() override;
    void initialize() override;
    void shutdown() override;
    void shutdown_subsystems() override;
};

template<typename... Hooks>
struct hook_group {};

namespace hook_detail {
    template<template<std::size_t> class H, std::size_t... I>
    hook_group<H<I>...> make_series(std::index_sequence<I...>);
}

template<template<std::size_t> class H, std::size_t N>
using hook_series = decltype(hook_detail::make_series<H>(std::make_index_sequence<N>{}));

template<typename... Entries>
class basic_hook_system final : public hook_system_base {
    template<typename T>
    void add_entry(std::type_identity<T>) {
        static_assert(std::is_base_of_v<hook_base, T>, "every entry must derive from hook<>");
        add_hook<T>();
    }

    template<typename... H>
    void add_entry(std::type_identity<hook_group<H...>>) {
        (add_entry(std::type_identity<H>{}), ...);
    }

    void add_hooks() override {
        (add_entry(std::type_identity<Entries>{}), ...);
    }
};