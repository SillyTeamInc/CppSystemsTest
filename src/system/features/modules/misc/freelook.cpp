//
// Created by emi on 10/20/26.
//

#include "freelook.hpp"
#include "events/base_tick_event.hpp"
#include "sdk/mc/options/option_types/FloatOption.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "core/core.hpp"
#include "sdk/mc/deps/minecraft_camera/CameraClientInstanceComponent.hpp"
#include "sdk/mc/deps/minecraft_camera/ICameraClientInstance.hpp"
#include "sdk/mc/deps/core/utility/NonOwnerPointer.hpp"
#include "sdk/mc/deps/minecraft_camera/CameraRegistry.hpp"

#include "sdk/mc/options/IOptionsReader.hpp"

void freelook::on_enable() {
    dispatcher().listen<base_tick_event, &freelook::on_base_tick>(this);
}

void freelook::on_disable() {
    dispatcher().deafen<base_tick_event, &freelook::on_base_tick>(this);
}

void freelook::on_base_tick(base_tick_event& event) {
    //ICameraClientInstance
    auto lp = static_cast<LocalPlayer*>(event.actor);

    spdlog::info("LocalPlayer id:{:X}, mContext id:{:X}", reinterpret_cast<std::uintptr_t>(lp), lp->mContext.mEntity.rawId);

    for (auto&&[i, component]: lp->mContext.mEnTTRegistry.view<CameraClientInstanceComponent>().each()) {
        spdlog::info("CameraClientInstanceComponent id:{:X} CameraClientInstanceComponent:{:X}", i.rawId, reinterpret_cast<std::uintptr_t>(&component));
    }


    ClientInstance* ci = core::get_client_instance();

    Bedrock::NonOwnerPointer<CameraRegistry> reg = ci->getCameraRegistry();
    spdlog::info("c->mGameCamera id:{:X}", reg->mGameCamera->rawId);
    for (OwnerPtr<EntityId> a: reg->mCameraEntities) {
        spdlog::info("CameraEntity id:{:X}", a->rawId);
    }
}
