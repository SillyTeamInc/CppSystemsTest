//
// Created by emi on 10/12/2026.
//

#include "hook_base.hpp"

#include <MinHook.h>
#include <spdlog/spdlog.h>

namespace {
    bool is_executable(const std::uintptr_t address) {
        MEMORY_BASIC_INFORMATION mbi{};
        if (VirtualQuery(reinterpret_cast<void*>(address), &mbi, sizeof(mbi)) == 0) return false;
        if (mbi.State != MEM_COMMIT || (mbi.Protect & PAGE_GUARD)) return false;
        constexpr DWORD executable = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
        return (mbi.Protect & executable) != 0;
    }
}

bool hook_base::try_install() {
    if (m_state == hook_state::installed) return true;
    if (m_state == hook_state::failed) return false;

    const std::uintptr_t target = resolve_target();
    if (target == 0) {
        if (retry_unresolved()) {
            if (!m_logged_waiting) {
                spdlog::info("Hook '{}' is waiting for target", m_name);
                m_logged_waiting = true;
            }
            return false;
        }
        spdlog::error("Hook '{}' failed to resolve it's target", m_name);
        m_state = hook_state::failed;
        return false;
    }

    if (!is_executable(target)) {
        spdlog::error("Hook '{}' skipped, 0x{:X} is not executable code", m_name, target);
        m_state = hook_state::failed;
        return false;
    }

    on_initialize();
    if (!do_install(target)) {
        m_state = hook_state::failed;
        return false;
    }

    m_state = hook_state::installed;
    return true;
}

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
    m_state = hook_state::pending;
}
