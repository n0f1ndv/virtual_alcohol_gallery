#include "window.hpp"
#include "input.hpp"
#include "model.hpp"
#include "shader.hpp"

// TODO:
// * Clean up includes

Window::Window(float windowLength, float windowHeight, std::string windowTitle)
    : windowHeight{windowHeight}
    , windowLength{windowLength} {
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

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetErrorCallback(errorCallback);
}

Window::~Window() {
    glfwTerminate();
}

void Window::Loop() {
    Model cube;
    cube.Load("models/cone.obj");

    Shader shader = Shader("shaders/fragmentModel.glsl", "shaders/vertexModel.glsl");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawing goes here :D
        // TODO:
        // * Move drawing into its own function (or create class that manages scenes) (TOP PRIOTITY)
        // * Add camera
        glm::vec3 cameraPosition = glm::vec3(0.0f, 5.0f, -10.0f);
        glm::vec3 modelColor = glm::vec3(0.0f, 1.0f, 1.0f);
        glm::vec3 lightPosition = glm::vec3(2.0f, 4.0f, 2.0f);

        glm::mat4 P = glm::perspective(glm::radians(50.0f), windowLength / windowHeight, 1.0f, 50.0f);
        glm::mat4 V = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 M = glm::mat4(1.0f);
    
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "V"), 1, false, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "M"), 1, false, glm::value_ptr(M));

        glUniform3fv(glGetUniformLocation(shader.GetProgramID(), "uLightPos"), 1, glm::value_ptr(lightPosition));
        glUniform3fv(glGetUniformLocation(shader.GetProgramID(), "uViewPos"), 1, glm::value_ptr(cameraPosition));
        glUniform3fv(glGetUniformLocation(shader.GetProgramID(), "uColor"), 1, glm::value_ptr(modelColor));

        cube.Draw(shader.GetProgramID());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void errorCallback(int error, const char* description) {
    std::cerr << "GLFW ERROR: " << error << ": " << description << "\n";
}
