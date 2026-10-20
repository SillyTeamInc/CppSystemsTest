//
// Created by emi on 10/17/26.
//
#pragma once
#include <array>

#include "glm/glm.hpp"
#include "sdk/mc/platform/brstd/bitset.hpp"
#include "util/types.hpp"

struct MoveInputState {
public:
    enum class Flag : std::size_t {
        SneakDown = 0,
        SneakToggleDown = 1,
        WantDownSlow = 2,
        WantUpSlow = 3,
        BlockSelectDown = 4,
        AscendBlock = 5,
        DescendBlock = 6,
        JumpDown = 7,
        SprintDown = 8,
        UpLeft = 9,
        UpRight = 10,
        DownLeft = 11,
        DownRight = 12,
        Up = 13,
        Down = 14,
        Left = 15,
        Right = 16,
        Ascend = 17,
        Descend = 18,
        ChangeHeight = 19,
        LookCenter = 20,
        SneakInputCurrentlyDown = 21,
        SneakInputWasReleased = 22,
        SneakInputWasPressed = 23,
        JumpInputWasReleased = 24,
        JumpInputWasPressed = 25,
        JumpInputCurrentlyDown = 26,
        Count = 27,
    };

public:
    brstd::bitset<27, uint> mFlagValues;
    glm::vec2 mAnalogMoveVector;
    uchar mLookSlightDirField;
    uchar mLookNormalDirField;
    uchar mLookSmoothDirField;
};

struct MoveInputComponent {
public:
    enum class Flag : int {
        Sneaking = 0,
        Sprinting = 1,
        WantUp = 2,
        WantDown = 3,
        Jumping = 4,
        AutoJumpingInWater = 5,
        MoveInputStateLocked = 6,
        PersistSneak = 7,
        AutoJumpEnabled = 8,
        IsCameraRelativeMovementEnabled = 9,
        IsRotControlledByMoveDirection = 10,
        Count = 11,
    };

public:
    MoveInputState mInputState;
    MoveInputState mRawInputState;
    uchar mHoldAutoJumpInWaterTicks;
    glm::vec2 mMove;
    glm::vec2 mLookDelta;
    glm::vec2 mInteractDir;
    glm::vec3 mDisplacement;
    glm::vec3 mDisplacementDelta;
    glm::vec3 mCameraOrientation;
    brstd::bitset<11, ushort> mFlagValues;
    std::array<bool, 2> mIsPaddling;
};
