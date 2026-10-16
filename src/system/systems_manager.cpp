//
// Created by emi on 2/23/2026.
//

#include "systems_manager.hpp"
#include "generic_system.hpp"
#include <ranges>

systems_manager::~systems_manager() {
    shutdown_systems();
}

std::unique_ptr<systems_manager> systems_manager::s_instance = nullptr;

systems_manager* systems_manager::i() {
    if (!s_instance) {
        s_instance = std::make_unique<systems_manager>();
    }
    return s_instance.get();
}

void systems_manager::destroy_instance() {
    s_instance.reset();
}

void systems_manager::initialize_systems() const {
    for (auto& system: m_owned) {
        //spdlog::info("initializing system: {}", typeid(*system).name());
        system->initialize();
    }
}

void systems_manager::shutdown_systems() {
    while (!m_owned.empty()) {
        generic_system& system = *m_owned.back();
        //spdlog::info("shutting down system: {}", typeid(system).name());
        system.shutdown();
        detach(system);
        m_owned.pop_back();
    }
}

void systems_manager::attach(generic_system& s) {
    s.manager = this;
    index(s);
    s.for_each_child([this](generic_system& child) { attach(child); });
}

void systems_manager::detach(generic_system& s) {
    s.for_each_child([this](generic_system& child) { detach(child); });
    unindex(s);
    s.manager = nullptr;
}

void systems_manager::index(generic_system& s) {
    m_all.push_back(&s);
    spdlog::debug("Indexing system: {}", typeid(s).name());
    const auto [it, inserted] = m_by_type.try_emplace(typeid(s), &s);
    assert(inserted && "duplicate system type registered");
}

void systems_manager::unindex(generic_system& s) {
    std::erase(m_all, &s);
    if (const auto it = m_by_type.find(typeid(s)); it != m_by_type.end() && it->second == &s) {
        m_by_type.erase(it);
    }
}
