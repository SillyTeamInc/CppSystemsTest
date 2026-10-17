//
// Created by emi on 10/16/26.
//

#include "module.hpp"

#include "events/module_state_change_event.hpp"
#include "system/systems_manager.hpp"
#include "system/event_system.hpp"

module_system* module::get_module_system() {
    return systems()->get<module_system>();
}

hvs::event_dispatcher& module::dispatcher() {
    return systems()->get<event_system>()->dispatcher();
}

bool module::lock_state(bool locked) {
    m_state_locked = locked;
    return m_state_locked;
}

int module::get_default_key_code() const {
    return m_info.default_key_code;
}

std::string_view module::get_category_name() const {
    return magic_enum::enum_name(m_info.module_category);
}

const std::string_view& module::get_name() const {
    return m_info.name;
}

const std::string_view& module::get_description() const {
    return m_info.description;
}

category module::get_category() const {
    return m_info.module_category;
}

bool module::is_enabled() const {
    return m_enabled;
}

bool module::is_wanted_state() const {
    return m_wanted_state;
}

bool module::is_state_locked() const {
    return m_state_locked;
}

bool module::is_toggle() const {
    return m_info.toggle;
}

void module::set_key_code(int kc) {
    this->key_code = kc;
    spdlog::info("Key code for module '{}' set to {}", this->m_info.name, kc);
}

void module::set_enabled(bool enabled, bool silent) {
    bool shouldToggle = !m_state_locked && (enabled != m_wanted_state);
    if (!shouldToggle) return;

    auto event = module_state_change_event(this, enabled);
    if (!silent) dispatcher().post(event); // lol

    if (event.is_canceled()) return;

    m_wanted_state = enabled;
}

void module::toggle() {
    set_enabled(!m_wanted_state);
}

void module::enable() {
    set_enabled(true);
}

void module::disable() {
    set_enabled(false);
}
