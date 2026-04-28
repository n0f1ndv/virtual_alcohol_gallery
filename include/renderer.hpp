#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"
#include "model.hpp"

// Funkcja rysująca. 
// Argument wymagany to załadowany obiekt Model. 
// Reszta (pozycja, obrót, skala) ma wartości domyślne.
void DrawEntity(
    Shader& shader, 
    Model& model, 
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), // Kąty w stopniach (X, Y, Z)
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
);