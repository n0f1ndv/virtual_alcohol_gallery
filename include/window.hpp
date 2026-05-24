#pragma once

#include "scene.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "interaction.hpp"
#include "postProcessing.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class Window {
public:
    Window(float windowHeight, float windowWidth, std::string windowTitle);
    ~Window();

    void Loop();

    static void errorCallback(int error, const char* description);
    static void windowResizeCallback(GLFWwindow* window, int width, int height);

private:
    float windowHeight;
    float windowWidth;
    float aspectRatio;

    double delta;
    double totalTime;
    double frameTime;
    double lastTime;

    GLFWwindow* window;

    Shader* shader;
    Shader* ppShader;
    PostProcessing* postProcessing;
    Camera* camera;
    Scene* scene;
    InteractionSystem* interaction;

    void Clock();

    static void handleError(int error, const char* description);
    void handleResizing(int width, int height);
};
