#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

// TODO:
// Move this into class (Top priotity)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
