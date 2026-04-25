#pragma once

// TODO:
// * Review this code (TOP PRIORITY)

#include <GL/glew.h>
#include <string>
#include <vector>

struct Vertex {
    float Position[3];
    float Normal[3];
    float TexCoords[2];
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GLuint texture; // 0 if none

    Mesh();
    ~Mesh();

    // --- Prevent copying ---
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // --- Enable moving ---
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // prepare GL buffers (creates VAO/VBO/EBO)
    void Setup();
    // draw mesh with given shader program (expects sampler uTexture)
    void Draw(GLuint program);
    // free GL resources
    void Dispose();

private:
    GLuint VAO, VBO, EBO;
};

class Model {
public:
    Model();
    ~Model();

    // Load an OBJ (Assimp) from path (relative textures expected)
    void Load(const std::string& path);

    // Draw all meshes using program
    void Draw(GLuint program);

private:
    std::vector<Mesh> meshes;
    std::string directory;

    // internal
    void ProcessNode(struct aiNode* node, const struct aiScene* scene);
    Mesh ProcessMesh(struct aiMesh* mesh, const struct aiScene* scene);
};
