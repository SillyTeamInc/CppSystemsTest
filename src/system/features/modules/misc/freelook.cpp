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

    std::size_t cameraCount = 0;
    std::size_t removedUpdateComponents = 0;
    for (auto&& [id, cameraComponent]: player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraComponent>().each()) {
        ++cameraCount;

        auto storage = player->mContext.find_storage<VanillaCamera::UpdatePlayerFromCameraComponent>();
        if (storage->contains(id)) {
            m_cams[id] = storage->get(id).mUpdateMode;

            storage->remove(id);
            ++removedUpdateComponents;
        }
    }

    for (auto&& [id, cameraComponent]:
         player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraDirectLookComponent>().each()) {
        m_og_rot_rads[id] = cameraComponent.mRotRads;
    }

    for (auto&& [id, cameraComponent]:
         player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraOrbitComponent>().each()) {
        m_orbit_rots_c[id] = cameraComponent.mCurrentSpherical;
        m_orbit_rots_i[id] = cameraComponent.mIdealSpherical;
    }


    this->m_third_person_opt = static_cast<EnumOption*>(
            core::get_client_instance()->getOptions().getIfValid(OptionID::GameThirdperson).value());
    m_last_camera_state = m_third_person_opt->mValue;
    m_third_person_opt->mValue = 1;
    log->info("freelook enabled, removed {} UpdatePlayerFromCameraComponents, camera count: {}", removedUpdateComponents,
              cameraCount);
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
            auto it = m_cams.find(id);
            if (it == m_cams.end()) {
                continue;
            }

            storage->emplace(id, VanillaCamera::UpdatePlayerFromCameraComponent(it->second));
            ++restoredUpdateComponents;
        }

        ++restoredUpdateComponents;
    }

    for (auto&& [id, cameraComponent]:
         player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraDirectLookComponent>().each()) {
        if (!m_og_rot_rads.contains(id)) {
            log->warn("on_disable: no original rotation found for camera id={}", id.rawId);
            continue;
        }

        cameraComponent.mRotRads = m_og_rot_rads[id];
    }

    for (auto&& [id, cameraComponent]:
         player->mContext.mEnTTRegistry.view<MinecraftCamera::CameraOrbitComponent>().each()) {
        if (!m_orbit_rots_c.contains(id) || !m_orbit_rots_i.contains(id)) {
            log->warn("on_disable: no original orbit rotation found for camera id={}", id.rawId);
            continue;
        }

        cameraComponent.mCurrentSpherical = m_orbit_rots_c[id];
        cameraComponent.mIdealSpherical = m_orbit_rots_i[id];
    }


    m_third_person_opt->mValue = m_last_camera_state;
    log->info("freelook disabled and restored");
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