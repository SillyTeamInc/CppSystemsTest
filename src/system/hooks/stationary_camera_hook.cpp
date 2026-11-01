//
// Created by emi on 10/31/26.
//

#include "stationary_camera_hook.hpp"
#include "system/signatures/basic_signature_system.hpp"
#include <system/event_system.hpp>

#include "events/stationary_camera_event.hpp"
void* stationary_camera_hook::detour(void* self, void* regRef) {
    stationary_camera_event event = {};
    m_events->dispatcher().post(event);

    return original(self, regRef);
}

std::uintptr_t stationary_camera_hook::resolve_target() {
    return manager->get<signature_system>()->sigs().StationaryCameraSystem.tick.value();
}

void stationary_camera_hook::on_initialize() {
    m_events = manager->get<event_system>();
}
