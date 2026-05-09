#include "camera.hpp"

#include "collision.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

Camera::Camera(GLFWwindow* window, GLuint program, glm::vec3 position) 
    : position(position)
    , window(window)
    , program(program) {
    
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, Camera::keyCallback);
    glfwSetCursorPosCallback(window, Camera::cursorPositionCallback);
}

Camera::~Camera() {}

void Camera::Update(float aspectRatio) {
    if (fly) 
        flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    else
        flatFront = glm::normalize(glm::vec3(front.x, front.y, front.z));
    
    right = glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    velocity = (flatFront * speed_z) - (right * speed_y);
    
    delta = (float)glfwGetTime();
    totalTime += delta;

    position += velocity * delta;
    glfwSetTime(0);

    P = glm::perspective(glm::radians(50.0f), aspectRatio, 1.0f, 50.0f);
    V = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));

    glUniform3fv(glGetUniformLocation(program, "uViewPos"), 1, glm::value_ptr(position));

    glUniformMatrix4fv(glGetUniformLocation(program, "P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(glGetUniformLocation(program, "V"), 1, false, glm::value_ptr(V));

    glUniform1f(glGetUniformLocation(program, "time"), totalTime);
}

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
    // std::cout << position.x << ", " << position.y << ", " << position.z << ", " << "\n";

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
        
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        fly = (fly == true) ? false : true;
    }

    if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) speed_y = speed;
		if (key == GLFW_KEY_D) speed_y = -speed;
		if (key == GLFW_KEY_W) speed_z = speed;
		if (key == GLFW_KEY_S) speed_z = -speed;
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) speed_y = 0;
		if (key == GLFW_KEY_D) speed_y = 0;	

		if (key == GLFW_KEY_W) speed_z = 0;
		if (key == GLFW_KEY_S) speed_z = 0;
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

inline float fluctuateFOV(float base, float time) {
    return glm::radians(base) + ((std::sin(time) + 1) / 2);
}