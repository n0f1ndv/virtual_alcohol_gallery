#pragma once

#include <glm/glm.hpp>

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
};

bool CheckCollision(const BoundingBox& a, const BoundingBox& b);
BoundingBox TransformBox(BoundingBox base, glm::vec3 pos, glm::vec3 scale);
BoundingBox getPlayerBox(glm::vec3 pos);
