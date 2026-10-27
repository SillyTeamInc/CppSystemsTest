#include <imgui.h>
#include <iostream>


#include <windows.h>

#include "core/logging.hpp"
#include "hvs/hvs.hpp"
#include "system/complex_system.hpp"
#include "system/core_system.hpp"
#include "system/event_system.hpp"
#include "system/features/feature_system.hpp"
#include "system/generic_system.hpp"
#include "system/hooks/base/hooks.hpp"
#include "system/signatures/basic_signature_system.hpp"
#include "system/systems_manager.hpp"

#include "system/hooks/base/hook_system.hpp"

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <core/core.hpp>
#include "events/mod_tick_event.hpp"
#include "events/render_event.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/client/game/MinecraftGame.hpp"
#include "sdk/mc/deps/application/gamecore/AppPlatform_GameCore.hpp"
#include "sdk/mc/deps/application/gamecore/GameHolder.hpp"
#include "sdk/mc/deps/application/gamecore/Platform_GameCore.hpp"
#include "sdk/mc/deps/application/gamecore/WinMain.hpp"
#include "sdk/mc/entity/components/MoveInputComponent.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/world/actor/Actor.hpp"
#include "system/render/render_system.hpp"
#include <embedded/resources.hpp>

#include "sdk/mc/client/renderer/game/LevelRenderer.hpp"
#include "util/exceptions.hpp"

class injected_notification {
    systems_manager* manager = nullptr;
public:
    injected_notification(systems_manager* mgr) : manager(mgr) {}

    hvs::event_dispatcher& dispatcher() const {
        return manager->get<event_system>()->dispatcher();
    }

    void on_injected() {
        m_injection_time = std::chrono::high_resolution_clock::now();

        dispatcher().listen<render_event, &injected_notification::on_render>(this);
    }

    void on_render(render_event& event) {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_injection_time).count();
        constexpr int64 hold_duration = 3000;
        constexpr int64 fade_duration = 2000;
        constexpr int64 max_duration = hold_duration + fade_duration;
        float alpha = duration <= hold_duration ? 1.0f : 1.0f - static_cast<float>(duration - hold_duration) / fade_duration;\

        if (alpha <= 0.0f) {
            dispatcher().deafen<render_event, &injected_notification::on_render>(this);
            spdlog::info("injected_notification ended");
            return;
        }

        auto drawList = ImGui::GetForegroundDrawList();

        constexpr auto text = "CppSystemsTest loaded";
        ImFont* font = ImGui::GetFont();
        ImVec2 textSize = font->CalcTextSizeA(22, FLT_MAX, 0.0f, text);
        ImVec2 pos = ImVec2(10, 10);
        ImVec2 size = ImVec2(textSize.x + 20, textSize.y + 20);
        ImU32 bgColor = IM_COL32(0, 0, 0, static_cast<ImU8>(alpha * 100));
        ImU32 textColor = IM_COL32(100, 255, 100, static_cast<ImU8>(alpha * 255));

        const ImVec2 bottomRight(pos.x + size.x, pos.y + size.y);
        drawList->AddRectFilled(pos, bottomRight, bgColor);
        drawList->PushClipRect(pos, bottomRight, true);
        drawList->AddText(ImVec2(pos.x + 10, pos.y + 10), textColor, text);
        drawList->PopClipRect();
    }

    std::chrono::high_resolution_clock::time_point m_injection_time = std::chrono::high_resolution_clock::now();
};

void main_thread(HMODULE hModule) {
    core::logging::init(spdlog::level::trace);
    spdlog::info("hello world!!!");
    util::exceptions::attach_handler();

    const auto manager = systems_manager::i();
    auto* coreSys = manager->add_system<core_system>(hModule);

    auto* eventSys = manager->add_system<event_system>(); // the dispatcher is available at add time heres :3
    auto* sigSys = manager->add_system<signature_system>();
    manager->add_system<hook_system>();
    manager->add_system<render_system>();
    manager->add_system<feature_system>();

    manager->initialize_systems();

    entt::type_hash<MoveInputComponent> typehash = entt::type_hash<MoveInputComponent>();
    constexpr auto stripped = entt::internal::stripped_type_name<MoveInputComponent>();
    spdlog::info("stripped type name: " + std::string(stripped));
    spdlog::info("type hash of MoveInputComponent: {:X}", typehash.value());

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


    void* lp = nullptr;

    spdlog::info("IClientInstance::tick index={:X}", kiero::detail::magic_vft::vtable_index<&ClientInstance::tick>());

    injected_notification notification(manager);
    notification.on_injected();

    {
        LocalPlayer* lp = ci->getLocalPlayer();
        if (lp) lp->displayClientMessage("CppSystemsTest loaded");
    }

    while (!coreSys->m_eject_requested) {
        auto l = ci->getLocalPlayer();
        if (lp != l) {
            lp = l;
            spdlog::info("localPlayer is now {:X}", reinterpret_cast<std::uintptr_t>(l));
            if (l) {
                auto* la = reinterpret_cast<Actor*>(l);
                bool hasComponent = la->mContext.hasComponent<MoveInputComponent>();
                auto mic = la->mContext.tryGetComponent<MoveInputComponent>();
                spdlog::info("has MoveInputComponent: {:X}", hasComponent);
            }
        }

        eventSys->dispatcher().post(mod_tick_event());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    manager->shutdown_systems();

    util::exceptions::remove_handler();

    {
        LocalPlayer* lp = ci->getLocalPlayer();
        if (lp) lp->displayClientMessage("CppSystemsTest unloaded");
    }

    spdlog::info("All cleaned up, cya!");
    core::logging::shutdown();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(main_thread), hModule, 0, nullptr);
    }

    return TRUE;
}
