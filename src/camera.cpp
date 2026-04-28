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

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Camera::handleMouse(double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::mat4 Mc = glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    Mc = glm::rotate(Mc, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec4 dir_ = Mc * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    front = glm::normalize(glm::vec3(dir_));
}