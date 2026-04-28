#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
private:
    void handleKey(int key, int scancode, int action, int mods);
    void handleMouse(double xpos, double ypos);
    
    GLFWwindow* window;

public:
    glm::vec3 position;

    Camera(GLFWwindow* window, glm::vec3 position);
    ~Camera();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
};
