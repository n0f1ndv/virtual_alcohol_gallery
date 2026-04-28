#include "window.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "renderer.hpp"

#include <cmath>

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

    glClearColor(1.000f, 0.780f, 0.918f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetErrorCallback(errorCallback);
}

Window::~Window() {
    glfwTerminate();
}

void Window::Loop() { 
    Camera camera(window, glm::vec3(0.0f, 0.0f, -10.0f));


    Shader shader = Shader("shaders/fragmentModel.glsl", "shaders/vertexModel.glsl");
    glfwSetCursorPos(window, 1280.0f / 2, 720.0f / 2);
    Model cube;
    cube.Load("models/cube.obj");

    //do podłogi
    Model floor;
    floor.Load("models/cube.obj");


    float var;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        glm::vec3 flatFront;
        if (camera.fly) {flatFront = glm::normalize(glm::vec3(camera.front.x, 0.0f, camera.front.z));}
        else {flatFront = glm::normalize(glm::vec3(camera.front.x, camera.front.y, camera.front.z));}
        glm::vec3 right = glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 velocity = (flatFront * camera.speed_z) - (right * camera.speed_y);
        camera.position += velocity * (float)glfwGetTime(); 

        var += M_PI * glfwGetTime();
        glfwSetTime(0);

        // Drawing goes here :D
        // TODO:
        // * Move drawing into its own function (or create class that manages scenes) (TOP PRIOTITY)
        // * Implement camera (TOP PRIORITY)
        // * Implement better way to place models
        glm::vec3 modelColor = glm::vec3(0.792f, 0.929f, 1.000f);
        glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, -4.0f);

        glm::mat4 P = glm::perspective(glm::radians(50.0f), windowLength / windowHeight, 1.0f, 50.0f);
        // glm::mat4 V = glm::lookAt(camera.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 V = glm::lookAt(camera.position, camera.position + camera.front, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 M = glm::mat4(1.0f);

        M = glm::scale(M, glm::vec3(0.001f,0.001f,0.001f));
        M = glm::rotate(M, glm::radians(20 * var), glm::vec3(1.0f, 1.0f, 0.0f));
    
        shader.Use();
        // TODO:
        // * Create helper functions to setting uniform values (It's completely optional ;*)
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "V"), 1, false, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "M"), 1, false, glm::value_ptr(M));

        glUniform3fv(glGetUniformLocation(shader.GetProgramID(), "uLightPos"), 1, glm::value_ptr(lightPosition));
        glUniform3fv(glGetUniformLocation(shader.GetProgramID(), "uViewPos"), 1, glm::value_ptr(camera.position));
        glUniform3fv(glGetUniformLocation(shader.GetProgramID(), "uColor"), 1, glm::value_ptr(modelColor));

        cube.Draw(shader.GetProgramID());

        //szybka podłowa w clankerze do wyrzucenia potem

        DrawEntity(shader, floor, 
            glm::vec3(0.0f, -3.0f, 0.0f), //position
            glm::vec3(0.0f), //rotation
            glm::vec3(20.0f, 0.1f, 20.0f)); //scale

        //test
        DrawEntity(shader, cube, 
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f),
            glm::vec3(2.0f, 2.0f, 2.0f));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void errorCallback(int error, const char* description) {
    std::cerr << "GLFW ERROR: " << error << ": " << description << "\n";
}
