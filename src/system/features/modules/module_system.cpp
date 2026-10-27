//
// Created by emi on 10/16/26.
//
#include "module_system.hpp"

#include "events/keyboard_event.hpp"
#include "events/mod_tick_event.hpp"
#include "misc/freelook.hpp"
#include "misc/fullbright.hpp"
#include "misc/sprint.hpp"
#include "misc/test_module.hpp"
#include "misc/zoom.hpp"
#include "system/event_system.hpp"
#include "system/systems_manager.hpp"

hvs::event_dispatcher &module_system::dispatcher() const {
    return manager->get<event_system>()->dispatcher();
}

void module_system::initialize() {
    spdlog::info("Initializing module system");

    // modules
    add<test_module>();
    add<sprint>();
    add<fullbright>();
    add<freelook>();
    add<zoom>();

    for (const auto &module: modules) {
        module->on_manager_init();
        // this is on-init, so we don't want to use set_enabled.
        // it's on the same thread anyway so it shouldnt matter i think
        if (module->m_info.default_enabled) {
            module->m_enabled = true;
            module->m_wanted_state = true;

            module->on_enable();
            spdlog::info("default-enabled module {}", module->get_name());
        }
    }

    // events
    dispatcher().listen<keyboard_event, &module_system::on_keyboard_event, hvs::priority::highest,
        hvs::on_cancel::receive>(this);
    dispatcher().listen<mod_tick_event, &module_system::on_mod_tick, hvs::priority::highest,
        hvs::on_cancel::receive>(this);
}

void module_system::shutdown() {
    spdlog::info("Shutting down module system");

    dispatcher().deafen<keyboard_event, &module_system::on_keyboard_event>(this);
    dispatcher().deafen<mod_tick_event, &module_system::on_mod_tick>(this);

    for (auto &module: modules) {
        if (module->m_enabled) {
            spdlog::debug("calling on_disable for {}", module->get_name());
            module->on_disable();
        }
        module->m_enabled = false;
        module->on_manager_exit();
        spdlog::trace("cleaned up {}", module->get_name());
        module.reset();
    }

    modules.clear();
}

void module_system::on_keyboard_event(keyboard_event &event) const {
    if (event.is_repeated()) return;

    for (const auto &module: modules) {
        if (module->key_code != event.key_code) continue;
        if (!module->can_be_toggled()) continue;

        if (!module->is_toggle()) module->set_enabled(event.is_down);
        else if (event.is_down) module->toggle();
    }
}

void module_system::on_mod_tick(const mod_tick_event &event) const {
    for (const auto &module: modules) {
        if (module->m_wanted_state != module->m_enabled) {
            module->m_enabled = module->m_wanted_state;
            /*spdlog::trace("onClientTick: calling {} on module {}", module->m_enabled ? "onEnable" : "onDisable",
                          module->get_name());*/
            if (module->m_enabled)module->on_enable();
            else module->on_disable();
        }
    }
}
