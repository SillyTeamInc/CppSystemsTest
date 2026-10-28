//
// Created by emi on 10/12/2026.
//
#pragma once

#include <atomic>
#include <cstdint>
#include <string>
#include <utility>

#include "system/generic_system.hpp"
#include "system/systems_manager.hpp"

enum class hook_state { pending, installed, failed, skipped };

#define HOOK_RETRIES_UNTIL_RESOLVED                                                                                    \
    [[nodiscard]] bool retry_unresolved() const override { return true; }

class hook_base : public generic_system {
    friend class hook_system_base;
    std::string m_name;
    hook_state m_state = hook_state::pending;
    void* m_target = nullptr;
    bool m_enabled = false;
    bool m_logged_waiting = false;
    static inline std::atomic<int> s_in_flight{0};

    bool queue_enabled(bool enabled);
    void commit_enabled(bool enabled);

public:
    struct detour_guard {
        detour_guard() { s_in_flight.fetch_add(1); }
        ~detour_guard() { s_in_flight.fetch_sub(1); }
        detour_guard(const detour_guard&) = delete;
        detour_guard& operator=(const detour_guard&) = delete;
    };

    static void wait_for_detours();

    explicit hook_base(std::string name) : m_name(std::move(name)) {}

    [[nodiscard]] const std::string& name() const { return m_name; }

    [[nodiscard]] hook_state state() const { return m_state; }

    [[nodiscard]] bool installed() const { return m_target != nullptr; }

    [[nodiscard]] bool enabled() const { return m_enabled; }

    [[nodiscard]] virtual bool start_enabled() const { return true; }

    [[nodiscard]] virtual bool retry_unresolved() const { return false; }

    bool try_install();

    bool set_enabled(bool enabled);

    void shutdown() override;

protected:
    virtual bool applicable() { return true; }

    [[nodiscard]] virtual std::uintptr_t resolve_target() = 0;

    virtual void on_initialize() {}

    virtual bool do_install(std::uintptr_t target) = 0;

    bool install(std::uintptr_t target, void* detour, void** original);
};
