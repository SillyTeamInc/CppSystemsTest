//
// Created by emi on 10/15/26.
//
#pragma once
#include <map>

#include "../../../../core/sdk_base.hpp"

class ClientInstance;

class MinecraftGame : SDK_CLASS {
public:
    struct ClientInterface {
        void* mUnk; // Literally no clue
        std::shared_ptr<ClientInstance> mClientInstance;
    };

    SDK_FIELD(mClientInstances, (sigs().MinecraftGame.mClientInstances.v()), std::map<uint8_t, ClientInterface>);

    ClientInstance* getPrimaryClientInstance() {
        // technically inaccurate since im supposed to check ci->isPrimaryClient() as well but who cares
        if (mClientInstances.empty()) {
            spdlog::warn("MinecraftGame::getPrimaryClientInstance() called but mClientInstances is empty");
            return nullptr;
        }
        return mClientInstances.begin()->second.mClientInstance.get();
    }
};
