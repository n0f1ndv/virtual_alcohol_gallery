#pragma once

#include <GL/glew.h>

#include <string>

class Shader {
public:
    GLuint program;

    Shader(std::string fragmentShaderPath, std::string vertexShaderPath);

    void Use();
    
private:
    std::string ReadFile(std::string path);
};
