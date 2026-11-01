//
// Created by emi on 10/28/26.
//

#include "freecam.hpp"

#include "../../../../sdk/mc/deps/minecraft_camera/components/RedirectCameraInputComponent.hpp"
#include "events/base_tick_event.hpp"
#include "events/local_player_turn_event.hpp"
#include "events/module_state_change_event.hpp"
#include "events/stationary_camera_event.hpp"
#include "freelook.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/client/game/Minecraft.hpp"
#include "sdk/mc/deps/minecraft_camera/CameraRegistry.hpp"
#include "sdk/mc/deps/minecraft_camera/components/ActiveCameraComponent.hpp"
#include "sdk/mc/entity/components/ClientInputLockComponent.hpp"
#include "sdk/mc/entity/components/RenderPositionComponent.hpp"
#include "sdk/mc/entity/components/StateVectorComponent.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/EnumOption.hpp"
#include "system/features/modules/module_system.hpp"

namespace {
    float get_rotation_key_offset(bool raw, bool allow_strafe) {
        auto player = core::get_client_instance()->getLocalPlayer();
        if (!player)
            return 0;

        bool w = keyboard::state["key.forward"];
        bool s = keyboard::state["key.back"];
        bool a = keyboard::state["key.left"];
        bool d = keyboard::state["key.right"];

        bool isMoving = w || s || a || d;
        if (!isMoving)
            return 0;

        float yawOffset = 0;
        if (w && a && allow_strafe)
            yawOffset = -45;
        else if (w && d && allow_strafe)
            yawOffset = 45;
        else if (s && a && allow_strafe)
            yawOffset = -135;
        else if (s && d && allow_strafe)
            yawOffset = 135;
        else if (w)
            yawOffset = 0;
        else if (a && allow_strafe)
            yawOffset = -90;
        else if (s)
            yawOffset = -180;
        else if (d && allow_strafe)
            yawOffset = 90;
        else
            yawOffset = 0;

        return yawOffset;
    }

    glm::vec2 get_motion(float yaw, float speed, bool raw, bool allow_strafe) {
        yaw += get_rotation_key_offset(raw, allow_strafe) + 90;

        float calcYaw = glm::radians(yaw);

        glm::vec2 motion;
        motion.x = cos(calcYaw) * (speed);
        motion.y = sin(calcYaw) * (speed);

        return motion;
    }

    glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) { return a + (b - a) * t; }
    glm::vec2 rot_from_quat(const glm::qua<float>& q) {
        float yh = std::atan2(q.y, q.w);
        float cy = std::cos(yh), sy = std::sin(yh);
        float sp = q.x * cy - q.z * sy;
        float cp = q.w * cy + q.y * sy;
        float ph = std::atan2(sp, cp);
        return {-glm::degrees(2.f * ph), 180.f - glm::degrees(2.f * yh)};
    }
} // namespace

// just for fun™

