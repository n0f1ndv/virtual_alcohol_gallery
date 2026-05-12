#include "window.hpp"
#include "model.hpp"
#include "collision.hpp"

Window::Window(float windowWidth, float windowHeight, std::string windowTitle)
    : windowHeight{windowHeight}
    , windowWidth{windowWidth}
    , aspectRatio{windowWidth / windowHeight} {
    if (!glfwInit()) {
        std::cerr << "ERROR: Failed to initialize GLFW\n";
    }

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
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
    ppShader = new Shader("shaders/fragmentPP.glsl", "shaders/vertexPP.glsl");
    postProcessing = new PostProcessing(windowWidth, windowHeight);
    camera = new Camera(window, shader->program, glm::vec3(0.0f, 0.0f, -10.0f));
    scene = new Scene(shader->program);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);

    glfwSetWindowSizeCallback(window, Window::windowResizeCallback);
    glfwSetErrorCallback(Window::errorCallback);
}

Window::~Window() {
    glfwTerminate();
}

void Window::Loop() {
    while (!glfwWindowShouldClose(window)) {
        Clock();

        postProcessing->Bind();

        shader->Use();
    
        camera->Update(aspectRatio, frameTime);

        scene->DrawModels();
        scene->DrawLights();

        postProcessing->BindDefault();

        ppShader->Use();

        postProcessing->Draw();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void Window::Clock() {
    frameTime = (float)glfwGetTime();

    totalTime += frameTime;
    delta = frameTime - lastTime;
    lastTime = frameTime;

    glUniform1f(glGetUniformLocation(ppShader->program, "time"), totalTime);
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
