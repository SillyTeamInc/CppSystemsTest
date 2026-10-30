//
// Created by emi on 10/28/26.
//

#include "freecam.hpp"

#include "../../../../sdk/mc/deps/minecraft_camera/components/RedirectCameraInputComponent.hpp"
#include "events/base_tick_event.hpp"
#include "events/local_player_turn_event.hpp"
#include "events/module_state_change_event.hpp"
#include "freelook.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/deps/minecraft_camera/CameraRegistry.hpp"
#include "sdk/mc/deps/minecraft_camera/components/ActiveCameraComponent.hpp"
#include "sdk/mc/entity/components/RenderPositionComponent.hpp"
#include "sdk/mc/entity/components/StateVectorComponent.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/EnumOption.hpp"
#include "system/features/modules/module_system.hpp"


// just for fun™

void freecam::on_enable() {
    dispatcher().listen<local_player_turn_event, &freecam::on_lp_turn>(this);
    dispatcher().listen<module_state_change_event, &freecam::on_module_state_change>(this);
    dispatcher().listen<render_event, &freecam::on_render>(this);
    dispatcher().listen<base_tick_event, &freecam::on_base_tick>(this);


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
        MinecraftCamera::CameraComponent* camera = entity->tryGetComponent<MinecraftCamera::CameraComponent>();

        if (storage->contains(id)) {
            m_cams[id] = storage->get(id).mUpdateMode;
            storage->remove(id);
            ++removedUpdateComponents;
        }

        if (camera->getMode() == CameraMode::Free) {
            entity->set_flag<MinecraftCamera::OverrideCameraComponent>(true);
            auto svc = player->mContext.tryGetComponent<StateVectorComponent>();
            auto statComp = entity->getOrAddComponent<MinecraftCamera::StationaryCameraComponent>();
        }

        if (auto directLook = entity->tryGetComponent<MinecraftCamera::CameraDirectLookComponent>()) {
            if (!directLook.has_value()) continue;
            m_og_rot_rads[id] = directLook->mRotRads;
        }

        if (auto orbit = entity->tryGetComponent<MinecraftCamera::CameraOrbitComponent>()) {
            if (!orbit.has_value()) continue;

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



    auto storage = player->mContext.find_storage<VanillaCamera::UpdatePlayerFromCameraComponent>();
    for (OwnerPtr<EntityId> entity : cameraRegistry->mCameraEntities) {
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
            if (!directLook.has_value()) continue;
            if (!m_og_rot_rads.contains(id)) {
                log->warn("on_disable: no original rotation found for camera id={}", id.rawId);
            } else {
                directLook->mRotRads = m_og_rot_rads[id];
            }
        }

        if (auto orbit = entity->tryGetComponent<MinecraftCamera::CameraOrbitComponent>()) {
            if (!orbit.has_value()) continue;

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
    if (!freelook) return; // shouldnt happen unless the module isn't registered

    if (static_cast<void*>(event.module) == this) {
        if (freelook->is_enabled()) {
            event.cancel();
            log->info("freelook and freecam are mutually exclusive");;
        }
    } else if (static_cast<void*>(event.module) == freelook) {
        if (m_enabled) {
            event.cancel();
            log->info("freelook and freecam are mutually exclusive");
        }
    }
}

namespace {
    /*float MathUtils::getRotationKeyOffset(bool raw, bool allowStrafe)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return 0;
        auto moveInput = player->getMoveInputComponent();
        if (raw) moveInput = reinterpret_cast<MoveInputComponent*>(player->getRawMoveInputComponent());

        bool isMoving = moveInput->mForward || moveInput->mBackward || moveInput->mLeft || moveInput->mRight;
        if (!isMoving) return 0;

        bool w = moveInput->mForward;
        bool s = moveInput->mBackward;
        bool a = moveInput->mLeft;
        bool d = moveInput->mRight;

        float yawOffset = 0;
        if (w && a && allowStrafe)
            yawOffset = -45;
        else if (w && d && allowStrafe)
            yawOffset = 45;
        else if (s && a && allowStrafe)
            yawOffset = -135;
        else if (s && d && allowStrafe)
            yawOffset = 135;
        else if (w)
            yawOffset = 0;
        else if (a && allowStrafe)
            yawOffset = -90;
        else if (s)
            yawOffset = -180;
        else if (d && allowStrafe)
            yawOffset = 90;
        else
            yawOffset = 0;

        return yawOffset;
    }


    glm::vec2 MathUtils::getMotion(float yaw, float speed, bool raw, bool allowStrafe) {
        yaw += getRotationKeyOffset(raw, allowStrafe) + 90;

        float calcYaw = glm::radians(yaw);

        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return { 0, 0 };

        if (!Keyboard::isUsingMoveKeys()) return { 0, 0 };

        glm::vec2 motion;
        motion.x = cos(calcYaw) * (speed);
        motion.y = sin(calcYaw) * (speed);

        return motion;
    }
    */
    float get_rotation_key_offset(bool raw, bool allow_strafe) {
        auto player = core::get_client_instance()->getLocalPlayer();
        if (!player) return 0;
        auto moveInput = player->mContext.tryGetComponent<MoveInputComponent>();
        if (!moveInput.has_value()) return 0;

        bool isMoving = moveInput->forward() || moveInput->backward() || moveInput->left() || moveInput->right();
        if (!isMoving) return 0;

        bool w = moveInput->forward();
        bool s = moveInput->backward();
        bool a = moveInput->left();
        bool d = moveInput->right();

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

}

void freecam::on_base_tick(base_tick_event& event) {
    m_last_pos = m_pos;

    glm::vec3 motion = glm::vec3(0, 0, 0);

    if (keyboard::is_using_move_keys(true))
    {
        glm::vec2 calc = get_motion(m_rot.y, 0.1, false, true);
        motion.x = calc.x;
        motion.z = calc.y;

        bool isJumping = keyboard::state["key.jump"];
        bool isSneaking = keyboard::state["key.sneak"];

        if (isJumping)
            motion.y += 0.1f;
        else if (isSneaking)
            motion.y -= 0.1f;
    }

    m_pos += motion;
}

namespace {
    glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) {
        return a + (b - a) * t;
    }
}

void freecam::on_render(render_event& event) {
    if (!m_enabled) return;

    auto player = core::get_client_instance()->getLocalPlayer();
    if (!player) return;

    auto cameraRegistry = core::get_client_instance()->getCameraRegistry();
    if (!cameraRegistry) return;

    for (OwnerPtr<EntityId> entity : cameraRegistry->mCameraEntities) {
        const auto camera = entity->tryGetComponent<MinecraftCamera::CameraComponent>();
        if (!camera) continue;

        if (camera->getMode() == CameraMode::Free) {
            auto& st = entity->getOrAddComponent<MinecraftCamera::StationaryCameraComponent>();
            st.mPos = m_pos;
        }
    }
}

void freecam::on_lp_turn(local_player_turn_event& event) {
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
            /*case CameraMode::FirstPerson: {
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
            }*/
            case CameraMode::Free: {
                auto& st = entity->getOrAddComponent<MinecraftCamera::StationaryCameraComponent>();
                // StationaryCameraComponent
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


/*
    AABBShapeComponent
    StateVectorComponent
    RenderPositionComponent
    ActorRotationComponent
    ActorHeadRotationComponent
    MobBodyRotationComponent
    WalkAnimationComponent
    MoveInputComponent
    CameraComponent
    CameraDirectLookComponent
    CameraOrbitComponent
    UpdatePlayerFromCameraComponent

    MinecraftCamera::RenderCameraComponent
    CameraRenderPlayerModelComponent
    RedirectCameraInputComponent
    CameraAlignWithTargetForwardComponent
    ExtendPlayerRenderingComponent
    CameraRenderFirstPersonObjectsComponent*/
/*

void Freecam::onEnable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->setFlag<RenderCameraComponent>(true);
    player->setFlag<CameraRenderPlayerModelComponent>(true);
    player->setFlag<RedirectCameraInputComponent>(true);

    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Freecam::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &Freecam::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Freecam::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->listen<ActorRenderEvent, &Freecam::onActorRenderEvent, nes::event_priority::VERY_FIRST>(this);



    mLastRot = *player->getActorRotationComponent();
    mLastRot.mOldYaw = mLastRot.mYaw;
    mLastRot.mOldPitch = mLastRot.mPitch;

    mLastHeadRot = *player->getActorHeadRotationComponent();
    mLastHeadRot.mOldHeadRot = mLastHeadRot.mHeadRot;

    mLastBodyRot = *player->getMobBodyRotationComponent();
    mLastBodyRot.yOldBodyRot = mLastBodyRot.yBodyRot;

    mAABBMin = player->getAABBShapeComponent()->mMin;
    mAABBMax = player->getAABBShapeComponent()->mMax;
    mSvPos = player->getStateVectorComponent()->mPos;
    mSvPosOld = player->getStateVectorComponent()->mPos;
    mOldPos = player->getRenderPositionComponent()->mPosition;
    if (mMode.mValue != Mode::Detached)
        player->getWalkAnimationComponent()->mWalkAnimSpeed = 0.0f;

    mOrigin = mSvPos;
    mOldOrigin = mSvPosOld;


    auto gock = player->getActorHeadRotationComponent();
    mHeadYaw = { gock->mHeadRot, gock->mOldHeadRot };

    for (auto&& [id, cameraComponent] : player->mContext.mRegistry->view<CameraComponent>().each())
    {
        player->mContext.mRegistry->set_flag<CameraAlignWithTargetForwardComponent>(id, false);
        player->mContext.mRegistry->set_flag<ExtendPlayerRenderingComponent>(id, true);

        auto storage = player->mContext.mRegistry->assure_t<UpdatePlayerFromCameraComponent>();
        if (mMode.mValue == Mode::Detached && storage->contains(id))
        {
            mCameras[id] = storage->get(id).mUpdateMode;
            storage->remove(id);
        }

        if (cameraComponent.getMode() == CameraMode::FirstPerson)
        {
            auto* gaming = player->mContext.mRegistry->try_get<CameraDirectLookComponent>(id);
            if (gaming)
            {
                mOriginalRotRads[cameraComponent.getMode()] = gaming->mRotRads;
            }

            // Remove the CameraRenderFirstPersonObjectsComponent flag
            player->mContext.mRegistry->set_flag<CameraRenderFirstPersonObjectsComponent>(id, false);
        } else if (cameraComponent.getMode() == CameraMode::ThirdPerson || cameraComponent.getMode() == CameraMode::ThirdPersonFront)
        {
            auto* gaming = player->mContext.mRegistry->try_get<CameraOrbitComponent>(id);
            if (gaming)
            {
                mOriginalRotRads[cameraComponent.getMode()] = gaming->mRotRads;
            }
        }
    }
}

void Freecam::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Freecam::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &Freecam::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Freecam::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<ActorRenderEvent, &Freecam::onActorRenderEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->setFlag<RenderCameraComponent>(false);
    player->setFlag<CameraRenderPlayerModelComponent>(false);
    player->setFlag<RedirectCameraInputComponent>(false);

    if (mMode.mValue == Mode::Normal)
    {
        player->getAABBShapeComponent()->mMin = mAABBMin;
        player->getAABBShapeComponent()->mMax = mAABBMax;
        player->getStateVectorComponent()->mPos = mSvPos;
        player->getStateVectorComponent()->mPosOld = mSvPosOld;
    }
    player->getWalkAnimationComponent()->mWalkAnimSpeed = 1.0f;
    player->getMoveInputComponent()->reset( false);


    mResetRot = true;
}

void Freecam::onPacketInEvent(PacketInEvent& event)
{

    if (event.mPacket->getId() == PacketID::MovePlayer)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;

        auto mpp = event.getPacket<MovePlayerPacket>();
        if (mpp->mPlayerID == player->getRuntimeID())
        {
            if (mDisableOnLagback.mValue)
            {
                NotifyUtils::notify("Lagback detected!", 5.f, Notification::Type::Warning);
                setEnabled(false);
                return;
            }
            glm::vec2 pktRot = mpp->mRot;
            float headRot = mpp->mYHeadRot;
            mLastRot.mYaw = pktRot.x;
            mLastRot.mPitch = pktRot.y;
            mLastHeadRot.mHeadRot = headRot;
            mLastBodyRot.yBodyRot = pktRot.x;

            mLastRot.mOldYaw = pktRot.x;
            mLastRot.mOldPitch = pktRot.y;
            mLastHeadRot.mOldHeadRot = headRot;
            mLastBodyRot.yOldBodyRot = pktRot.x;


            player->setPosition(mpp->mPos);
            mOrigin = mpp->mPos;
            mOldOrigin = mpp->mPos;
            spdlog::debug("[Freecam] Listening to lagback, setting position to {}, {}, {}", mpp->mPos.x, mpp->mPos.y, mpp->mPos.z);
        }
    }
}

void Freecam::onPacketOutEvent(PacketOutEvent& event)
{
    if ((event.mPacket->getId() == PacketID::PlayerAuthInput || event.mPacket->getId() == PacketID::MovePlayer) && mMode.mValue != Mode::Detached)
        event.mCancelled = true;

    if ((event.mPacket->getId() == PacketID::PlayerAuthInput) && mMode.mValue == Mode::Detached)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;
        auto paip = event.getPacket<PlayerAuthInputPacket>();

        paip->mRot = { mLastRot.mPitch, mLastRot.mYaw };
        paip->mYHeadRot = mLastRot.mYaw;

        // Clamp the rot to -180 to 180 for yaw, -90 to 90 for pitch
        paip->mRot.x = MathUtils::wrap(paip->mRot.x, -90, 90);
        paip->mRot.y = MathUtils::wrap(paip->mRot.y, -180, 180);
        paip->mYHeadRot = MathUtils::wrap(paip->mYHeadRot, -180, 180);
        paip->removeMovingInput();
    }

}


void Freecam::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    player->setFlag<RenderCameraComponent>(true);
    player->setFlag<CameraRenderPlayerModelComponent>(true);
    //player->setFlag<CameraRenderFirstPersonObjects>(false);

    glm::vec3 motion = glm::vec3(0, 0, 0);

    if (Keyboard::isUsingMoveKeys(true))
    {
        glm::vec2 rots = mRotRads;
        rots = glm::vec2(rots.y, rots.x); // Correct rotation (pitch, yaw)
        // convert the rots to degrees (-180 to 180 for yaw, -90 to 90 for pitch)
        rots = glm::degrees(rots);
        // Invert the yaw
        rots.y = -rots.y + 180;
        // Wrap
        rots.y = MathUtils::wrap(rots.y, -180, 180);

        glm::vec2 calc = MathUtils::getMotion(rots.y, mSpeed.mValue / 10, false);
        motion.x = calc.x;
        motion.z = calc.y;
        auto& keyboard = *ClientInstance::get()->getKeyboardSettings();

        bool isJumping = Keyboard::mPressedKeys[keyboard["key.jump"]];
        bool isSneaking = Keyboard::mPressedKeys[keyboard["key.sneak"]];

        if (isJumping)
            motion.y += mSpeed.mValue / 10;
        else if (isSneaking)
            motion.y -= mSpeed.mValue / 10;
    }

    if (mMode.mValue == Mode::Normal) player->getStateVectorComponent()->mVelocity = motion;
    else if (mMode.mValue == Mode::Detached)
    {
        mOldOrigin = mOrigin;
        mOrigin += motion;
    }
}

void Freecam::onActorRenderEvent(ActorRenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mEntity != player) return;
    if (*event.mPos == glm::vec3(0.f, 0.f, 0.f) && *event.mRot == glm::vec2(0.f, 0.f)) event.cancel(); // Prevents the hand from rendering

    auto oldRots = *player->getActorRotationComponent();
    auto oldHeadRots = *player->getActorHeadRotationComponent();
    auto oldBodyRots = *player->getMobBodyRotationComponent();

    *player->getActorRotationComponent() = mLastRot;
    *player->getActorHeadRotationComponent() = mLastHeadRot;
    *player->getMobBodyRotationComponent() = mLastBodyRot;
    if (mMode.mValue == Mode::Detached)
    {
        player->getMoveInputComponent()->reset(true, false);
        // TODO: Prevent the raycast from updating while in detached mode
    }

    auto original = event.mDetour->getOriginal<&ActorRenderDispatcherHook::render>();
    auto newPos = *event.mPos
    if (mMode.mValue == Mode::Detached) newPos = *event.mPos
    original(event._this, event.mEntityRenderContext, event.mEntity, event.mCameraTargetPos, &newPos, event.mRot, event.mIgnoreLighting);
    event.cancel();

    if (mMode.mValue != Mode::Detached)
    {
        *player->getActorRotationComponent() = oldRots;
        *player->getActorHeadRotationComponent() = oldHeadRots;
        *player->getMobBodyRotationComponent() = oldBodyRots;
    }
}

void Freecam::onLookInputEvent(LookInputEvent& event)
{
    if (mResetRot)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        for (auto&& [id, cameraComponent] : player->mContext.mRegistry->view<CameraComponent>().each())
        {
            player->mContext.mRegistry->set_flag<CameraAlignWithTargetForwardComponent>(id, true);
            auto storage = player->mContext.mRegistry->assure_t<UpdatePlayerFromCameraComponent>();
            if (!storage->contains(id))
            {
                storage->emplace(id, UpdatePlayerFromCameraComponent(mCameras[id]));
            }

            if (cameraComponent.getMode() == CameraMode::FirstPerson)
            {
                auto* gaming = player->mContext.mRegistry->try_get<CameraDirectLookComponent>(id);
                if (gaming)
                {
                    gaming->mRotRads = mOriginalRotRads[cameraComponent.getMode()];
                }

                player->mContext.mRegistry->set_flag<CameraRenderFirstPersonObjectsComponent>(id, true);
            } else if (cameraComponent.getMode() == CameraMode::ThirdPerson || cameraComponent.getMode() == CameraMode::ThirdPersonFront)
            {
                auto* gaming = player->mContext.mRegistry->try_get<CameraOrbitComponent>(id);
                if (gaming)
                {
                    gaming->mRotRads = mOriginalRotRads[cameraComponent.getMode()];
                }
            }
        }

        mResetRot = false;
    }


    if (!mEnabled) return;
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;


    ClientInstance::get()->getOptions()->mThirdPerson->value = 0;

    if (mMode.mValue == Mode::Detached)
    {
        event.mFirstPersonCamera->mOrigin = getLerpedOrigin();
    }
    mRotRads = event.mCameraDirectLookComponent->mRotRads;
}*/