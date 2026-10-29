//
// Created by emi on 10/28/26.
//

#include "freecam.hpp"

#include "events/base_tick_event.hpp"
#include "events/local_player_turn_event.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"

// just for fun™

void freecam::on_enable() {
    dispatcher().listen<base_tick_event, &freecam::on_base_tick>(this);
    dispatcher().listen<local_player_turn_event, &freecam::on_lp_turn>(this);

    ClientInstance* ci = core::get_client_instance();
    LocalPlayer* lp = ci->getLocalPlayer();
    if (!lp) {
        disable();
        return;
    }
}

void freecam::on_disable() {
    dispatcher().deafen_all(this);
}

void freecam::on_base_tick(base_tick_event& event) {

}

void freecam::on_lp_turn(local_player_turn_event& event) {

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

    RenderCameraComponent
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