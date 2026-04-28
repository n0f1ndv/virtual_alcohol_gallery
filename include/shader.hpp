#pragma once

#include <string>

class Shader {
    private:
        unsigned int program;

        std::string ReadFile(std::string path);

    public:
        Shader(std::string fragmentShaderPath, std::string vertexShaderPath);

        void Use();
        unsigned int GetProgramID();
};
