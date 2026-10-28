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

void hook_system_base::apply_enabled(const std::vector<hook_base*>& hooks, bool enabled) {
    std::vector<hook_base*> queued;
    queued.reserve(hooks.size());
    for (auto* hook: hooks) {
        if (hook->queue_enabled(enabled))
            queued.push_back(hook);
    }

    if (queued.empty())
        return;

    if (const auto status = MH_ApplyQueued(); status != MH_OK) {
        // tbh idk how this would fail but might as well handle it
        spdlog::warn("batch {} failed [{}], trying to iterate instead!", enabled ? "enable" : "disable",
                     MH_StatusToString(status));
        for (auto* hook: queued)
            hook->set_enabled(enabled);
        return;
    }

    for (auto* hook: queued)
        hook->commit_enabled(enabled);
    spdlog::info("{} {} hooks from one batch", enabled ? "enabled" : "disabled", queued.size());
}

void hook_system_base::initialize_subsystems() {
    if (init_minhook()) {
        add_hooks();
    }
}


void hook_system_base::initialize() {
    complex_system::initialize();

    std::size_t installed = 0;
    std::vector<hook_base*> to_enable;
    m_pending = 0;
    for (auto* hook: m_hooks) {
        if (hook->installed()) {
            ++installed;
            if (hook->start_enabled())
                to_enable.push_back(hook);
        } else if (hook->state() == hook_state::pending)
            ++m_pending;
    }
    apply_enabled(to_enable, true);

    spdlog::info("{}/{} hooks installed, {} pending", installed, m_hooks.size(), m_pending);
    if (m_pending > 0)
        start_listening();
}

void hook_system_base::shutdown() {
    stop_listening();

    // prevents hooks from being removed while theyre being executed
    apply_enabled(m_hooks, false);
    hook_base::wait_for_detours();

    complex_system::shutdown();
    m_hooks.clear();

    if (m_minhook_ready) {
        MH_Uninitialize();
        m_minhook_ready = false;
        spdlog::info("MinHook uninitialized");
    }
}

void hook_system_base::shutdown_subsystems() {}

void hook_system_base::on_mod_tick(mod_tick_event&) {
    if (m_pending == 0)
        return;

    std::vector<hook_base*> to_enable;
    for (auto* hook: m_hooks) {
        if (hook->state() != hook_state::pending)
            continue;

        if (hook->try_install()) {
            --m_pending;
            if (hook->start_enabled())
                to_enable.push_back(hook);
        } else if (hook->state() == hook_state::failed || hook->state() == hook_state::skipped) {
            --m_pending;
        }
    }
    apply_enabled(to_enable, true);

    if (m_pending == 0)
        spdlog::info("all hooks installed!!");
}

void hook_system_base::start_listening() {
    if (m_listening)
        return;
    m_listening = true;
    manager->get<event_system>()->dispatcher().listen<mod_tick_event, &hook_system_base::on_mod_tick>(this);
}

void hook_system_base::stop_listening() {
    if (!m_listening)
        return;

    manager->get<event_system>()->dispatcher().deafen<mod_tick_event, &hook_system_base::on_mod_tick>(this);
    m_listening = false;
}
