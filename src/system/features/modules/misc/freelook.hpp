//
// Created by emi on 10/20/26.
//

#pragma once

#include <util/keyboard.hpp>
#include <windows.h>
#include "../module.hpp"
#include "events/local_player_turn_event.hpp"
#include "sdk/mc/deps/minecraft_camera/Spherical.hpp"


class EnumOption;

class freelook : public module_base<freelook> {
public:
    static constexpr module_info info {"Freelook", "Yeah", category::misc, static_cast<int>(keyboard::key::f), false, false};

    explicit freelook() : module_base(info) {}

    int mLastCameraState = 0;
    glm::vec2 mLookingAngles = glm::vec2(0.0f, 0.0f);
    bool mHadComponent = false;
    std::unordered_map<EntityId, glm::vec2> mOriginalRotRads;
    std::unordered_map<EntityId, int> mCameras{};
    std::unordered_map<EntityId, Spherical> mOrbitRotsC{};
    std::unordered_map<EntityId, Spherical> mOrbitRotsI{};


    bool mResetRot = false;
    EnumOption* mThirdPerson = nullptr;
    glm::vec2 mHeadYaw;

    void on_enable() override;
    void on_disable() override;
    void on_lp_turn(local_player_turn_event& event);
};
