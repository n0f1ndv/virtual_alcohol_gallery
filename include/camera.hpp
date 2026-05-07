#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, 10.0f); 

    //zmienne do ruchu
    float speed_y = 0.0f;
    float speed_z = 0.0f;
    bool fly = true;

    Camera(GLFWwindow* window, GLuint program, glm::vec3 position);
    ~Camera();

    void Update(float aspectRatio);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

private:
    void handleKey(int key, int scancode, int action, int mods);
    void handleMouse(double xpos, double ypos);
    
    GLFWwindow* window;
    GLuint program;

    glm::mat4 P;
    glm::mat4 V;

    glm::vec3 flatFront;
    glm::vec3 nextPosition;
    glm::vec3 right;
    glm::vec3 velocity;

    //pozycje startowe dla kamery oraz myszki
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 640.0f;
    float lastY = 360.0f; 
    bool firstMouse = true;

    float speed = 8;
};
