#include "interaction.hpp"
#include <iostream>
#include <algorithm>

InteractionSystem::InteractionSystem() 
    : isDrinking(false), animTimer(0.0f), isInteractPressed(false) {}

bool InteractionSystem::RayIntersectsAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, BoundingBox box, float& intersectionDistance) {
    glm::vec3 invDir = 1.0f / (rayDir + glm::vec3(0.00001f)); 

    float t1 = (box.min.x - rayOrigin.x) * invDir.x;
    float t2 = (box.max.x - rayOrigin.x) * invDir.x;
    float t3 = (box.min.y - rayOrigin.y) * invDir.y;
    float t4 = (box.max.y - rayOrigin.y) * invDir.y;
    float t5 = (box.min.z - rayOrigin.z) * invDir.z;
    float t6 = (box.max.z - rayOrigin.z) * invDir.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax < 0 || tmin > tmax) return false;

    intersectionDistance = tmin;
    return true;
}

void InteractionSystem::Update(GLFWwindow* window, Camera* camera, Scene* scene, float deltaTime) {
    if (isDrinking) {
        animTimer += deltaTime;
        if (animTimer >= drinkDuration) {
            isDrinking = false;
            animTimer = 0.0f;
            std::cout << "[SYSTEM] Wypito butelke!\n";
        }
        return; 
    }

    // --- ZMIANA TUTAJ: Wykrywamy klawisz 'E' ---
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (!isInteractPressed) {
            isInteractPressed = true; 

            float reach = 4.0f; 
            float closestDist = 1000.0f;
            Entity* target = nullptr;

            for (auto& entity : scene->entities) {
                if (!entity.isInteractable || !entity.isVisible) continue;

                float hitDist = 0.0f;
                if (RayIntersectsAABB(camera->position, camera->front, entity.GetHitbox(), hitDist)) {
                    if (hitDist > 0.0f && hitDist < reach && hitDist < closestDist) {
                        closestDist = hitDist;
                        target = &entity;
                    }
                }
            }

            if (target != nullptr) {
                if (target->tag == "wine_bottle") {
                    std::cout << "[INTERAKCJA] Zebrano: " << target->tag << " (Odleglosc: " << closestDist << ")\n";
                    target->isVisible = false; 
                    isDrinking = true;         
                    animTimer = 0.0f;
                }
            }
        }
    } else {
        isInteractPressed = false; 
    }
}