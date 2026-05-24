#include "scene.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Light::Light(glm::vec3 position, glm::vec3 color, float intensity, float constant, float linear, float quadratic)
    : position(position), color(color), intensity(intensity), constant(constant), linear(linear), quadratic(quadratic) {}

Entity::Entity(Model* mod, glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, glm::vec3 col, bool interact, std::string t) 
    : model(mod), position(pos), rotation(rot), scale(sc), color(col), 
      isVisible(true), isInteractable(interact), tag(t) {
    hitbox.min = glm::vec3(0.0f);
    hitbox.max = glm::vec3(0.0f);
}

void Entity::SetHitboxLimits(glm::vec3 minOffset, glm::vec3 maxOffset) {
    hitbox.min = position + minOffset;
    hitbox.max = position + maxOffset;
}

BoundingBox Entity::GetHitbox() const {
    return hitbox;
}

void Entity::Draw(GLuint program) {
    if (isVisible && model != nullptr) {
        model->Draw(program, position, rotation, scale, color);
    }
}

Scene::Scene(GLuint program) : program{program} {
    models[0].Load("models/cube.obj");
    models[1].Load("models/wine.obj");
    models[2].Load("models/cigarette.obj");
    models[3].Load("models/stand.obj");
    models[4].Load("models/floor.obj");

    entities.push_back(Entity(&models[4], 
        glm::vec3(0.0f, -3.0f, 0.0f),   //position
        glm::vec3(0.0f),                //rotation
        glm::vec3(30.0f, 0.1f, 30.0f),  //scale
        glm::vec3(0.2f, 0.2f, 0.2f),    //color
        false));                        //canInteract

    for (int j = 0; j <= 4; j++) {
        for (int i = 0; i <= 9; i++) {
            entities.push_back(
                Entity(
                    &models[3], 
                    glm::vec3(-20.0f + (i * 5), -2.2f, -20.0f + (j * 10)), 
                    glm::vec3(0.0f), 
                    glm::vec3(0.5f, 0.9f, 0.5f), 
                    glm::vec3(0.8f, 0.7f, 0.4f), 
                    false,
                    "stand"
                )
            );
            
            Entity wine(
                &models[1], 
                glm::vec3(-20.0f + (i * 5), -0.25f, -20.0f + (j * 10)), 
                glm::vec3(-90.0f, 0.0f, 0.0f), 
                glm::vec3(0.06f, 0.06f, 0.06f), 
                glm::vec3(0.5f, 0.5f, 0.5f), 
                true, 
                "wine_bottle"
            );

            wine.SetHitboxLimits(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 1.0f, 0.5f));
            
            entities.push_back(wine);
        }
    }

    glUniform1i(glGetUniformLocation(program, "uNumLights"), LIGHTS_NUMBER);

    float radius = 15.0f;
    float height = 1.0f;

    int currentLightCount = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            float angle = ((float)currentLightCount / (float)LIGHTS_NUMBER) * 2.0f * glm::pi<float>();

            float x = radius * std::cos(angle);
            float z = radius * std::sin(angle);

            lights.push_back(
                Light(
                    glm::vec3(x, height, z), 
                    glm::vec3(1.0f, 0.75f, 0.0f),
                    0.9f,
                    1.0f,
                    0.060f,
                    0.020f
                )
            );

            currentLightCount++;
        }
    }
}

Scene::~Scene() {

}

void Scene::DrawModels() {
    for (auto& entity : entities) {
        entity.Draw(program);
    }
}

void Scene::ApplyLights() {
    std::string base;

    for (int i = 0; i < LIGHTS_NUMBER; i++) {
        base = "uLights[" + std::to_string(i) + "].";

        glUniform3fv(glGetUniformLocation(program, (base + "position").c_str()), 1, glm::value_ptr(lights[i].position));
        glUniform3fv(glGetUniformLocation(program, (base + "color").c_str()), 1, glm::value_ptr(lights[i].color));

        glUniform1f(glGetUniformLocation(program, (base + "intensity").c_str()), lights[i].intensity);
        glUniform1f(glGetUniformLocation(program, (base + "constant").c_str()), lights[i].constant);
        glUniform1f(glGetUniformLocation(program, (base + "linear").c_str()), lights[i].linear);
        glUniform1f(glGetUniformLocation(program, (base + "quadratic").c_str()), lights[i].quadratic);
    }
}
