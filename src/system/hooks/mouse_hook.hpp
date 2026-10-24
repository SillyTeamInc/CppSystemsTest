//
// Created by emi on 10/12/2026.
//
#pragma once

#include <cstdint>

#include "system/hooks/base/hook.hpp"

class event_system;

class mouse_hook : public hook<mouse_hook, void(void*, std::uint8_t, int, short, short, short, short, std::uint8_t)> {
    event_system* m_events = nullptr;

public:
    mouse_hook() : hook("MouseDevice::feed") {}

    void detour(void* self, std::uint8_t actionButtonId, int buttonData, short x, short y, short dx, short dy,
                std::uint8_t forceMotionlessPointer);

protected:
    std::uintptr_t resolve_target() override;
    void on_initialize() override;
};
