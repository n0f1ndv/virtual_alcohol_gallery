#include "shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(std::string fragment_shader_path, std::string vertex_shader_path) {
    std::string code = ReadFile(vertex_shader_path);

    int success;
    char info_log[512];

    // Vertex Shader compilation
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* v_code = code.c_str();
    glShaderSource(vertex, 1, &v_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cout << "Error occured while compiling vertex shader\n";
    }

    // Fragment Shader compilation
    code = ReadFile(fragment_shader_path);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* f_code = code.c_str();
    glShaderSource(fragment, 1 , &f_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cout << "Error occured while compiling fragment shader\n";
    }

    program_id = glCreateProgram();
    glAttachShader(program_id, vertex);
    glAttachShader(program_id, fragment);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        std::cout << "Error occured while linking shader program\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(program_id);
}

unsigned int Shader::GetProgramID() {
    return program_id;
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
        std::cout << "Error occured while reading from file\n";
    }

    return text;
}
