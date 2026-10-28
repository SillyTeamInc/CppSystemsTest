//
// Created by emi on 10/28/26.
//

#pragma once
#include "system/features/modules/module.hpp"
#include "util/keyboard.hpp"

class base_tick_event;
class local_player_turn_event;

class freecam : public module_base<freecam> {
public:
    static constexpr module_info info{"Freecam", "Cam of the free", category::misc,
                                      static_cast<int>(keyboard::key::grave_accent), true};

    explicit freecam() : module_base(info) {}

    void on_enable() override;
    void on_disable() override;
    void on_base_tick(base_tick_event& event);
    void on_lp_turn(local_player_turn_event& event);
};
