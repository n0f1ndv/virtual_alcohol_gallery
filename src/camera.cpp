#include "camera.hpp"

#include <iostream>

Camera::Camera(GLFWwindow* window, glm::vec3 position) 
    : window(window)
    , position(position) {
    
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, Camera::keyCallback);
    glfwSetCursorPosCallback(window, Camera::cursorPositionCallback);
}

Camera::~Camera() {}

void Camera::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Camera* instance = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->handleKey(key, scancode, action, mods);
    }
}

void Camera::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    Camera* instance = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->handleMouse(xpos, ypos);
    }
}

void Camera::handleKey(int key, int scancode, int action, int mods) {
    std::cout << position.x << ", " << position.y << ", " << position.z << ", " << "\n";
}

void Camera::handleMouse(double xpos, double ypos) {
    std::cout << xpos << ", " << ypos << "\n";
}