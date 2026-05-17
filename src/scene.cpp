#include "scene.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


Entity::Entity(Model* mod, glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, glm::vec3 col, bool interact, std::string t) 
    : model(mod), position(pos), rotation(rot), scale(sc), color(col), 
      isVisible(true), isInteractable(interact), tag(t) {
    // Domyślny, pusty hitbox (zostanie nadpisany, jeśli obiekt ma kolizję)
    hitbox.min = glm::vec3(0.0f);
    hitbox.max = glm::vec3(0.0f);
}

//--------------------hitboxy klikania--------------------
void Entity::SetHitboxLimits(glm::vec3 minOffset, glm::vec3 maxOffset) {
    hitbox.min = position + minOffset;
    hitbox.max = position + maxOffset;
}

BoundingBox Entity::GetHitbox() const {
    return hitbox;
}
//------------------------------------------------------------

void Entity::Draw(GLuint program) {
    if (isVisible && model != nullptr) {
        // Zobacz jakie to czyste! Obiekt sam woła model i przekazuje mu SWOJE pozycje
        model->Draw(program, position, rotation, scale, color);
    }
}


Scene::Scene(GLuint program) : program{program} {
    
    models[0].Load("models/cube.obj");
    models[1].Load("models/wine_no_tx.obj");
    models[2].Load("models/cigarette.obj");
    models[3].Load("models/tree.obj");
    models[4].Load("models/stand.obj");
    models[5].Load("models/bottle1.obj");
    models[6].Load("models/bottle2.obj");

    glm::vec3 wine_color;

    // Floor     
    entities.push_back(Entity(&models[0], 
        glm::vec3(0.0f, -3.0f, 0.0f),   //position
        glm::vec3(0.0f),                //rotation
        glm::vec3(30.0f, 0.1f, 30.0f),  //scale
        glm::vec3(0.2f, 0.2f, 0.2f),    //color
        false));                        //canInteract

    entities.push_back(Entity(
        &models[3],
        glm::vec3(0.0f, -3.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        false
    ));

    // Bottles on stands
    for (int j = 0; j <= 4; j++) {
        for (int i = 0; i <= 9; i++) {
             
            if (i % 2 == 1) {
                Entity wine(
                    &models[5], 
                    glm::vec3(-20.0f + (i * 5), -0.1f, -20.0f + (j * 10)), 
                    glm::vec3(0.0f, 0.0f, 0.0f), 
                    glm::vec3(0.075f), 
                    wine_color, 
                    true, 
                    "wine_bottle"
                );

                wine.SetHitboxLimits(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 1.0f, 0.5f));
                entities.push_back(wine);
            }
            else {
                Entity wine(
                    &models[6], 
                    glm::vec3(-20.0f + (i * 5), -0.2f, -20.0f + (j * 10)), 
                    glm::vec3(0.0f, 0.0f, 0.0f), 
                    glm::vec3(0.15f), 
                    wine_color, 
                    true, 
                    "wine_bottle"
                );

                wine.SetHitboxLimits(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 1.0f, 0.5f));
                entities.push_back(wine);
            }

            entities.push_back(Entity(&models[4], 
                glm::vec3(-20.0f + (i * 5), -1.5f, -20.0f + (j * 10)), 
                glm::vec3(0.0f), 
                glm::vec3(0.6f, 0.6f, 0.6f), 
                glm::vec3(0.8f, 0.7f, 0.4f), 
                false,
                "stand"
            ));
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

void Scene::DrawLights() {
    glUniform3fv(glGetUniformLocation(program, "uLightPos"), 1, glm::value_ptr(lightPosition));
    glUniform3fv(glGetUniformLocation(program, "uColor"), 1, glm::value_ptr(modelColor));
}
