#include "window.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "collision.hpp"

#include <cmath>

// TODO:
// * Clean up includes

Window::Window(float windowLength, float windowHeight, std::string windowTitle)
    : windowHeight{windowHeight}
    , windowLength{windowLength}
    , aspectRatio{windowLength / windowHeight} {
    if (!glfwInit()) {
        std::cerr << "ERROR: Failed to initialize GLFW\n";
    }

    window = glfwCreateWindow(windowLength, windowHeight, windowTitle.c_str(), NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: Failed to create window\n";

        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: Failed to initialize GLEW\n";
        glfwTerminate();
    }

    glClearColor(1.000f, 0.780f, 0.918f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, windowLength / 2, windowHeight / 2);

    glfwSetWindowSizeCallback(window, Window::windowResizeCallback);
    glfwSetErrorCallback(Window::errorCallback);
}

Window::~Window() {
    glfwTerminate();
}

void Window::Loop() {
    Shader shader("shaders/fragmentModel.glsl", "shaders/vertexModel.glsl");
    
    Camera camera(window, shader.program, glm::vec3(0.0f, 0.0f, -10.0f));

    Model cube;
    cube.Load("models/cube.obj");

    Model wine;
    wine.Load("models/wine.obj");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
    
        camera.Update(aspectRatio);
        
        // This piece of garbage goes to the scene class
        glm::vec3 modelColor = glm::vec3(0.792f, 0.929f, 1.000f);
        glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, -4.0f);

        glUniform3fv(glGetUniformLocation(shader.program, "uLightPos"), 1, glm::value_ptr(lightPosition));
        glUniform3fv(glGetUniformLocation(shader.program, "uColor"), 1, glm::value_ptr(modelColor));

        cube.Draw(shader.program, 
            glm::vec3(0.0f, -3.0f, 0.0f),   // position
            glm::vec3(0.0f),                // rotation
            glm::vec3(20.0f, 0.1f, 20.0f),  // scale
            glm::vec3(0.2f,0.2f,0.2f)       // color
        ); 

        cube.Draw(shader.program, 
            glm::vec3(4.0f, -1.5f, 2.0f),
            glm::vec3(0.0f),
            glm::vec3(0.5f, 1.25f, 0.5f),
            glm::vec3(0.8f,0.7f,0.4f)
        ); 

        wine.Draw(shader.program, 
            glm::vec3(4.0f, -0.25f, 2.0f),
            glm::vec3(-90.0f, 0.0f, 0.0f),
            glm::vec3(0.06f, 0.06f, 0.06f),
            glm::vec3(0.0f,0.5f,0.5f)
        ); 

        cube.Draw(shader.program, 
            glm::vec3(4.0f, -1.5f, -2.0f),
            glm::vec3(0.0f),
            glm::vec3(0.5f, 1.25f, 0.5f),
            glm::vec3(0.2f,0.2f,0.2f)
        ); 

        wine.Draw(shader.program, 
            glm::vec3(4.0f, -0.25f, -2.0f),
            glm::vec3(-90.0f, 0.0f, 0.0f),
            glm::vec3(0.06f, 0.06f, 0.06f),
            glm::vec3(1.0f,0.0f,0.0f)
        );

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void Window::handleResizing(int width, int height) {
    if (height == 0)
        return;
            
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

void Window::handleError(int error, const char* description) {
    std::cerr << "GLFW ERROR: " << error << ": " << description << "\n";
}

void Window::windowResizeCallback(GLFWwindow* window, int width, int height) {
    Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->handleResizing(width, height);
    }
}

void Window::errorCallback(int error, const char* description) {
    handleError(error, description);
}
