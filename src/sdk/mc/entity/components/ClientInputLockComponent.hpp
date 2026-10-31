//
// Created by emi on 10/31/2026.
//

#pragma once
#include "sdk/mc/deps/core/container/EnumSet.hpp"

enum class ClientInputLockCategory : uchar {
    Undefined       = 0,
    Camera          = 1,
    Movement        = 2,
    Emoting         = 3,
    LateralMovement = 4,
    Sneak           = 5,
    Jump            = 6,
    Mount           = 7,
    Dismount        = 8,
    MoveForward     = 9,
    MoveBackward    = 10,
    MoveLeft        = 11,
    MoveRight       = 12,
    Count           = 13,
};


enum class ClientInputLockType : uchar {
    Undefined    = 0,
    Move         = 1,
    Jump         = 2,
    Sneak        = 3,
    Mount        = 4,
    Dismount     = 5,
    FreeLook     = 6,
    MoveLeft     = 7,
    MoveRight    = 8,
    MoveForward  = 9,
    MoveBackward = 10,
    Count        = 11,
};


struct ClientInputLockComponent {
public:
    Bedrock::EnumSet<::ClientInputLockCategory, 13> mActiveCategories;
    Bedrock::EnumSet<::ClientInputLockType, 11>     mClientInputLocks;

    void setActive(ClientInputLockCategory category, bool value) {
        mActiveCategories.mContainer.set(static_cast<std::underlying_type_t<ClientInputLockCategory>>(category), value);
    }

    void setClientInputLock(ClientInputLockType type, bool value) {
        mClientInputLocks.mContainer.set(static_cast<std::underlying_type_t<ClientInputLockType>>(type), value);
    }

    void lockAll() {
        for (uchar category = 0; category < static_cast<uchar>(ClientInputLockCategory::Count); ++category) {
            if (category == static_cast<uchar>(ClientInputLockCategory::Camera)) {
                continue;
            }
            mActiveCategories.mContainer.set(category, true);
        }
        for (uchar type = 0; type < static_cast<uchar>(ClientInputLockType::Count); ++type) {
            if (type == static_cast<uchar>(ClientInputLockType::FreeLook)) {
                continue;
            }
            mClientInputLocks.mContainer.set(type, true);
        }
    }

    void unlockAll() {
        for (uchar category = 0; category < static_cast<uchar>(ClientInputLockCategory::Count); ++category) {
            mActiveCategories.mContainer.set(category, false);
        }
        for (uchar type = 0; type < static_cast<uchar>(ClientInputLockType::Count); ++type) {
            mClientInputLocks.mContainer.set(type, false);
        }
    }

};
