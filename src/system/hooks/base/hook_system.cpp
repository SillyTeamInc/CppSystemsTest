//
// Created by emi on 10/12/2026.
//

#include "system/hooks/base/hook_system.hpp"

#include <MinHook.h>
#include <spdlog/spdlog.h>

bool hook_system_base::init_minhook() {
    if (const auto status = MH_Initialize(); status != MH_OK) {
        spdlog::error("MinHook failed to initialize: {}", MH_StatusToString(status));
        return false;
    }

    m_minhook_ready = true;
    spdlog::info("MinHook initialized");
    return true;
}

void hook_system_base::initialize_subsystems() {
    if (init_minhook()) {
        add_hooks();
    }
}

void hook_system_base::initialize() {
    complex_system::initialize();

    std::size_t installed = 0;
    for (auto* hook : m_hooks) {
        if (!hook->installed()) {
            continue;
        }
        ++installed;
        if (hook->start_enabled()) {
            hook->set_enabled(true);
        }
    }

    spdlog::info("{}/{} hooks installed", installed, m_hooks.size());
}

void hook_system_base::shutdown() {
    complex_system::shutdown();
    m_hooks.clear();

    if (m_minhook_ready) {
        MH_Uninitialize();
        m_minhook_ready = false;
        spdlog::info("MinHook uninitialized");
    }
}

void hook_system_base::shutdown_subsystems() {

}
