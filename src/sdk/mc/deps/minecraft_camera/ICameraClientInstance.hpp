//
// Created by emi on 10/21/26.
//

#pragma once
#include <optional>
#include <glm/glm.hpp>

class StrictEntityContext;
class Actor;
class LocalPlayer;

class ICameraClientInstance {
public:
    // NOLINTBEGIN
    virtual ~ICameraClientInstance() = default;
    virtual float getFrameAlpha() = 0;
    virtual Actor* getCameraActor() = 0;
    virtual std::optional<StrictEntityContext> getCameraEntity() = 0;
    virtual std::optional<bool> isUnderLiquid() = 0;
    virtual LocalPlayer* getLocalPlayer() = 0;
    virtual glm::vec2 getLookDelta() = 0;
    virtual float getSecondsSinceProgramStart() = 0;
    // NOLINTEND
};
