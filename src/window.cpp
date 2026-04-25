#include "window.hpp"
#include "input.hpp"
#include "model.hpp"
#include "shader.hpp"

Window::Window(int window_height, int window_length, std::string window_title)
    : m_window_height{window_height}
    , m_window_length{window_length} {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
    }

    m_window = glfwCreateWindow(m_window_height, m_window_length, window_title.c_str(), NULL, NULL);
    if (!m_window) {
        std::cout << "Failed to create window\n";

        glfwTerminate();
    }

    glfwMakeContextCurrent(m_window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW\n";
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCursorPosCallback(m_window, cursor_position_callback);
}

Window::~Window() {
    glfwTerminate();
}

void Window::Loop() {
    Model cube;
    cube.Load("models/cube.obj");

    Shader shader = Shader("shaders/fragment_model.glsl", "shaders/vertex_model.glsl");

    while (true) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawing goes here :D (TODO: Move drawing to separate method/function idk)
        // TODO: Add camera
        glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -10.0f);
        glm::vec3 modelColor = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 lightPosition = glm::vec3(2.0f, 4.0f, 2.0f);

        glm::mat4 P = glm::perspective(glm::radians(50.0f), 1280.0f / 720.0f, 1.0f, 50.0f);
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

        glfwSwapBuffers(m_window);

        glfwPollEvents();
    }
}