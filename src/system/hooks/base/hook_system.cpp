//
// Created by emi on 10/12/2026.
//

#include "system/hooks/base/hook_system.hpp"

#include <MinHook.h>
#include <spdlog/spdlog.h>

#include "events/mod_tick_event.hpp"
#include "system/event_system.hpp"

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
    m_pending = 0;
    for (auto* hook : m_hooks) {
        if (hook->installed()) {
            ++installed;
            if (hook->start_enabled()) hook->set_enabled(true);
        } else if (hook->state() == hook_state::pending)
            ++m_pending;
    }

    spdlog::info("{}/{} hooks installed, {} pending", installed, m_hooks.size(), m_pending);
    if (m_pending > 0) start_listening();
}

void hook_system_base::shutdown() {
    stop_listening();

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

void hook_system_base::on_mod_tick(mod_tick_event&) {
    if (m_pending == 0) return;

    for (auto* hook : m_hooks) {
        if (hook->state() != hook_state::pending) continue;

        if (hook->try_install()) {
            --m_pending;
            if (hook->start_enabled()) hook->set_enabled(true);
        } else if (hook->state() == hook_state::failed) {
            --m_pending;
        }
    }

    if (m_pending == 0) spdlog::info("all hooks installed!!");
}

void hook_system_base::start_listening() {
    if (m_listening) return;
    m_listening = true;
    manager->get<event_system>()->dispatcher().listen<mod_tick_event, &hook_system_base::on_mod_tick>(this);
}

void hook_system_base::stop_listening() {
    if (!m_listening) return;

    manager->get<event_system>()->dispatcher().deafen<mod_tick_event, &hook_system_base::on_mod_tick>(this);
    m_listening = false;
}
