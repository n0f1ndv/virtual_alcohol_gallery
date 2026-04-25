#include "shader.hpp"

Shader::Shader(std::string fragment_shader_path, std::string vertex_shader_path) {
    // Reading files should have its own function imo
    std::string fragment_shader_code;
    std::string vertex_shader_code;

    std::ifstream fragment_shader_file;
    std::ifstream vertex_shader_file;

    vertex_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        fragment_shader_file.open(fragment_shader_path.c_str());
        vertex_shader_file.open(vertex_shader_path.c_str());

        std::stringstream fragment_shader_stream;
        std::stringstream vertex_shader_stream;

        fragment_shader_stream << fragment_shader_file.rdbuf();   
        vertex_shader_stream << vertex_shader_file.rdbuf();

        fragment_shader_file.close();
        vertex_shader_file.close();

        fragment_shader_code = fragment_shader_stream.str();
        vertex_shader_code = vertex_shader_stream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error occured while reading from file\n";
    }

    int success;
    char info_log[512];

    // Vertex Shader compilation
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* v_code = vertex_shader_code.c_str();
    glShaderSource(vertex, 1, &v_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cout << "Error occured while compiling vertex shader\n";
    }

    // Vertex Shader compilation
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* f_code = fragment_shader_code.c_str();
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