void freecam::on_enable() {
    dispatcher().listen<local_player_turn_event, &freecam::on_lp_turn>(this);
    dispatcher().listen<module_state_change_event, &freecam::on_module_state_change>(this);
    dispatcher().listen<render_event, &freecam::on_render>(this);
    dispatcher().listen<base_tick_event, &freecam::on_base_tick>(this);
    dispatcher().listen<stationary_camera_event, &freecam::on_sc_tick>(this);

    ClientInstance* ci = core::get_client_instance();
    auto player = ci->getLocalPlayer();
    auto cameraRegistry = ci->getCameraRegistry();

    if (!player || !cameraRegistry) {
        disable();
        return;
    }
    StateVectorComponent* stateVector = player->mContext.tryGetComponent<StateVectorComponent>();
    ClientInputLockComponent* clientInputLock = player->mContext.tryGetComponent<ClientInputLockComponent>();
    if (clientInputLock) {
        clientInputLock->lockAll();
    } else
        log->error("missing ClientInputLockComponent");

    const MinecraftCamera::CameraComponent* active = nullptr;
    for (OwnerPtr<EntityId> e: cameraRegistry->mCameraEntities)
        if (e->has_flag<MinecraftCamera::ActiveCameraComponent>())
            active = e->tryGetComponent<MinecraftCamera::CameraComponent>();
    if (!active) {
        disable();
        log->error("missing active camera????");
        return;
    }

    m_pos = m_last_pos = active->mPosition;
    m_rot = rot_from_quat(active->mOrientation);

    std::size_t cameraCount = 0;
    std::size_t removedUpdateComponents = 0;
    auto storage = player->mContext.find_storage<VanillaCamera::UpdatePlayerFromCameraComponent>();
    for (OwnerPtr<EntityId> entity: cameraRegistry->mCameraEntities) {
        ++cameraCount;
        const auto id = entity->mEntity;
        MinecraftCamera::CameraComponent* camera = entity->tryGetComponent<MinecraftCamera::CameraComponent>();

        if (storage->contains(id)) {
            m_cams[id] = storage->get(id).mUpdateMode;
            storage->remove(id);
            ++removedUpdateComponents;
        }

        if (camera->getMode() == CameraMode::Free) {
            entity->set_flag<MinecraftCamera::OverrideCameraComponent>(true);
            auto& st = entity->getOrAddComponent<MinecraftCamera::StationaryCameraComponent>();
            st.mPos = m_pos;
            st.mRot = m_rot;
        }

        if (auto directLook = entity->tryGetComponent<MinecraftCamera::CameraDirectLookComponent>()) {
            if (!directLook.has_value())
                continue;
            m_og_rot_rads[id] = directLook->mRotRads;
        }

        if (auto orbit = entity->tryGetComponent<MinecraftCamera::CameraOrbitComponent>()) {
            if (!orbit.has_value())
                continue;

            m_orbit_rots_c[id] = orbit->mCurrentSpherical;
            m_orbit_rots_i[id] = orbit->mIdealSpherical;
        }
    }


    this->m_third_person_opt = static_cast<EnumOption*>(
            core::get_client_instance()->getOptions().getIfValid(OptionID::GameThirdperson).value());
    m_last_camera_state = m_third_person_opt->mValue;


    log->info("freecam enabled, removed {} UpdatePlayerFromCameraComponents, camera count: {}", removedUpdateComponents,
              cameraCount);
}

void freecam::on_disable() {
    dispatcher().deafen_all(this);

    auto player = core::get_client_instance()->getLocalPlayer();
    if (!player) {
        return;
    }

    auto cameraRegistry = core::get_client_instance()->getCameraRegistry();
    if (!cameraRegistry) {
        return;
    }

    ClientInputLockComponent* clientInputLock = player->mContext.tryGetComponent<ClientInputLockComponent>();
    if (clientInputLock) {
        clientInputLock->unlockAll();
    } else
        log->error("missing ClientInputLockComponent");


    auto storage = player->mContext.find_storage<VanillaCamera::UpdatePlayerFromCameraComponent>();
    for (OwnerPtr<EntityId> entity: cameraRegistry->mCameraEntities) {
        const auto id = entity->mEntity;
        if (!storage->contains(id)) {
            auto it = m_cams.find(id);
            if (it != m_cams.end()) {
                storage->emplace(id, VanillaCamera::UpdatePlayerFromCameraComponent(it->second));
            }
        }

        MinecraftCamera::CameraComponent* camera = entity->tryGetComponent<MinecraftCamera::CameraComponent>();

        if (camera && camera->getMode() == CameraMode::Free) {
            entity->set_flag<MinecraftCamera::OverrideCameraComponent>(false);
        }

        if (auto directLook = entity->tryGetComponent<MinecraftCamera::CameraDirectLookComponent>()) {
            if (!directLook.has_value())
                continue;
            if (!m_og_rot_rads.contains(id)) {
                log->warn("on_disable: no original rotation found for camera id={}", id.rawId);
            } else {
                directLook->mRotRads = m_og_rot_rads[id];
            }
        }

        if (auto orbit = entity->tryGetComponent<MinecraftCamera::CameraOrbitComponent>()) {
            if (!orbit.has_value())
                continue;

            if (!m_orbit_rots_c.contains(id) || !m_orbit_rots_i.contains(id)) {
                log->warn("on_disable: no original orbit rotation found for camera id={}", id.rawId);
            } else {
                orbit->mCurrentSpherical = m_orbit_rots_c[id];
                orbit->mIdealSpherical = m_orbit_rots_i[id];
            }
        }
    }


    m_third_person_opt->mValue = m_last_camera_state;
    log->info("freecam disabled and restored");
}
// (see the to do in freelook!!!!)
void freecam::on_module_state_change(module_state_change_event& event) {
    auto freelook = get_module_system()->get<::freelook>();
    if (!freelook)
        return; // shouldnt happen unless the module isn't registered

    if (static_cast<void*>(event.module) == this) {
        if (freelook->is_enabled()) {
            event.cancel();
            log->info("freelook and freecam are mutually exclusive");
            ;
        }
    } else if (static_cast<void*>(event.module) == freelook) {
        if (m_enabled) {
            event.cancel();
            log->info("freelook and freecam are mutually exclusive");
        }
    }
}


