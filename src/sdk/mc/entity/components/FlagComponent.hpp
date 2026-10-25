//
// Created by emi on 10/25/26.
//

#pragma once

namespace VanillaCamera {
    struct UpdatePlayerFromCameraComponent
    {
    public:
        int mUpdateMode = 0;
    };
}


struct TargetCameraSetInitialOrientationComponent {};
struct CameraAlignWithTargetForwardComponent {};
struct ExtendPlayerRenderingComponent {};
struct ActiveCameraComponent {};
struct CurrentInputCameraComponent {};
struct CameraFirstPersonComponent {};
struct CameraBobComponent {};
struct CameraThirdPersonComponent {};
struct PlayerStateAffectsRenderingComponent {};

struct SetMovingFlagRequestComponent {};
struct CameraRenderFirstPersonObjectsComponent {};
struct CameraRenderPlayerModelComponent {};
struct RedirectCameraInputComponent {};
struct RenderCameraComponent {};
struct GameCameraComponent {};
struct OnFireComponent {};
struct MoveRequestComponent {};