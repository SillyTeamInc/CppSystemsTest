//
// Created by emi on 10/12/2026.
//

#include "hook_base.hpp"

#include <MinHook.h>
#include <spdlog/spdlog.h>

bool hook_base::install(std::uintptr_t target, void* detour, void** original) {
    auto* address = reinterpret_cast<void*>(target);
    if (const auto status = MH_CreateHook(address, detour, original); status != MH_OK) {
        spdlog::error("Hook '{}' could not be created at 0x{:X}: {}", m_name, target, MH_StatusToString(status));
        return false;
    }

    m_target = address;
    spdlog::info("Hook '{}' installed at 0x{:X}", m_name, target);
    return true;
}

bool hook_base::set_enabled(bool enabled) {
    if (!installed()) {
        return false;
    }
    if (enabled == m_enabled) {
        return true;
    }

    const auto status = enabled ? MH_EnableHook(m_target) : MH_DisableHook(m_target);
    if (status != MH_OK) {
        spdlog::error("Hook '{}' could not be {}: {}", m_name, enabled ? "enabled" : "disabled", MH_StatusToString(status));
        return false;
    }

    m_enabled = enabled;
    spdlog::info("Hook '{}' {}", m_name, enabled ? "enabled" : "disabled");
    return true;
}

void hook_base::shutdown() {
    if (!installed()) {
        return;
    }

    if (const auto status = MH_RemoveHook(m_target); status != MH_OK) {
        spdlog::warn("Hook '{}' could not be removed: {}", m_name, MH_StatusToString(status));
    }

    m_target = nullptr;
    m_enabled = false;
}
