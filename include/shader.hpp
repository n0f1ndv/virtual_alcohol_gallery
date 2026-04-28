#pragma once

#include <string>

class Shader {
    private:
        unsigned int program_id;

        std::string ReadFile(std::string path);

    public:
        Shader(std::string fragment_shader_path, std::string vertex_shader_path);

        void Use();
        unsigned int GetProgramID();
};
