//
// Created by emi on 10/20/26.
//

#include "freelook.hpp"
#include "core/core.hpp"
#include "events/base_tick_event.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/deps/core/utility/NonOwnerPointer.hpp"
#include "sdk/mc/deps/minecraft_camera/CameraClientInstanceComponent.hpp"
#include "sdk/mc/deps/minecraft_camera/CameraRegistry.hpp"
#include "sdk/mc/deps/minecraft_camera/ICameraClientInstance.hpp"
#include "sdk/mc/entity/components/ActorHeadRotationComponent.hpp"
#include "sdk/mc/entity/components/FlagComponent.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/FloatOption.hpp"


#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/EnumOption.hpp"

#include <spdlog/spdlog.h>

#include "events/local_player_turn_event.hpp"
#include "sdk/mc/entity/components/CameraOrbitComponent.hpp"

void freelook::on_enable() {
    dispatcher().listen<local_player_turn_event, &freelook::on_lp_turn>(this);

    ClientInstance* ci = core::get_client_instance();
    auto player = ci->getLocalPlayer();
    if (!player) {
        disable();
        return;
    }

    auto gock = player->getActorHeadRotationComponent();
    mHeadYaw = {gock->mYHeadRot, gock->mYHeadRotO};

    std::size_t cameraCount = 0;
    std::size_t removedUpdateComponents = 0;
    for (auto&& [id, cameraComponent]: player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraComponent>().each()) {
        ++cameraCount;

        auto storage = player->mContext.find_storage<VanillaCamera::UpdatePlayerFromCameraComponent>();
        if (storage->contains(id)) {
            mCameras[id] = storage->get(id).mUpdateMode;

            storage->remove(id);
            ++removedUpdateComponents;
        }
    }

    for (auto&& [id, cameraComponent]:
         player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraDirectLookComponent>().each()) {
        mOriginalRotRads[id] = cameraComponent.mRotRads;
    }

    for (auto&& [id, cameraComponent]:
         player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraOrbitComponent>().each()) {
        mOrbitRotsC[id] = cameraComponent.mCurrentSpherical;
        mOrbitRotsI[id] = cameraComponent.mIdealSpherical;
    }


    this->mThirdPerson = static_cast<EnumOption*>(
            core::get_client_instance()->getOptions().getIfValid(OptionID::GameThirdperson).value());
    mLastCameraState = mThirdPerson->mValue;
    mThirdPerson->mValue = 1;
    spdlog::info("[freelook] on_enable: saved camera state={}; complete", mLastCameraState);
}

void freelook::on_disable() {
    dispatcher().deafen<local_player_turn_event, &freelook::on_lp_turn>(this);
    auto player = core::get_client_instance()->getLocalPlayer();
    if (!player) {
        return;
    }

    std::size_t cameraCount = 0;
    std::size_t restoredUpdateComponents = 0;
    for (auto&& [id, cameraComponent]: player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraComponent>().each()) {
        ++cameraCount;

        auto storage = player->mContext.find_storage<VanillaCamera::UpdatePlayerFromCameraComponent>();
        if (!storage->contains(id)) {
            auto it = mCameras.find(id);
            if (it == mCameras.end()) {
                continue;
            }

            storage->emplace(id, VanillaCamera::UpdatePlayerFromCameraComponent(it->second));
            ++restoredUpdateComponents;
        }

        ++restoredUpdateComponents;
    }

    for (auto&& [id, cameraComponent]:
         player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraDirectLookComponent>().each()) {
        if (!mOriginalRotRads.contains(id)) {
            spdlog::warn("[freelook] on_disable: no original rotation found for camera id={}", id.rawId);
            continue;
        }

        cameraComponent.mRotRads = mOriginalRotRads[id];
    }

    for (auto&& [id, cameraComponent]:
         player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraOrbitComponent>().each()) {
        if (!mOrbitRotsC.contains(id) || !mOrbitRotsI.contains(id)) {
            spdlog::warn("[freelook] on_disable: no original orbit rotation found for camera id={}", id.rawId);
            continue;
        }

        cameraComponent.mCurrentSpherical = mOrbitRotsC[id];
        cameraComponent.mIdealSpherical = mOrbitRotsI[id];
    }

    mThirdPerson->mValue = mLastCameraState;
}

void freelook::on_lp_turn(local_player_turn_event& event) {
    glm::vec2 rot = glm::vec2(event.deltaRots->y, event.deltaRots->x);

    const glm::vec2 deltaRads = glm::radians(rot);


    for (auto&& [id, cameraComponent]:
         event.player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraDirectLookComponent>().each()) {
        glm::vec2 newRot = glm::vec2(-deltaRads.x, deltaRads.y);
        cameraComponent.mRotRads += newRot;
    }

    for (auto&& [id, cameraComponent]:
         event.player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraOrbitComponent>().each()) {
        cameraComponent.mCurrentSpherical.mAzimuth += deltaRads.x;
        cameraComponent.mCurrentSpherical.mPolarAngle =
                std::clamp(cameraComponent.mCurrentSpherical.mPolarAngle + deltaRads.y, 0.0f, glm::pi<float>());

        cameraComponent.mIdealSpherical = cameraComponent.mCurrentSpherical;
    }

    event.cancel();
}