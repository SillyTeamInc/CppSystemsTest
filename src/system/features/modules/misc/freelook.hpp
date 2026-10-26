//
// Created by emi on 10/20/26.
//

#pragma once

#include <util/keyboard.hpp>
#include <windows.h>
#include "../module.hpp"
#include "events/local_player_turn_event.hpp"
#include "sdk/mc/deps/minecraft_camera/Spherical.hpp"


class EnumOption;

class freelook : public module_base<freelook> {
    EnumOption* m_third_person_opt = nullptr;

    int m_last_camera_state = 0;
    std::unordered_map<EntityId, glm::vec2> m_og_rot_rads;
    std::unordered_map<EntityId, int> m_cams{};
    std::unordered_map<EntityId, Spherical> m_orbit_rots_c{};
    std::unordered_map<EntityId, Spherical> m_orbit_rots_i{};
public:
    static constexpr module_info info {"Freelook", "Yeah", category::misc, static_cast<int>(keyboard::key::f), false, false};

    explicit freelook() : module_base(info) {}





    void on_enable() override;
    void on_disable() override;
    void on_lp_turn(local_player_turn_event& event);
};
