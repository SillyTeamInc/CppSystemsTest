//
// Created by emi on 10/31/26.
//

#pragma once
#include "base/hook.hpp"

class stationary_camera_hook : public hook<stationary_camera_hook, void*(void*, void*)> {
    event_system* m_events = nullptr;

public:
    stationary_camera_hook() : hook("StationaryCameraSystem::tick") {}
    void* detour(void* self, void* regRef);

protected:
    std::uintptr_t resolve_target() override;
    void on_initialize() override;
};
