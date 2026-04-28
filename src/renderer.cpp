#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void DrawEntity(Shader& shader, Model& model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    
    // Tworzenie macierzy modelu (M)
    glm::mat4 M = glm::mat4(1.0f);
    
    // Kolejność: Przesunięcie -> Obrót -> Skalowanie
    M = glm::translate(M, position);
    
    // Obracamy wzdłuż każdej osi
    M = glm::rotate(M, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    M = glm::rotate(M, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::rotate(M, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    M = glm::scale(M, scale);

    // Wysyłanie danych do karty graficznej
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "M"), 1, GL_FALSE, glm::value_ptr(M));

    // Wywołanie rysowania na przekazanym obiekcie
    model.Draw(shader.GetProgramID());
}