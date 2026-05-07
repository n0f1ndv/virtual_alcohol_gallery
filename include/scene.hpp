#pragma once

#include "model.hpp"

#include <GL/glew.h>

class Scene {
public:
    Scene(GLuint program);
    ~Scene();

    void DrawModels();
    void DrawLights();

private:
    GLuint program;

    // TODO: Fix manageing models and lights BECUASE THIS SUCKS
    Model models[2];

    glm::vec3 modelColor = glm::vec3(0.792f, 0.929f, 1.000f);
    glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, -4.0f);
};
