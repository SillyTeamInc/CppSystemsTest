//
// Created by emi on 10/26/26.
//

#pragma once
#include <optional>


#include "glm/vec3.hpp"
#include "sdk/mc/deps/game_refs/OwnerPtr.hpp"

struct EntityId;

namespace MinecraftCamera {

    struct CameraBlendStateComponent {
    public:
        struct EaseState {
        public:
            float mAmount;
            float mFieldOfView;
            glm::vec3 mEulerAngles;
            glm::vec3 mPosition;
        };

        struct SnapShotEaseValues {
        public:
            glm::vec3 mTargetPos;
        };

    public:
        std::optional<::MinecraftCamera::CameraBlendStateComponent::EaseState> mLastEaseState;
        WeakRef<::EntityId> mActiveCamera;
        float mPercentage;
        float (*mEaseFunction)(float, float, float);
        float mBlendTime;
        float mBlendStartFieldOfView;
        glm::vec3 mBlendStartPosition;
        glm::vec3 mBlendStartEulerAngles;
        SnapShotEaseValues mSnapShotEaseValues;
    };

} // namespace MinecraftCamera

