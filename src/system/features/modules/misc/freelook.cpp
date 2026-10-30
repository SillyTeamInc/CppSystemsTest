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
#include "events/module_state_change_event.hpp"
#include "freecam.hpp"
#include "sdk/mc/entity/components/CameraOrbitComponent.hpp"
#include "system/features/modules/module_system.hpp"

void freelook::on_enable() {
    dispatcher().listen<local_player_turn_event, &freelook::on_lp_turn>(this);
    dispatcher().listen<module_state_change_event, &freelook::on_module_state_change>(this);

    ClientInstance* ci = core::get_client_instance();
    auto player = ci->getLocalPlayer();
    auto cameraRegistry = ci->getCameraRegistry();

    if (!player || !cameraRegistry) {
        disable();
        return;
    }

    std::size_t cameraCount = 0;
    std::size_t removedUpdateComponents = 0;
    auto storage = player->mContext.find_storage<VanillaCamera::UpdatePlayerFromCameraComponent>();
    for (OwnerPtr<EntityId> entity : cameraRegistry->mCameraEntities) {
        ++cameraCount;
        const auto id = entity->mEntity;

        if (storage->contains(id)) {
            m_cams[id] = storage->get(id).mUpdateMode;
            storage->remove(id);
            ++removedUpdateComponents;
        }

        if (auto directLook = entity->tryGetComponent<MinecraftCamera::CameraDirectLookComponent>()) {
            if (!directLook.has_value()) return;

            m_og_rot_rads[id] = directLook->mRotRads;
        }

        if (auto orbit = entity->tryGetComponent<MinecraftCamera::CameraOrbitComponent>()) {
            if (!orbit.has_value()) return;

            m_orbit_rots_c[id] = orbit->mCurrentSpherical;
            m_orbit_rots_i[id] = orbit->mIdealSpherical;
        }
    }


    this->m_third_person_opt = static_cast<EnumOption*>(
            core::get_client_instance()->getOptions().getIfValid(OptionID::GameThirdperson).value());
    m_last_camera_state = m_third_person_opt->mValue;
    m_third_person_opt->mValue = 1;
    log->info("freelook enabled, removed {} UpdatePlayerFromCameraComponents, camera count: {}", removedUpdateComponents,
              cameraCount);
}

void freelook::on_disable() {
    dispatcher().deafen_all(this);
    auto player = core::get_client_instance()->getLocalPlayer();
    if (!player) {
        return;
    }

    auto cameraRegistry = core::get_client_instance()->getCameraRegistry();
    if (!cameraRegistry) {
        return;
    }

    auto storage = player->mContext.find_storage<VanillaCamera::UpdatePlayerFromCameraComponent>();
    for (OwnerPtr<EntityId> entity : cameraRegistry->mCameraEntities) {
        const auto id = entity->mEntity;
        if (!storage->contains(id)) {
            auto it = m_cams.find(id);
            if (it != m_cams.end()) {
                storage->emplace(id, VanillaCamera::UpdatePlayerFromCameraComponent(it->second));
            }
        }

        if (auto directLook = entity->tryGetComponent<MinecraftCamera::CameraDirectLookComponent>()) {
            if (!directLook.has_value()) return;

            if (!m_og_rot_rads.contains(id)) {
                log->warn("on_disable: no original rotation found for camera id={}", id.rawId);
            } else {
                directLook->mRotRads = m_og_rot_rads[id];
            }
        }

        if (auto orbit = entity->tryGetComponent<MinecraftCamera::CameraOrbitComponent>()) {
            if (!orbit.has_value()) return;

            if (!m_orbit_rots_c.contains(id) || !m_orbit_rots_i.contains(id)) {
                log->warn("on_disable: no original orbit rotation found for camera id={}", id.rawId);
            } else {
                orbit->mCurrentSpherical = m_orbit_rots_c[id];
                orbit->mIdealSpherical = m_orbit_rots_i[id];
            }
        }
    }


    m_third_person_opt->mValue = m_last_camera_state;
    log->info("freelook disabled and restored");
}

// TODO: somehow make this functionality into the manager
void freelook::on_module_state_change(module_state_change_event& event) {
    auto freecam = get_module_system()->get<::freecam>();
    if (!freecam) return; // shouldnt happen unless the module isn't registered

    if (static_cast<void*>(event.module) == this) {
        if (freecam->is_enabled()) {
            event.cancel();
            log->info("freelook and freecam are mutually exclusive");;
        }
    } else if (static_cast<void*>(event.module) == freecam) {
        if (m_enabled) {
            event.cancel();
            log->info("freelook and freecam are mutually exclusive");
        }
    }
}

void freelook::on_lp_turn(local_player_turn_event& event) {
    auto cameraRegistry = core::get_client_instance()->getCameraRegistry();
    if (!cameraRegistry) {
        return;
    }

    const glm::vec2 deltaRads = glm::radians(glm::vec2(event.deltaRots->y, event.deltaRots->x));

    for (OwnerPtr<EntityId> entity : cameraRegistry->mCameraEntities) {
        const auto camera = entity->tryGetComponent<MinecraftCamera::CameraComponent>();
        if (!camera) {
            continue;
        }

        switch (camera->getMode()) {
            case CameraMode::FirstPerson: {
                if (auto directLook = entity->tryGetComponent<MinecraftCamera::CameraDirectLookComponent>()) {
                    directLook->mRotRads += glm::vec2(-deltaRads.x, deltaRads.y);
                }
                break;
            }
            case CameraMode::ThirdPerson:
            case CameraMode::ThirdPersonFront: {
                if (auto orbit = entity->tryGetComponent<MinecraftCamera::CameraOrbitComponent>()) {
                    orbit->mCurrentSpherical.mAzimuth += deltaRads.x;
                    orbit->mCurrentSpherical.mPolarAngle = std::clamp(
                            orbit->mCurrentSpherical.mPolarAngle + deltaRads.y, 0.0f, glm::pi<float>());
                    orbit->mIdealSpherical = orbit->mCurrentSpherical;
                }
                break;
            }
            default:
                break;
        }
    }

    event.cancel();
}