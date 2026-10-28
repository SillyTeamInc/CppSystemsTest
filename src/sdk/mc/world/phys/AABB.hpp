//
// Created by emi on 10/28/26.
//

#pragma once

#include <glm/glm.hpp>

struct AABB {
    union {
        glm::vec3 mMin, x, r, s;
    };
    union {
        glm::vec3 mMax, y, g, t, z, b, p;
    };

    AABB(const glm::vec3& position, const glm::vec3& size) {
        mMin.x = position.x + size.x;
        mMin.y = position.y + size.y;
        mMin.z = position.z + size.z;
        mMax.x = position.x;
        mMax.y = position.y;
        mMax.z = position.z;
    }

    AABB(const glm::vec3& min, const glm::vec3& max, bool) : mMin(min), mMax(max) {}
    AABB() : mMin(0), mMax(0) {}

    [[nodiscard]] glm::vec3 getClosestPoint(const glm::vec3& point) const {
        glm::vec3 result;
        result.x = point.x < mMin.x ? mMin.x : (point.x > mMax.x ? mMax.x : point.x);
        result.y = point.y < mMin.y ? mMin.y : (point.y > mMax.y ? mMax.y : point.y);
        result.z = point.z < mMin.z ? mMin.z : (point.z > mMax.z ? mMax.z : point.z);
        return result;
    }

    bool operator==(const AABB& other) const { return this->mMin == other.mMin && this->mMax == other.mMax; }
};
