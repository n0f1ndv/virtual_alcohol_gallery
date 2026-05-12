#pragma once

#include "model.hpp"

#include <GL/glew.h>

#include <vector>
#include <string>

//do przeniesienia
class BoundingBox {
    public:
        glm::vec3 min;
        glm::vec3 max;
};
//klasa na kazdy obiekt mowiaca np. czy moze wejsc w interakcje
class Entity {
private:
    Model* model;
    BoundingBox hitbox;

public:

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 color;
    
    bool isVisible;
    bool isInteractable;
    std::string tag;

    Entity(Model* mod, glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, glm::vec3 col, bool interact, std::string t = "");

    void SetHitboxLimits(glm::vec3 minOffset, glm::vec3 maxOffset);
    BoundingBox GetHitbox() const;

    void Draw(GLuint program);
};

class Scene {
public:
    Scene(GLuint program);
    ~Scene();

    void DrawModels();
    void DrawLights();

    //wektor przechowujacy obiekty
    std::vector<Entity> entities;

private:
    GLuint program;

    // TODO: Fix manageing models and lights BECUASE THIS SUCKS
    Model models[2];

    glm::vec3 modelColor = glm::vec3(0.792f, 0.929f, 1.000f);
    glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, -4.0f);
};
