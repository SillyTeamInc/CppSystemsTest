//
// Created by emi on 10/12/2026.
//
#pragma once

#include <atomic>
#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

#include <spdlog/spdlog.h>

#include "system/hooks/base/hook_base.hpp"

template<typename Derived, typename Signature>
class hook;

template<typename Derived, typename R, typename... Args>
class hook<Derived, R(Args...)> : public hook_base {
public:
    using signature = R(Args...);

    void initialize() final {
        static_assert(std::is_same_v<decltype(&Derived::detour), R (Derived::*)(Args...)>,
                      "Derived::detour must be a public non-static member taking exactly the hooked function's arguments");

        if (s_self.load() != nullptr) {
            spdlog::error("Hook '{}' skipped, another instance of this hook is already installed!!!!", name());
            return;
        }

        const std::uintptr_t target = resolve_target();
        if (target == 0) {
            spdlog::error("Hook '{}' skipped, its target address was not resolved", name());
            return;
        }

        on_initialize();

        s_self.store(static_cast<Derived*>(this));
        if (!install(target, reinterpret_cast<void*>(&trampoline), reinterpret_cast<void**>(&s_original))) {
            s_self.store(nullptr);
        }
    }

    void shutdown() final {
        set_enabled(false);
        s_self.store(nullptr);
        hook_base::shutdown();
    }

protected:
    explicit hook(std::string name) : hook_base(std::move(name)) {}

    R original(Args... args) const {
        return s_original(std::forward<Args>(args)...);
    }

private:
    // This is stupid but it works so Fuck you
    static inline std::atomic<Derived*> s_self{nullptr};
    static inline signature* s_original = nullptr;

    static R trampoline(Args... args) {
        if (auto* self = s_self.load(std::memory_order_acquire)) {
            return self->detour(std::forward<Args>(args)...);
        }
        return s_original(std::forward<Args>(args)...);
    }
};
