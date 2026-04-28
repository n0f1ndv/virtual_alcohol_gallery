#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

// TODO:
// Move this into class (Top priotity)
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
