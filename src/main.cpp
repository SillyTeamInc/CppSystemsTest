#include <iostream>
#include <windows.h>

#include "core/core.hpp"
#include "hvs/hvs.hpp"
#include "system/systems_manager.hpp"
#include "system/complex_system.hpp"
#include "system/core_system.hpp"
#include "system/generic_system.hpp"
#include "system/event_system.hpp"
#include "system/features/feature_system.hpp"
#include "system/signatures/basic_signature_system.hpp"
#include "system/hooks/base/hooks.hpp"

#include "system/hooks/base/hook_system.hpp"

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "sdk/mc/deps/application/gamecore/Platform_GameCore.hpp"
#include "sdk/mc/deps/application/gamecore/WinMain.hpp"
#include "sdk/mc/deps/application/gamecore/AppPlatform_GameCore.hpp"
#include "sdk/mc/deps/application/gamecore/GameHolder.hpp"
#include "sdk/mc/client/game/MinecraftGame.hpp"

void main_thread(HMODULE hModule) {
    core::init_logging(spdlog::level::trace);
    spdlog::info("hello world!!!");

    const auto manager = systems_manager::i();
    auto* coreSys = manager->add_system<core_system>(hModule);

    auto* eventSys = manager->add_system<event_system>(); // the dispatcher is available at add time heres :3
    auto* sigSys = manager->add_system<signature_system>();
    manager->add_system<hook_system>();
    manager->add_system<feature_system>();

    manager->initialize_systems();

    spdlog::info("getLocalPlayer addr={:X}", sigSys->sigs().ClientInstance.getLocalPlayer.value());

    MainGameCore::WinMain* winMain = MainGameCore::WinMain::get();
    Bedrock::Platform_GameCore* platform = winMain->mPlatform;
    Bedrock::GameHolder* holder = platform->mHolder;
    MinecraftGame* game = holder->mGame;
    ClientInstance* ci = game->getPrimaryClientInstance();
    spdlog::info("MainGameCore::WinMain instance={:X}", reinterpret_cast<std::uintptr_t>(winMain));
    spdlog::info("Bedrock::Platform_GameCore mPlatform_Shim={:X}", reinterpret_cast<std::uintptr_t>(platform));
    spdlog::info("Bedrock::GameHolder mGame={:X}", reinterpret_cast<std::uintptr_t>(holder));
    spdlog::info("MinecraftGame instance={:X}", reinterpret_cast<std::uintptr_t>(game));
    spdlog::info("ClientInstance={:X}", reinterpret_cast<std::uintptr_t>(ci));
    spdlog::info("ClientInstance from core={:X}", reinterpret_cast<std::uintptr_t>(core::get_client_instance()));
    spdlog::info("press end to eject the dll");

    while (!coreSys->m_eject_requested) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    manager->shutdown_systems();

    spdlog::info("All cleaned up, cya!");
    core::shutdown_logging();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(main_thread), hModule, 0, nullptr);
    }

    return TRUE;
}
