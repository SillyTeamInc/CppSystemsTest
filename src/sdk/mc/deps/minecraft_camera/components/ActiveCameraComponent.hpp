//
// Created by emi on 10/29/2026.
//
#pragma once

namespace MinecraftCamera {
    struct OverrideCameraComponent {};
    struct StationaryCameraComponent {
        glm::vec3 mPos;
        glm::vec2 mRot;
    };
    struct ActiveCameraComponent {};
    struct CurrentInputCameraComponent {};
    struct CameraFirstPersonComponent {};
    struct RenderCameraComponent {};
    struct CameraAttachComponent {
    public:
        WeakRef<::EntityContext> mAttachTo;
        ::HashedString mLocator;
    };
} // namespace MinecraftCamera
