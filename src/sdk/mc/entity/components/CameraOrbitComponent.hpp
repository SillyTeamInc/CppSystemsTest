//
// Created by emi on 10/20/26.
//

#pragma once
#include "sdk/mc/deps/minecraft_camera/Spherical.hpp"

namespace MinecraftCamera {
    struct CameraOrbitComponent {
    public:
        float mAzimuthSmoothingStiffness;
        float mPolarAngleSmoothingStiffness;
        float mDistanceSmoothingStiffness;
        float mPolarAngleMin;
        float mPolarAngleMax;
        float mYawAngleMin;
        float mYawAngleMax;
        float mRadius;
        bool mInvertXInput;
        bool mInvertYInput;
        Spherical mCurrentSpherical;
        Spherical mIdealSpherical;
        float mAzimuthVelocity;
        float mPolarAngleVelocity;
        float mDistanceVelocity;
        float mDistanceConstraint;
    };
}
