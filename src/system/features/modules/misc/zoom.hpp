//
// Created by emi on 10/26/26.
//

#pragma once

#include "system/features/modules/module.hpp"

class render_event;
class local_player_turn_event;
class moused_event;
class FloatOption;
namespace fov_check_event { class post; }

class zoom : public module_base<zoom> {
public:
    static constexpr float TARGET = 0.40f;

    static constexpr module_info info {"Zoom", "Zooming everywhere", category::misc, static_cast<int>(keyboard::key::c), false, false};

    explicit zoom() : module_base(info) {}

    float m_target_mult = 1.f;;
    float m_current_mult = 1.f;

    void on_enable() override;
    void on_disable() override;
    void on_fov_event(fov_check_event::post& event);
    void on_mouse_event(moused_event& event);
    void on_lp_turn(local_player_turn_event& event);
    void on_render(render_event& event);
    bool can_be_toggled() override;

    void on_manager_init() override;
    void on_manager_exit() override;
};
