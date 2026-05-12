#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "scene.hpp"

class InteractionSystem {
public:
    bool isDrinking = false;
    float animTimer = 0.0f;
    const float drinkDuration = 0.5f;

    InteractionSystem();

    void Update(GLFWwindow* window, Camera* camera, Scene* scene, float deltaTime);

private:
    bool isInteractPressed = false;

    bool RayIntersectsAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, BoundingBox box, float& intersectionDistance);
};