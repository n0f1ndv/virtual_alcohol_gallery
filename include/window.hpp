#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class Window {
    private:
        int m_window_height;
        int m_window_length;
        GLFWwindow* m_window;

    public:
        Window(int window_height, int window_length, std::string window_title);

        void init();
        void loop();
};
