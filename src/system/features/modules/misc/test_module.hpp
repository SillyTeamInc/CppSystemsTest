//
// Created by emi on 10/16/26.
//
#pragma once

#include "../module.hpp"
#include "events/base_tick_event.hpp"

class test_module : public module_base<test_module> {
public:
    static constexpr module_info info {"Test", "Example module", category::misc, VK_F3};

    explicit test_module() : module_base(info) {}

    void on_enable() override;
    void on_disable() override;

    void on_base_tick(base_tick_event &event);

    void on_manager_init() override;
    void on_manager_exit() override;
};
