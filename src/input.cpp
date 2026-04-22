#include "input.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod) {
    if (action == GLFW_PRESS) {
        std::cout << "I'm pressed ~0~\n";
    }

    if (action == GLFW_RELEASE) {
        std::cout << "I'm released! 0o0\n";
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    std::cout << xpos << ", " << ypos << "\n";
}
