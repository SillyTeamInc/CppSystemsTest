//
// Created by emi on 10/27/2026.
//

#pragma once

#include "hvs/hvs.hpp"


// i wasnt sure which one i needed lol
namespace fov_check_event {
    class pre : public hvs::cancelable_event {
    public:
        LevelRendererPlayer* camera;
        float a;
        // undefined at this point, should be set by listeners
        float result = 0.f;
        bool enableVariableFov;

        pre(LevelRendererPlayer* camera, float a, bool enableVariableFov) : camera(camera), a(a), enableVariableFov(enableVariableFov) {}
    };

    class post : public hvs::event {
    public:
        LevelRendererPlayer* camera;
        float a;
        float result;
        bool enableVariableFov;

        post(LevelRendererPlayer* camera, float a, float result, bool enableVariableFov) : camera(camera), a(a), result(result), enableVariableFov(enableVariableFov) {}
    };
}