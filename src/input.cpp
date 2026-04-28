#include "input.hpp"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
    if (action == GLFW_PRESS) {
        std::cout << "I'm pressed ~0~\n";
    }

    if (action == GLFW_RELEASE) {
        std::cout << "I'm released! 0o0\n";
    }
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    std::cout << xpos << ", " << ypos << "\n";
}
