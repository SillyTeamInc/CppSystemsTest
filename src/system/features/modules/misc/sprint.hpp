//
// Created by emi on 10/17/26.
//

#pragma once

#include "../module.hpp"

class base_tick_event;

class sprint : public module_base<sprint> {
public:
    static constexpr module_info info{"Sprint", "Automatically sprints", category::misc, 0, true};

    explicit sprint() : module_base(info) {}

    void on_enable() override;
    void on_disable() override;
    void on_base_tick(base_tick_event& event);
};
