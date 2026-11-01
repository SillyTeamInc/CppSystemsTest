//
// Created by emi on 11/1/26.
//

#include "culling_hook.hpp"

#include "events/cull_event.hpp"
#include "system/event_system.hpp"
#include "system/systems_manager.hpp"

void* culling_hook::detour(void* self, uint8_t type) {
    // testing....!!!! NOP for now
    spdlog::info("culling_hook::detour called with type: {}", static_cast<int>(type));

    cull_event event(type);
    m_events->dispatcher().post(event);
    type = event.m_type;

    return original(self, type);
}

std::uintptr_t culling_hook::resolve_target() {
    return manager->get<signature_system>()->sigs().LevelRendererCameraListeners.updateLevelCullerType;
}

void culling_hook::on_initialize() {
    m_events = manager->get<event_system>();
}
