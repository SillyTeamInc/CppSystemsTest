//
// Created by emi on 10/15/26.
//
#pragma once
#include "core/sdk_base.hpp"
#include "util/memory.hpp"

class Actor;



class IClientInstance {
    // NOLINTBEGIN
private:
    VIRT_PAD // Accounting for the base class
public:
    // i want client symbols so bad >:c i feel bad referencing levi
    // this isn't supposed to be a cheat so whatevers
    // https://discord.com/users/921648530769182740 please!!!!
    virtual ~IClientInstance() = default;
};

// note that this doesn't include all of the base classes
class ClientInstance : public IClientInstance, SDK_CLASS {
public:
    Actor* getLocalPlayer() {
        return util::memory::call_func<Actor*>(sigs().ClientInstance.getLocalPlayer.v(), reinterpret_cast<std::uintptr_t>(this));
    }
};
