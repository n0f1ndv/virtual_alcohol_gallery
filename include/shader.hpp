#pragma once

#include <string>

class Shader {
public:
    Shader(std::string fragmentShaderPath, std::string vertexShaderPath);

    void Use();
    unsigned int GetProgramID();
    
private:
    unsigned int program;

    std::string ReadFile(std::string path);
};
