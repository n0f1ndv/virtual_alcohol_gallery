#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

class Window {
public:
    Window(float windowHeight, float windowLength, std::string windowTitle);
    ~Window();

    void Loop();

    static void errorCallback(int error, const char* description);
    static void windowResizeCallback(GLFWwindow* window, int width, int height);

private:
    float windowHeight;
    float windowLength;
    float aspectRatio;
    GLFWwindow* window;

    static void handleError(int error, const char* description);
    void handleResizing(int width, int height);
};
