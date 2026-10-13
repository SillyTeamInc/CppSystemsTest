//
// Created by emi on 10/9/26.
//
#pragma once

#include <system/generic_system.hpp>

#include "events/moused_event.hpp"
#include "events/keyboard_event.hpp"

class module_system : public generic_system {

public:
    hvs::event_dispatcher& dispatcher() {
        return manager->get<event_system>()->dispatcher();
    }

    void on_mouse_event(moused_event& event) {
        if (event.action_button_id == 0) return;
        spdlog::info("mouse event received, x: {}, y: {}, dx: {}, dy: {}, actionButtonId: {}, buttonData: {}, forceMotionlessPointer: {}",
                     event.x, event.y, event.dx, event.dy, event.action_button_id, event.button_data, event.force_motionless_pointer);
    }

    void on_keyboard_event(keyboard_event& event) {
        spdlog::info("keyboard event received, {}", event.to_string());
    }

    void initialize() override {
        spdlog::info("Initializing module system");
        dispatcher().listen<moused_event, &module_system::on_mouse_event>(this);
        dispatcher().listen<keyboard_event, &module_system::on_keyboard_event>(this);
    }

    void shutdown() override {
        spdlog::info("Shutting down module system");
        dispatcher().deafen<moused_event, &module_system::on_mouse_event>(this);
        dispatcher().deafen<keyboard_event, &module_system::on_keyboard_event>(this);
    }
};
