#include <iostream>
#include <windows.h>

#include "core/log.hpp"
#include "hvs/hvs.hpp"
#include "system/systems_manager.hpp"
#include "system/complex_system.hpp"
#include "system/core_system.hpp"
#include "system/generic_system.hpp"
#include "system/event_system.hpp"
#include "system/features/feature_system.hpp"
#include "system/signatures/basic_signature_system.hpp"
#include "system/hooks/base/hooks.hpp"
#include <GameInput.h>

#include "system/hooks/base/hook_system.hpp"

// i don't know how to dev for this on linux so i have to use a vm running on a 3rd gen i7-3770s give me mercy

void main_thread(HMODULE hModule) {
    core::init_logging(spdlog::level::trace);
    spdlog::info("hello world!!!");

    // Wait for debugger to attach
    /*while (!IsDebuggerPresent()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }*/

    const auto manager = std::make_unique<systems_manager>();

    auto* coreSys = manager->add_system<core_system>(hModule);

    manager->add_system<event_system>(); // the dispatcher is available at add time heres :3
    auto* sigSys = manager->add_system<signature_system>();
    manager->add_system<hook_system>();
    manager->add_system<feature_system>();

    manager->initialize_systems();
    spdlog::info("getLocalPlayer addr={:X}", sigSys->sigs().ClientInstance.getLocalPlayer.value());


    while (!coreSys->m_eject_requested) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    manager->shutdown_systems();

    spdlog::info("All cleaned up, cya!");


    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

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
