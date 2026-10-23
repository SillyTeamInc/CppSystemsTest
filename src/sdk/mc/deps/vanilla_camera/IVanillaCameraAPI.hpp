//
// Created by emi on 10/21/26.
//

#pragma once
#include <optional>

#include "glm/glm.hpp"

class EntityContext;

class IVanillaCameraAPI {
public:
    struct PortalData;
    struct DamageBobParameters;
    struct MovementBobParameters;
    struct VehicleRotationInformation;
    struct VehicleSeatCameraOptions;

    struct PortalData {
    public:
        // NOLINTBEGIN
        float mTime;
        float mRotation;
        float mIntensity;
        // NOLINTEND
    };

    struct DamageBobParameters {
    public:
        // NOLINTBEGIN
        float mFractionalFramesSinceDeath;
        float mDamagedFromDirectionYawRadians;
        float mDamageCountdownRemaining;
        float mDamageBobStrength;
        // NOLINTEND
    };

    struct MovementBobParameters {
    public:
        // NOLINTBEGIN
        glm::vec2 mMovementDirectionEulerAnglesRadians;
        float  mMovementDistance;
        float  mBobFactor;
        // NOLINTEND
    };

    struct VehicleRotationInformation {
    public:
        // NOLINTBEGIN
        float mCenterDirectionRadians;
        float mHalfAngleRadians;
        float mVehicleRotationRadians;
        // NOLINTEND
    };

    struct VehicleSeatCameraOptions {
    public:
        // NOLINTBEGIN
        std::optional<float> mThirdPersonCameraDistanceOverride;
        float mCameraRelaxDistanceSmoothingOverride;
        // NOLINTEND
    };

    using PlayerViewMode = ::SharedTypes::v1_21_100::PlayerViewMode;

public:
    // NOLINTBEGIN
    virtual ~IVanillaCameraAPI() = default;
    virtual ::std::optional<::IVanillaCameraAPI::DamageBobParameters>tryGetDamageBobParameters(::WeakRef<::EntityContext> const actorRef, float frameInterpolation) const = 0;
    virtual ::std::optional<::IVanillaCameraAPI::MovementBobParameters>tryGetMovementBobParameters(::WeakRef<::EntityContext> const actorRef, float frameInterpolation) const = 0;
    virtual ::std::optional<::IVanillaCameraAPI::VehicleRotationInformation>tryGetVehicleRotationInformation(::WeakRef<::EntityContext> const actorId) const = 0;
    virtual ::IVanillaCameraAPI::VehicleSeatCameraOptions getVehicleSeatCameraOptions(::WeakRef<::EntityContext> const actorId) const = 0;
    virtual ::std::optional<::IVanillaCameraAPI::PortalData> tryGetPortalData() const = 0;
    virtual ::SharedTypes::v1_21_100::PlayerViewMode getPlayerViewPerspectiveOption() const = 0;
    virtual ::std::optional<bool> isPlayerSleeping() const = 0;
    // NOLINTEND
};
