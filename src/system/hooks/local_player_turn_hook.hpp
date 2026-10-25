//
// Created by emi on 10/25/26.
//

#pragma once
#include "base/hook.hpp"
#include "glm/vec2.hpp"
#include "system/event_system.hpp"

class LocalPlayer;

class local_player_turn_hook : public hook<local_player_turn_hook, void(LocalPlayer*, glm::vec2*)> {
    event_system* m_events = nullptr;
public:
    local_player_turn_hook() : hook("LocalPlayer::localPlayerTurn") {}

    void detour(LocalPlayer* self, glm::vec2* rots);

protected:
    std::uintptr_t resolve_target() override;
    void on_initialize() override;
};
