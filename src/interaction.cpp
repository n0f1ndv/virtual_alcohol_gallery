#include "interaction.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cmath>

InteractionSystem::InteractionSystem(GLuint ppProgram, GLuint program)
    : ppProgram{ppProgram}
    , program{program} {}

InteractionSystem::~InteractionSystem() {}

bool InteractionSystem::RayIntersectsAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, BoundingBox box, float& dist) {
    glm::vec3 invDir = 1.0f / (rayDir + glm::vec3(0.00001f));
    glm::vec3 tMin = (box.min - rayOrigin) * invDir;
    glm::vec3 tMax = (box.max - rayOrigin) * invDir;
    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);
    float tNear = std::max(std::max(t1.x, t1.y), t1.z);
    float tFar = std::min(std::min(t2.x, t2.y), t2.z);
    
    if (tNear > tFar || tFar < 0) return false;
    
    dist = tNear;
    return true;
}

void InteractionSystem::Update(GLFWwindow* window, Camera* camera, Scene* scene, float deltaTime) {
    if (state == HandState::IDLE) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !isInteractPressed) {
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

            if (target != nullptr && target->tag == "wine_bottle") {
                targetItem = target;
                bottleStartPos = target->position;
                bottleStartPos.y += 0.5f; 

                state = HandState::REACHING_OUT;
                animTimer = 0.0f;
            }
        }
        // --- WYCIĄGANIE PAPIEROSA POD KLAWISZEM 'C' ---
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !isInteractPressed) {
            isInteractPressed = true;
            isSmoking = true;
            targetItem = nullptr; 
            
            glm::mat4 camWorld = glm::inverse(camera->GetV());
            glm::vec3 hidePos = glm::vec3(camWorld * glm::vec4(0.4f, -1.5f, -0.2f, 1.0f));
            
            bottleStartPos = hidePos; 
            
            state = HandState::RETRACTING; 
            animTimer = 0.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE && 
            glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
            isInteractPressed = false;
        }
    } 
    else {
        // --- PRZEJŚCIA MIĘDZY PIĘCIOMA STANAMI ---
        animTimer += deltaTime;
        if (animTimer >= duration) {
            if (state == HandState::REACHING_OUT) {
                if (targetItem) targetItem->isVisible = false;
                state = HandState::RETRACTING;
                animTimer = 0.0f;
            } 
            else if (state == HandState::RETRACTING) {
                state = HandState::DRINKING;
                animTimer = 0.0f;
            }
            else if (state == HandState::DRINKING) {
                state = HandState::HIDING;
                animTimer = 0.0f;
            }
            else if (state == HandState::HIDING) {
                state = HandState::IDLE;

                glm::vec3 flatFront = glm::normalize(glm::vec3(camera->front.x, 0.0f, camera->front.z));
                glm::vec3 flatRight = glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));
                glm::vec3 dropPos = camera->position - (flatFront * 1.0f) + (flatRight * 0.2f);
                dropPos.y = -2.7f; 
                float randomYRotation = static_cast<float>(rand() % 360);
                glm::vec3 lyingRotation = glm::vec3(0.0f, randomYRotation, 0.0f);

                if (isSmoking) {
                    scene->entities.push_back(Entity(
                        &scene->models[2],
                        dropPos,                            
                        lyingRotation,                      
                        glm::vec3(0.02f),
                        glm::vec3(1.0f, 1.0f, 1.0f),
                        false,                              
                        "dropped_cigarette"                 
                    ));
                    isSmoking = false;
                } 
                else if (targetItem != nullptr) {
                    scene->entities.push_back(Entity(
                        targetItem->GetModel(),
                        dropPos,                            
                        lyingRotation,                      
                        glm::vec3(0.06f),                   
                        targetItem->color,                  
                        false,                              
                        "empty_bottle"                      
                    ));
                }

                targetItem = nullptr;

                drunknessEffect++;

                glUseProgram(ppProgram);
                glUniform1i(glGetUniformLocation(ppProgram, "drunk"), drunknessEffect);
            }
        }
    }
}

