#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    void handleKey(int key, int scancode, int action, int mods);
    void handleMouse(double xpos, double ypos);
    
    GLFWwindow* window;

    //pozycje startowe dla kamery oraz myszki
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 640.0f;
    float lastY = 360.0f; 
    bool firstMouse = true;
    

public:
    glm::vec3 position;

    glm::vec3 front = glm::vec3(0.0f, 0.0f, 10.0f); 

    Camera(GLFWwindow* window, glm::vec3 position);
    ~Camera();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
};
