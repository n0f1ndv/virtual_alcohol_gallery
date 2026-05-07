#include "scene.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Scene::Scene(GLuint program) 
    : program{program} {
    models[0].Load("models/cube.obj");
    models[1].Load("models/wine.obj");
}

Scene::~Scene() {

}

void Scene::DrawModels() {
    // Floor     
    models[0].Draw(program, 
        glm::vec3(0.0f, -3.0f, 0.0f),   // position
        glm::vec3(0.0f),                // rotation
        glm::vec3(30.0f, 0.1f, 30.0f),  // scale
        glm::vec3(0.2f,0.2f,0.2f)       // color
    ); 

    // Bottles on stands
    for (int j = 0; j <= 10; j += 10) {
        for (int i = 0; i <= 50; i += 5) {
            models[0].Draw(program, 
                glm::vec3(-20.0f + i, -1.5f, -20.0f + j),
                glm::vec3(0.0f),
                glm::vec3(0.5f, 1.25f, 0.5f),
                glm::vec3(0.8f,0.7f,0.4f)
            ); 
        
            models[1].Draw(program, 
                glm::vec3(-20.0f + i, -0.25f, -20.0f + j),
                glm::vec3(-90.0f, 0.0f, 0.0f),
                glm::vec3(0.06f, 0.06f, 0.06f),
                glm::vec3(0.0f,0.5f,0.5f)
            ); 
        }
    }
}

void Scene::DrawLights() {
    glUniform3fv(glGetUniformLocation(program, "uLightPos"), 1, glm::value_ptr(lightPosition));
    glUniform3fv(glGetUniformLocation(program, "uColor"), 1, glm::value_ptr(modelColor));
}
