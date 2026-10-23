//
// Created by emi on 10/20/26.
//

#pragma once

#include <glm/glm.hpp>

namespace MinecraftCamera {
    class CameraDirectLookComponent {
    public:
        glm::vec2 mRotRads{};
        float mDelta = 0.0f;
        glm::vec2 mWrap{};

        void operator=(const CameraDirectLookComponent& other) {
            mRotRads = other.mRotRads;
            mDelta = other.mDelta;
            mWrap = other.mWrap;
        }
    };
}