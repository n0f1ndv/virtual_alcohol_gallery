// #include "collision.hpp"

// BoundingBox::BoundingBox() {

// }

// BoundingBox::~BoundingBox() {
    
// }

// bool BoundingBox::CheckCollision(const BoundingBox& a, const BoundingBox& b) {
//     return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
//            (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
//            (a.min.z <= b.max.z && a.max.z >= b.min.z);
// }

// BoundingBox BoundingBox::TransformBox(BoundingBox base, glm::vec3 pos, glm::vec3 scale) {
//     BoundingBox transformed;
//     transformed.min = (base.min * scale) + pos;
//     transformed.max = (base.max * scale) + pos;
//     return transformed;
// }

// BoundingBox BoundingBox::getPlayerBox(glm::vec3 pos) {
//     float size = 1.0f;
//     min = pos - glm::vec3(size, 1.0f, size); 
//     max = pos + glm::vec3(size, 1.0f, size);
// }
