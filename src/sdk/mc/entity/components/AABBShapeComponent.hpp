//
// Created by emi on 10/28/26.
//

#pragma once

#include <glm/glm.hpp>
#include "sdk/mc/world/phys/AABB.hpp"

struct AABBShapeComponent {
public:
    AABB mAABB;
    glm::vec2 mBBDim;
};
