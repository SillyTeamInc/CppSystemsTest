//
// Created by emi on 10/12/2026.
//
#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "system/generic_system.hpp"
#include "system/systems_manager.hpp"

class hook_base : public generic_system {
    std::string m_name;
    void* m_target = nullptr;
    bool m_enabled = false;
public:
    explicit hook_base(std::string name) : m_name(std::move(name)) {}

    [[nodiscard]] const std::string& name() const {
        return m_name;
    }
    [[nodiscard]] bool installed() const {
        return m_target != nullptr;
    }
    [[nodiscard]] bool enabled() const {
        return m_enabled;
    }
    [[nodiscard]] virtual bool start_enabled() const {
        return true;
    }

    bool set_enabled(bool enabled);

    void shutdown() override;

protected:
    [[nodiscard]] virtual std::uintptr_t resolve_target() = 0;
    virtual void on_initialize() {}

    bool install(std::uintptr_t target, void* detour, void** original);
};
