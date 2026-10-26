//
// Created by emi on 10/20/26.
//

#pragma once

#include "sdk/mc/deps/core/string/HashedString.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "spdlog/spdlog.h"
#include "util/types.hpp"

enum class CameraMode : int {
    Unknown = -1,
    FirstPerson = 0,
    ThirdPerson = 1,
    ThirdPersonFront = 2,
    DeathCamera = 3,
    Free = 4,
    FollowOrbit = 5,
};

namespace MinecraftCamera {
    struct CameraComponent {
    public:
        HashedString mId;
        glm::qua<float> mOrientation;
        glm::vec3 mPosition;
        float mAspectRatio;
        float mFieldOfView;
        float mNearPlane;
        float mFarPlane;
        glm::mat4x4 mPostViewTransform;
        glm::mat4x4 mSavedProjection;
        glm::mat4x4 mSavedModelView;

        // im not sorry :3
        static constexpr uint64 FirstPersonHash = HashedString::computeHash("minecraft:first_person");
        static constexpr uint64 ThirdPersonHash = HashedString::computeHash("minecraft:third_person");
        static constexpr uint64 ThirdPersonFrontHash = HashedString::computeHash("minecraft:third_person_front");
        static constexpr uint64 DeathCameraHash = HashedString::computeHash("minecraft:death_camera");
        static constexpr uint64 FreeHash = HashedString::computeHash("minecraft:free");
        static constexpr uint64 FollowOrbitHash = HashedString::computeHash("minecraft:follow_orbit");

        [[nodiscard]] CameraMode getMode() const {
            if (mId.mStrHash == FirstPersonHash) return CameraMode::FirstPerson;
            if (mId.mStrHash == ThirdPersonHash) return CameraMode::ThirdPerson;
            if (mId.mStrHash == ThirdPersonFrontHash) return CameraMode::ThirdPersonFront;
            if (mId.mStrHash == DeathCameraHash) return CameraMode::DeathCamera;
            if (mId.mStrHash == FreeHash) return CameraMode::Free;
            if (mId.mStrHash == FollowOrbitHash) return CameraMode::FollowOrbit;

            return CameraMode::Unknown;
        }

        std::string to_string() const {
            return fmt::format("CameraComponent{{mId={}, mOrientation=({}, {}, {}, {}), mPosition=({}, {}, {}), mAspectRatio={}, mFieldOfView={}, mNearPlane={}, mFarPlane={}}}",
                               mId.mStr, mOrientation.x, mOrientation.y, mOrientation.z, mOrientation.w,
                               mPosition.x, mPosition.y, mPosition.z,
                               mAspectRatio, mFieldOfView, mNearPlane, mFarPlane);
        }
    };
}
