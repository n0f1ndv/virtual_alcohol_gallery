#include "window.hpp"

Window::Window(int window_height, int window_length, std::string window_title)
    : m_window_height{window_height}
    , m_window_length{window_length} {
    if (!glfwInit()) {
        std::cout << "Failed to initialize library\n";
    }

    m_window = glfwCreateWindow(m_window_height, m_window_length, window_title.c_str(), NULL, NULL);
    if (!m_window) {
        std::cout << "Failed to create window\n";

        glfwTerminate();
    }
}

void Window::init() {
    
}

void Window::loop() {
    glfwMakeContextCurrent(m_window);

    while (!glfwWindowShouldClose(m_window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(m_window);

        glfwPollEvents();
    }

    // TODO: Move this into separate terminate method
    glfwTerminate();
}