//
// Created by emi on 10/16/26.
//

#include "core.hpp"
#include <sdk/mc/client/game/ClientInstance.hpp>
#include <sdk/mc/client/game/MinecraftGame.hpp>
#include <sdk/mc/deps/application/gamecore/WinMain.hpp>
#include <sdk/mc/deps/application/gamecore/Platform_GameCore.hpp>
#include <sdk/mc/deps/application/gamecore/GameHolder.hpp>

MinecraftGame* core::get_minecraft_game() {
    return MainGameCore::WinMain::get()->mPlatform->mHolder->mGame;
}

ClientInstance* core::get_client_instance() {
    return MainGameCore::WinMain::get()->mPlatform->mHolder->mGame->getPrimaryClientInstance();
}
