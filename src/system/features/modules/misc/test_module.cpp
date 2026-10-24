//
// Created by emi on 10/16/26.
//

#include "test_module.hpp"

#include <fstream>

#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/entity/components/MoveInputComponent.hpp"
#include "core/core.hpp"
#include "sdk/mc/options/option_types/OptionID.hpp"
#include "sdk/mc/world/actor/Actor.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/Option.hpp"
#include <nlohmann/json.hpp>

#include "events/base_tick_event.hpp"
#include "events/render_event.hpp"
#include "sdk/mc/entity/components/ActorHeadRotationComponent.hpp"
#include "sdk/mc/entity/components/CameraComponent.hpp"
#include "sdk/mc/entity/components/CameraDirectLookComponent.hpp"
#include "sdk/mc/options/option_types/FloatOption.hpp"
#include "util/option_dumper/option_dumper.hpp"

#include <imgui.h>

#include "util/keyboard.hpp"

void test_module::on_enable() {
    spdlog::info("test module enabled");
    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp) return;

    //option_dumper::dump_options();
    /*auto fovSetting = static_cast<FloatOption*>(ci->getOptions().getIfValid(OptionID::GfxFieldOfView).value());
    spdlog::info("VALUE_MIN={} VALUE_MAX={} mValue={} mDefaultValue={} DELTA={}", fovSetting->VALUE_MIN, fovSetting->VALUE_MAX, fovSetting->mValue, fovSetting->mDefaultValue, fovSetting->DELTA);*/

    dispatcher().listen<base_tick_event, &test_module::on_base_tick>(this);
    dispatcher().listen<render_event, &test_module::on_render>(this);
}

void test_module::on_disable() {
    spdlog::info("test module disabled");

    dispatcher().deafen<base_tick_event, &test_module::on_base_tick>(this);
    dispatcher().deafen<render_event, &test_module::on_render>(this);
}

void test_module::on_render(render_event &event) {

    ImGui::Begin("test_module");
    ImGui::Text("test_module");
    std::string_view escape_key_text = keyboard::key_to_string(keyboard::key::escape);
    std::string_view capslock = keyboard::key_to_string(keyboard::key::caps_lock);
    ImGui::Text("Escape: %s", escape_key_text.data());
    ImGui::Text("Caps Lock: %s", capslock.data());

    ImGui::End();
    static bool open = true;
    ImGui::ShowDemoWindow(&open);
}

void test_module::on_base_tick(base_tick_event &event) {

    /*auto lp = static_cast<LocalPlayer *>(event.actor);
    auto headRotComp = lp->mContext.tryGetComponent<ActorHeadRotationComponent>();
    if (!headRotComp.has_value()) {
        spdlog::warn("[test] ActorHeadRotationComponent not found..?");
        return;
    }

    spdlog::info("[test] lp:{:X} headRotComponent:{:X} mYHeadRot={} mYHeadRotO={}",
                 reinterpret_cast<std::uintptr_t>(lp), reinterpret_cast<std::uintptr_t>(&headRotComp.value()),
                 headRotComp->mYHeadRot, headRotComp->mYHeadRotO);


    for (auto &&[id, component]: lp->mContext.mEnTTRegistry.view<MinecraftCamera::CameraDirectLookComponent>().each()) {
        MinecraftCamera::CameraDirectLookComponent *compPtr = &component;
        spdlog::info("id:{:X} CameraDirectLookComponent:{:X} mRotRads=({}, {}) mDelta={} mWrap=({}, {})", id.rawId,
                     reinterpret_cast<std::uintptr_t>(compPtr), compPtr->mRotRads.x, compPtr->mRotRads.y,
                     compPtr->mDelta, compPtr->mWrap.x, compPtr->mWrap.y);
    }

    // CameraComponent
    for (auto&&[i, component]: lp->mContext.mEnTTRegistry.view<MinecraftCamera::CameraComponent>().each()) {
        MinecraftCamera::CameraComponent* compPtr = &component;
        spdlog::info("id:{:X} CameraComponent:{:X} mId={} mOrientation=({}, {}, {}, {}) mPosition=({}, {}, {}) mAspectRatio={} mFieldOfView={} mNearPlane={} mFarPlane={}",
                     i.rawId, reinterpret_cast<std::uintptr_t>(compPtr), compPtr->mId.mStr,
                     compPtr->mOrientation.x, compPtr->mOrientation.y, compPtr->mOrientation.z, compPtr->mOrientation.w,
                     compPtr->mPosition.x, compPtr->mPosition.y, compPtr->mPosition.z,
                     compPtr->mAspectRatio, compPtr->mFieldOfView, compPtr->mNearPlane, compPtr->mFarPlane);
    }

    spdlog::info("[test] ok");*/
}

void test_module::on_manager_init() {
    spdlog::info("test module initialized");
}

void test_module::on_manager_exit() {
    spdlog::info("test module exited");
}
