#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include "camera.hpp"
#include "scene.hpp"

// Pięć stanów pełnego cyklu interakcji
enum class HandState {
    IDLE,           
    REACHING_OUT,   // Sięganie po butelkę na stole
    RETRACTING,     // Przyciąganie butelki do ust
    DRINKING,       // Używanie przedmiotu (picie lub palenie)
    HIDING          // Chowanie przedmiotu (usta -> dół)
};

class InteractionSystem {
public:
    HandState state = HandState::IDLE;
    float animTimer = 0.0f;
    const float duration = 1.0f;

    float L1 = 1.0f;
    float L2 = 1.0f;
    float armThickness = 0.15f;

    InteractionSystem(GLuint ppProgram, GLuint program);
    
    void Update(GLFWwindow* window, Camera* camera, Scene* scene, float deltaTime);
    void DrawHand(Scene* scene, Camera* camera);
    
    bool IsMovementBlocked() const { 
        return state == HandState::REACHING_OUT || (state == HandState::RETRACTING && isSmoking == false); 
    }

private:
    Entity* targetItem = nullptr;
    glm::vec3 bottleStartPos;
    bool isInteractPressed = false;
    bool isSmoking = false;
    
    GLuint ppProgram;
    GLuint program;

    bool RayIntersectsAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, BoundingBox box, float& dist);
};