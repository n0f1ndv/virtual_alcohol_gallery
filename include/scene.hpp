#pragma once

#include "model.hpp"

#include <GL/glew.h>

#include <vector>
#include <string>

const int LIGHTS_NUMBER = 8;

class Light {
public:
    Light(glm::vec3 position, glm::vec3 color, float intensity, float constant, float linear, float quadratic);

    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    
    float constant;
    float linear;
    float quadratic;
};

class BoundingBox {
public:
    glm::vec3 min;
    glm::vec3 max;
};

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

    Model* GetModel() const {
        return model;
    }
};

class Scene {
public:
    Scene(GLuint program);
    ~Scene();

    void DrawModels();
    void ApplyLights();

    std::vector<Entity> entities;

    Model models[5];

private:
    GLuint program;

    std::vector<Light> lights;
};
