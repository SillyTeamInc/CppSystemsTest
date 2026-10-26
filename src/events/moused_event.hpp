//
// Created by emi on 10/12/2026.
//
#pragma once
#include "hvs/hvs.hpp"

class moused_event : public hvs::cancelable_event {
public:
    void* mouse_device;
    uint8_t action_button_id;
    int button_data;
    short x;
    short y;
    short dx;
    short dy;
    bool force_motionless_pointer;

    moused_event(void* this_, uint8_t action_button_id, int button_data, short x, short y, short dx, short dy, bool force_motionless_pointer)
        : mouse_device(this_), action_button_id(action_button_id), button_data(button_data), x(x), y(y), dx(dx), dy(dy), force_motionless_pointer(force_motionless_pointer) {}

    std::string to_string() {
        return fmt::format("MousedEvent: mouse_device = {}, action_button_id = {}, button_data = {}, x = {}, y = {}, dx = {}, dy = {}, force_motionless_pointer = {}",
            mouse_device, action_button_id, button_data, x, y, dx, dy, force_motionless_pointer);
    }
};