void freecam::on_base_tick(base_tick_event& event) {

    glm::vec3 motion = glm::vec3(0, 0, 0);
    constexpr float speed = 0.8f;

    if (keyboard::is_using_move_keys(false)) {
        glm::vec2 calc = get_motion(m_rot.y, speed, false, true);
        motion.x = calc.x;
        motion.z = calc.y;
    }

    bool isJumping = keyboard::state["key.jump"];
    bool isSneaking = keyboard::state["key.sneak"];

    if (isJumping)
        motion.y += speed;
    else if (isSneaking)
        motion.y -= speed;

    m_last_pos = m_pos;
    m_pos += motion;
}

void freecam::on_render(render_event& event) {
    if (!m_enabled)
        return;

    auto player = core::get_client_instance()->getLocalPlayer();
    if (!player)
        return;

    auto cameraRegistry = core::get_client_instance()->getCameraRegistry();
    if (!cameraRegistry)
        return;

    ClientInstance* ci = core::get_client_instance();
    Minecraft* mc = ci->getServerData().get();
    Timer* mSim = mc->mRealTimer;

    for (OwnerPtr<EntityId> entity: cameraRegistry->mCameraEntities) {
        const auto camera = entity->tryGetComponent<MinecraftCamera::CameraComponent>();
        if (!camera)
            continue;

        if (camera->getMode() == CameraMode::Free) {
            auto& st = entity->getOrAddComponent<MinecraftCamera::StationaryCameraComponent>();
            st.mPos = lerp(m_last_pos, m_pos, mSim->mPartialTicks);
        }
    }
}

void freecam::on_sc_tick(stationary_camera_event& event) {
    auto ci = core::get_client_instance();
    auto reg = ci->getCameraRegistry().get();;

    float a = ci->getServerData()->mSimTimer->mPartialTicks;

    for (OwnerPtr<EntityId> e: reg->mCameraEntities) {
        auto cam = e->tryGetComponent<MinecraftCamera::CameraComponent>();
        if (!cam.has_value() || cam->getMode() != CameraMode::Free)
            continue;

        auto& st = e->getOrAddComponent<MinecraftCamera::StationaryCameraComponent>();
        st.mPos = lerp(m_last_pos, m_pos, a);
        st.mRot = m_rot;
    }
}

void freecam::on_lp_turn(local_player_turn_event& event) {
    auto cameraRegistry = core::get_client_instance()->getCameraRegistry();
    if (!cameraRegistry) {
        return;
    }

    const glm::vec2 deltaRads = glm::radians(glm::vec2(event.deltaRots->y, event.deltaRots->x));

    for (OwnerPtr<EntityId> entity: cameraRegistry->mCameraEntities) {
        const auto camera = entity->tryGetComponent<MinecraftCamera::CameraComponent>();
        if (!camera) {
            continue;
        }

        switch (camera->getMode()) {
            case CameraMode::Free: {
                auto& st = entity->getOrAddComponent<MinecraftCamera::StationaryCameraComponent>();
                glm::vec2 newRot = glm::degrees(glm::vec2(-deltaRads.y, deltaRads.x));
                st.mRot += newRot;
                m_rot = st.mRot;
                break;
            }
            default:
                break;
        }
    }

    event.cancel();
}
