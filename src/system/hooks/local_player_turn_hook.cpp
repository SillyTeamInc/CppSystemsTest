//
// Created by emi on 10/25/26.
//

#include "local_player_turn_hook.hpp"

#include "events/local_player_turn_event.hpp"
#include "system/signatures/basic_signature_system.hpp"
void local_player_turn_hook::detour(LocalPlayer* self, glm::vec2* rots) {
    local_player_turn_event event(self, rots);
    m_events->dispatcher().post(event);
    if (event.is_canceled()) return;

    original(self, rots);
}
std::uintptr_t local_player_turn_hook::resolve_target() {
    return manager->get<signature_system>()->sigs().LocalPlayer.localPlayerTurn;
}
void local_player_turn_hook::on_initialize() {
    m_events = manager->get<event_system>();
}
