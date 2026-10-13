//
// Created by emi on 10/9/26.
//
#pragma once

#include <system/generic_system.hpp>
#include <hvs/hvs.hpp>

// owns the event_dispatcher and requires no init so its available at add time
class event_system : public generic_system {
    hvs::event_dispatcher m_dispatcher;
public:
    void initialize() override {
        spdlog::info("Initializing event system");
    }
    void shutdown() override {
        spdlog::info("Shutting down event system");
    }

    hvs::event_dispatcher& dispatcher() { return m_dispatcher; }
};

