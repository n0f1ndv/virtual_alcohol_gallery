#include "window.hpp"
#include "model.hpp"
#include "collision.hpp"

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

    shader = new Shader("shaders/fragmentModel.glsl", "shaders/vertexModel.glsl");
    camera = new Camera(window, shader->program, glm::vec3(0.0f, 0.0f, -10.0f));
    scene = new Scene(shader->program);

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
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Use();
    
        camera->Update(aspectRatio);

        scene->DrawModels();
        scene->DrawLights();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void Window::handleResizing(int width, int height) {
    if (height == 0) return;
            
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

void Window::handleError(int error, const char* description) {
    std::cerr << "ERROR: " << error << ": " << description << "\n";
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
