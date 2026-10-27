//
// Created by emi on 10/27/2026.
//

#pragma once

#include <system/hooks/base/hook.hpp>

class LevelRendererPlayer;

// float __fastcall LevelRendererPlayer::getFov(struct LevelRendererPlayer *this, float a, bool enableVariableFov)
class get_fov_hook : public hook<get_fov_hook, float(LevelRendererPlayer*, float, bool)> {
    event_system* m_events = nullptr;

public:
    get_fov_hook() : hook("LevelRendererPlayer::getFov") {}
    float detour(LevelRendererPlayer* self, float a, bool enableVariableFov);

protected:
    std::uintptr_t resolve_target() override;
    void on_initialize() override;
};
