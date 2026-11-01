//
// Created by emi on 10/28/26.
//

#pragma once
#include "system/features/modules/module.hpp"
#include "util/keyboard.hpp"

class base_tick_event;
class local_player_turn_event;
class module_state_change_event;
class stationary_camera_event;
class cull_event;

class EnumOption;
class render_event;
class moused_event;
class keyboard_event;

class freecam : public module_base<freecam> {
public:
    EnumOption* m_third_person_opt = nullptr;
    int m_last_camera_state = 0;
    std::unordered_map<EntityId, glm::vec2> m_og_rot_rads;
    std::unordered_map<EntityId, int> m_cams{};
    std::unordered_map<EntityId, Spherical> m_orbit_rots_c{};
    std::unordered_map<EntityId, Spherical> m_orbit_rots_i{};
    glm::vec3 m_pos;
    glm::vec3 m_last_pos;

    glm::vec2 m_rot;

    static constexpr float SPEED = 0.8f;

    float m_current_speed = SPEED;

    static constexpr module_info info{"Freecam", "Cam of the free", category::misc,
                                      static_cast<int>(keyboard::key::grave_accent), false};

    explicit freecam() : module_base(info) {}

    void on_enable() override;
    void on_disable() override;
    void on_cull(cull_event& event);
    void on_mouse(moused_event& event);
    void on_keyboard(keyboard_event& event);
    void on_module_state_change(module_state_change_event& event);
    void on_base_tick(base_tick_event& event);
    void on_render(render_event& event);
    void on_sc_tick(stationary_camera_event& event);
    void on_lp_turn(local_player_turn_event& event);
};
