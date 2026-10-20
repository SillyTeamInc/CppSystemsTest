//
// Created by emi on 10/17/26.
//

#pragma once
#include "hvs/hvs.hpp"
#include "sdk/mc/world/actor/Actor.hpp"

class base_tick_event : public hvs::event {
public:
    Mob* actor;
    base_tick_event(Mob* actor) : actor(actor) {}
};
