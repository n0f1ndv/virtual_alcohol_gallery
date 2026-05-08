#pragma once

#include "scene.hpp"
#include "camera.hpp"
#include "shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    Shader* shader;
    Camera* camera;
    Scene* scene;

    static void handleError(int error, const char* description);
    void handleResizing(int width, int height);
};
