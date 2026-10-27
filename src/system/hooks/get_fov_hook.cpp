//
// Created by emi on 10/27/2026.
//

#include "get_fov_hook.hpp"

#include "events/fov_check_event.hpp"
#include "sdk/mc/client/renderer/game/LevelRenderer.hpp"
#include "system/signatures/basic_signature_system.hpp"

float get_fov_hook::detour(LevelRendererPlayer* self, float a, bool enableVariableFov) {
    fov_check_event::pre event(self, a, enableVariableFov);
    m_events->dispatcher().post(event);
    if (event.is_canceled()) {
        return event.result;
    }

    float result = original(self, a, enableVariableFov);

    fov_check_event::post post_event(self, a, result, enableVariableFov);
    m_events->dispatcher().post(post_event);
    result = post_event.result;

    return result;
}

std::uintptr_t get_fov_hook::resolve_target() {
    return manager->get<signature_system>()->sigs().LevelRendererPlayer.getFov.value();
}

void get_fov_hook::on_initialize() {
    m_events = manager->get<event_system>();
}
