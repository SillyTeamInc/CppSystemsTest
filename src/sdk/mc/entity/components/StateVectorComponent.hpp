//
// Created by emi on 10/28/26.
//

#pragma once

#include <glm/glm.hpp>

struct StateVectorComponent {
public:
    glm::vec3 mPos;
    glm::vec3 mPosPrev;
    glm::vec3 mPosDelta;
};
