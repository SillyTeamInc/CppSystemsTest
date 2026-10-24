//
// Created by emi on 10/20/26.
//

#pragma once

#include "../module.hpp"

class FloatOption;
class base_tick_event;

class fullbright : public module_base<fullbright> {
public:
    static constexpr module_info info {"Fullbright", "Destroy darkness", category::misc, static_cast<int>(keyboard::key::numpad_0), true};

    explicit fullbright() : module_base(info) {}

    float m_prev_gamma = 1.0f;
    FloatOption* gamma = nullptr;

    void on_enable() override;
    void on_disable() override;
    void on_base_tick(base_tick_event& event);
};
