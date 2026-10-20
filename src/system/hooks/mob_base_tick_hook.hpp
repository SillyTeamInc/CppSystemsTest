//
// Created by emi on 10/17/26.
//

#pragma once

#include <cstdint>

#include "system/event_system.hpp"
#include "system/hooks/base/hook.hpp"

class Mob;
class Actor;

class mob_base_tick_hook : public hook<mob_base_tick_hook, void(Mob*)> {
    event_system* m_events = nullptr;

public:
    mob_base_tick_hook() : hook("Actor::baseTick") {}
    void detour(Mob* self);

    //HOOK_STARTS_DISABLED
    HOOK_RETRIES_UNTIL_RESOLVED
protected:
    std::uintptr_t resolve_target() override;
    void on_initialize() override;
};
