#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
    private:
        unsigned int program_id;

    public:
        Shader(std::string fragment_shader_path, std::string vertex_shader_path);

        void Use();
        unsigned int GetProgramID();
};
