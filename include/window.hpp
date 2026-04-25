#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

class Window {
    private:
        float windowHeight;
        float windowLength;
        GLFWwindow* window;

    public:
        Window(float windowHeight, float windowLength, std::string windowTitle);
        ~Window();

        void Loop();
};

void errorCallback(int error, const char* description);
