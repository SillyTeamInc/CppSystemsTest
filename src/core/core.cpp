//
// Created by emi on 10/16/26.
//

#include "core.hpp"
#include <sdk/mc/client/game/ClientInstance.hpp>
#include <sdk/mc/client/game/MinecraftGame.hpp>
#include <sdk/mc/deps/application/gamecore/WinMain.hpp>
#include <sdk/mc/deps/application/gamecore/Platform_GameCore.hpp>
#include <sdk/mc/deps/application/gamecore/GameHolder.hpp>

namespace {
    bool is_readable(void* ptr) {
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQuery(ptr, &mbi, sizeof(mbi))) {
            return (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)) != 0;
        }
        return false;
    }
}

MinecraftGame* core::get_minecraft_game() {
    // null checks shouldnt be an issue here as we wait for the window to be created
    // before initializing. from what i saw in-game, this path at least is assumed to exist
    // at window creation time. unless i get horribly unlucky, it should not matter :3
    return MainGameCore::WinMain::get()->mPlatform->mHolder->mGame;
}

ClientInstance* core::get_client_instance() {
    static ClientInstance* sci = nullptr;
    if (sci != nullptr) return sci;

    auto c = MainGameCore::WinMain::get()->mPlatform->mHolder->mGame->getPrimaryClientInstance();
    // Only an issue on startup
    if (!is_readable(c)) {
        spdlog::error("ClientInstance pointer is not readable: {:X}", reinterpret_cast<std::uintptr_t>(sci));
        return nullptr;
    }
    sci = c;
    return sci;
}
