#include "window.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"

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

//--kolizja--
bool CheckCollision(const BoundingBox& a, const BoundingBox& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

BoundingBox TransformBox(BoundingBox base, glm::vec3 pos, glm::vec3 scale) {
    BoundingBox transformed;
    transformed.min = (base.min * scale) + pos;
    transformed.max = (base.max * scale) + pos;
    return transformed;
}

BoundingBox getPlayerBox(glm::vec3 pos) {
    float size = 1.0f;
    return { pos - glm::vec3(size, 1.0f, size), pos + glm::vec3(size, 1.0f, size) };
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

    Model wine;
    wine.Load("models/wine.obj");

    std::vector<BoundingBox> colliders;
    
    colliders.push_back(TransformBox(cube.baseBox, 
        glm::vec3(4.0f, -1.5f, 2.0f), 
        glm::vec3(0.5f, 1.25f, 0.5f) 
    ));

    colliders.push_back(TransformBox(cube.baseBox, 
        glm::vec3(4.0f, -1.5f, -2.0f), 
        glm::vec3(0.5f, 1.25f, 0.5f)
    ));

    float var;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        glm::vec3 flatFront;
        if (camera.fly) {flatFront = glm::normalize(glm::vec3(camera.front.x, 0.0f, camera.front.z));}
        else {flatFront = glm::normalize(glm::vec3(camera.front.x, camera.front.y, camera.front.z));}
        
        glm::vec3 right = glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 velocity = (flatFront * camera.speed_z) - (right * camera.speed_y);
        
        glm::vec3 nextPosition = camera.position + (velocity * (float)glfwGetTime());
        BoundingBox nextPlayerBox = getPlayerBox(nextPosition);

        bool collision = false;
            for (const auto& box : colliders) {
                if (CheckCollision(nextPlayerBox, box)) {
                    collision = true;
                    break;
                }
            }
        
        if (!collision || !camera.fly) {
                camera.position = nextPosition; 
            } 

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

        floor.Draw(shader.GetProgramID(), 
            glm::vec3(0.0f, -3.0f, 0.0f),   // position
            glm::vec3(0.0f),                // rotation
            glm::vec3(20.0f, 0.1f, 20.0f),  // scale
            glm::vec3(0.2f,0.2f,0.2f)       // color
        ); 

        cube.Draw(shader.GetProgramID(), 
            glm::vec3(4.0f, -1.5f, 2.0f),
            glm::vec3(0.0f),
            glm::vec3(0.5f, 1.25f, 0.5f),
            glm::vec3(0.8f,0.7f,0.4f)
        ); 

        wine.Draw(shader.GetProgramID(), 
            glm::vec3(4.0f, -0.25f, 2.0f),
            glm::vec3(-90.0f, 0.0f, 0.0f),
            glm::vec3(0.06f, 0.06f, 0.06f),
            glm::vec3(0.0f,0.5f,0.5f)
        ); 

        cube.Draw(shader.GetProgramID(), 
            glm::vec3(4.0f, -1.5f, -2.0f),
            glm::vec3(0.0f),
            glm::vec3(0.5f, 1.25f, 0.5f),
            glm::vec3(0.2f,0.2f,0.2f)
        ); 

        wine.Draw(shader.GetProgramID(), 
            glm::vec3(4.0f, -0.25f, -2.0f),
            glm::vec3(-90.0f, 0.0f, 0.0f),
            glm::vec3(0.06f, 0.06f, 0.06f),
            glm::vec3(1.0f,0.0f,0.0f)
        );

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void errorCallback(int error, const char* description) {
    std::cerr << "GLFW ERROR: " << error << ": " << description << "\n";
}
