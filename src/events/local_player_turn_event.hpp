//
// Created by emi on 10/25/26.
//

#pragma once
#include <glm/glm.hpp>
#include "hvs/hvs.hpp"

class LocalPlayer;
class local_player_turn_event : public hvs::cancelable_event {
public:
    LocalPlayer* player;
    // these rots are only valid at call time because it points to data on stack
    glm::vec2* deltaRots{};

    local_player_turn_event(LocalPlayer* player, glm::vec2* rots) : player(player), deltaRots(rots) {}
};