void InteractionSystem::DrawHand(Scene* scene, Camera* camera) {
    if (state == HandState::IDLE) return;

    float t = glm::clamp(animTimer / duration, 0.0f, 1.0f);

    glm::vec3 skinColor = glm::vec3(0.9f, 0.75f, 0.65f);

    // 1. POZYCJE KLUCZOWE W UKŁADZIE KAMERY
    glm::mat4 camWorld = glm::inverse(camera->GetV());
    
    glm::vec3 shoulderPos = glm::vec3(camWorld * glm::vec4(0.3f, -0.4f, -0.6f, 1.0f)); 
    glm::vec3 hidePos     = glm::vec3(camWorld * glm::vec4(0.4f, -1.5f, -0.2f, 1.0f));  

    glm::vec3 mouthPos    = glm::vec3(camWorld * glm::vec4(0.0f, 0.3f, -0.3f, 1.0f)); 
    glm::vec3 smokePos    = glm::vec3(camWorld * glm::vec4(0.1f, -0.1f, -0.4f, 1.0f)); 

    glm::vec3 activeMouthPos = isSmoking ? smokePos : mouthPos;

    float currentShoulderAngle = 0.0f;
    float currentElbowAngle = 0.0f;
    glm::vec3 currentBaseDir;
    
    float maxTilt = isSmoking ? 90.0f : 160.0f;
    float maxTwist = isSmoking ? -90.0f : -50.0f; 
    
    float drinkTilt = 0.0f; 
    float armTwist = 0.0f;

    // 2. MATEMATYKA ZALEŻNA OD 5 STANÓW
    if (state == HandState::REACHING_OUT) {
        float dist = glm::distance(shoulderPos, bottleStartPos);
        dist = glm::clamp(dist, 0.01f, L1 + L2 - 0.01f);

        float cosAlpha = (L1 * L1 + L2 * L2 - dist * dist) / (2.0f * L1 * L2);
        float alpha = glm::degrees(std::acos(glm::clamp(cosAlpha, -1.0f, 1.0f)));

        float targetShoulderAngle = 90.0f - (alpha / 2.0f);
        float targetElbowAngle    = 180.0f - alpha;

        if (t < 0.5f) {
            float localT = glm::smoothstep(0.0f, 1.0f, t * 2.0f);
            currentShoulderAngle = glm::mix(0.0f, 90.0f, localT);
            currentElbowAngle    = glm::mix(0.0f, 160.0f, localT);
        } else {
            float localT = glm::smoothstep(0.0f, 1.0f, (t - 0.5f) * 2.0f);
            currentShoulderAngle = glm::mix(90.0f, targetShoulderAngle, localT);
            currentElbowAngle    = glm::mix(160.0f, targetElbowAngle, localT);
        }

        glm::vec3 downDir = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 aimDir = glm::normalize(bottleStartPos - shoulderPos);
        float dirProgress = glm::clamp(t * 2.0f, 0.0f, 1.0f);
        currentBaseDir = glm::normalize(glm::mix(downDir, aimDir, dirProgress));
        drinkTilt = 0.0f; // Na stole butelka stoi prosto

    } 
    else {
        // --- STANY: RETRACTING, DRINKING, HIDING ---
        glm::vec3 currentTarget;
        float smoothT = glm::smoothstep(0.0f, 1.0f, t);

        if (state == HandState::RETRACTING) {
            currentTarget = glm::mix(bottleStartPos, activeMouthPos, smoothT);
            drinkTilt = smoothT * maxTilt;
            armTwist = smoothT * maxTwist;
        } 
        else if (state == HandState::DRINKING) {
            currentTarget = activeMouthPos;
            drinkTilt = maxTilt;
            armTwist = maxTwist;
        } 
        else if (state == HandState::HIDING) {
            glm::vec3 straightTarget = glm::mix(activeMouthPos, hidePos, smoothT);
            float arcFactor = 4.0f * smoothT * (1.0f - smoothT);
            glm::vec3 forward = camera->front;
            glm::vec3 rightDir = glm::normalize(glm::cross(camera->front, glm::vec3(0.0f, 1.0f, 0.0f)));
            
            float pushForwardDistance = 1.2f;
            float pushRightDistance   = 0.4f;
            glm::vec3 arcOffset = (forward * pushForwardDistance) + (rightDir * pushRightDistance);
            currentTarget = straightTarget + (arcOffset * arcFactor);
            
            drinkTilt = glm::mix(maxTilt, 0.0f, smoothT);
            armTwist  = glm::mix(maxTwist, 0.0f, smoothT);
        }

        float dist = glm::distance(shoulderPos, currentTarget);
        dist = glm::clamp(dist, 0.01f, L1 + L2 - 0.01f);

        float cosAlpha = (L1 * L1 + L2 * L2 - dist * dist) / (2.0f * L1 * L2);
        float alpha = glm::degrees(std::acos(glm::clamp(cosAlpha, -1.0f, 1.0f)));

        currentShoulderAngle = 90.0f - (alpha / 2.0f);
        currentElbowAngle    = 180.0f - alpha;
        currentBaseDir       = glm::normalize(currentTarget - shoulderPos);
    }

    glm::vec3 zAxis = currentBaseDir;
    
    glm::vec3 camRight = glm::normalize(glm::vec3(camWorld[0])); 
    
    glm::vec3 yAxis = glm::cross(camRight, zAxis);
    if (glm::length(yAxis) < 0.001f) yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    yAxis = glm::normalize(yAxis);
    
    glm::vec3 xAxis = glm::normalize(glm::cross(yAxis, zAxis));

    //macierz obrotu
    glm::mat4 baseRot = glm::mat4(1.0f);
    baseRot[0] = glm::vec4(xAxis, 0.0f);
    baseRot[1] = glm::vec4(yAxis, 0.0f);
    baseRot[2] = glm::vec4(zAxis, 0.0f);

    baseRot = glm::rotate(baseRot, glm::radians(armTwist), glm::vec3(0.0f, 0.0f, 1.0f));

    // A. RAMIĘ
    glm::mat4 m1 = glm::translate(glm::mat4(1.0f), shoulderPos) * baseRot;
    m1 = glm::rotate(m1, glm::radians(currentShoulderAngle), glm::vec3(1.0f, 0.0f, 0.0f)); 

    glm::mat4 render1 = m1;
    render1 = glm::scale(render1, glm::vec3(armThickness / 2.0f, armThickness / 2.0f, L1 / 2.0f));
    render1 = glm::translate(render1, glm::vec3(0.0f, 0.0f, 1.0f)); 
    scene->models[0].DrawMatrix(program, render1, skinColor);

    // B. PRZEDRAMIĘ
    glm::mat4 m2 = glm::translate(m1, glm::vec3(0.0f, 0.0f, L1));
    m2 = glm::rotate(m2, glm::radians(-currentElbowAngle), glm::vec3(1.0f, 0.0f, 0.0f)); 

    glm::mat4 render2 = m2;
    render2 = glm::scale(render2, glm::vec3(armThickness / 2.0f, armThickness / 2.0f, L2 / 2.0f));
    render2 = glm::translate(render2, glm::vec3(0.0f, 0.0f, 1.0f));
    scene->models[0].DrawMatrix(program, render2, skinColor);


    // C. BUTELKA LUB PAPIEROS W DŁONI
    if (state != HandState::REACHING_OUT) {
        
        glm::mat4 wristMatrix = glm::translate(m2, glm::vec3(0.0f, 0.0f, L2));
        glm::vec3 wristWorldPos = glm::vec3(wristMatrix[3]); 

        glm::mat4 bottleMatrix = glm::mat4(1.0f);
        bottleMatrix = glm::translate(bottleMatrix, wristWorldPos);
        
        glm::mat4 camRot = camWorld;
        camRot[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); 
        bottleMatrix *= camRot;

        bottleMatrix = glm::rotate(bottleMatrix, glm::radians(-90.0f + drinkTilt), glm::vec3(1.0f, 0.0f, 0.0f));

        if (isSmoking) {
            bottleMatrix = glm::rotate(bottleMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            
            bottleMatrix = glm::translate(bottleMatrix, glm::vec3(-0.1f, -0.05f, 0.75f));
            bottleMatrix = glm::rotate(bottleMatrix, glm::radians(4.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        } 
        else {
            bottleMatrix = glm::translate(bottleMatrix, glm::vec3(0.0f, -0.2f, -0.5f)); 
        }

        glm::vec3 itemScale = isSmoking ? glm::vec3(0.02f) : glm::vec3(0.06f);
        bottleMatrix = glm::scale(bottleMatrix, itemScale);

        if (isSmoking) {
            scene->models[2].DrawMatrix(program, bottleMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        } 
        else if (targetItem != nullptr && targetItem->GetModel() != nullptr) {
            targetItem->GetModel()->DrawMatrix(program, bottleMatrix, targetItem->color);
        }
    }
}