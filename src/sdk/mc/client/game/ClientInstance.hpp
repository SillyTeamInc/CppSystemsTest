//
// Created by emi on 10/15/26.
//
#pragma once
#include "../../../../core/sdk_base.hpp"
#include "util/memory.hpp"

class Actor;

class ClientInstance : SDK_CLASS {
public:
    Actor* getLocalPlayer() {
        return util::memory::call_func<Actor*>(sigs().ClientInstance.getLocalPlayer.v(), reinterpret_cast<std::uintptr_t>(this));
    }
};
