//
// Created by emi on 11/1/26.
//

#pragma once
#include "base/hook.hpp"
#include "util/types.hpp"


class culling_hook : public hook<culling_hook, void*(void*, uint8 type)> {
    event_system* m_events = nullptr;

public:
    culling_hook() : hook("LevelRendererCameraListeners::updateLevelCullerType") {}

    void* detour(void* self, uint8 type);

protected:
    std::uintptr_t resolve_target() override;
    void on_initialize() override;
};
