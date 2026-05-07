#include "shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(std::string fragmentShaderPath, std::string vertexShaderPath) {
    std::string code = ReadFile(vertexShaderPath);

    int success;
    char infoLog[512];

    // Vertex Shader compilation
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* cCode = code.c_str();
    glShaderSource(vertex, 1, &cCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR: Compiling vertex shader not successful\n";
    }

    // Fragment Shader compilation
    code = ReadFile(fragmentShaderPath);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    cCode = code.c_str();
    glShaderSource(fragment, 1 , &cCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR: Compiling fragment shader not successful\n";
    }

    // Linking shaders
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR: Linking shader not successful\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(program);
}

std::string Shader::ReadFile(std::string path) {
    std::string text;

    std::ifstream file;

    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(path.c_str());

        std::stringstream stream;

        stream << file.rdbuf();
        file.close();
        text = stream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR: Reading from file not successful\n";
    }

    return text;
}
