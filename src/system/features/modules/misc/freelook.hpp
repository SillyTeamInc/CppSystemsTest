//
// Created by emi on 10/20/26.
//

#pragma once

#include "../module.hpp"
#include <windows.h>

class freelook : public module_base<freelook> {
public:
    static constexpr module_info info {"Freelook", "Yeah", category::misc, static_cast<int>(keyboard::key::f6), false, false};

    explicit freelook() : module_base(info) {}

    void on_enable() override;
    void on_disable() override;
    void on_base_tick(base_tick_event& event);
};
