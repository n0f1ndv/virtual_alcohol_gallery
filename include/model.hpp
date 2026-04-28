#pragma once

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
    GLuint texture;

    Mesh();
    Mesh(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    ~Mesh();

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& other) noexcept;

    void Setup();
    void Draw(GLuint program);
    void CleanUp();

private:
    GLuint VAO, VBO, EBO;
};

class Model {
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void ProcessNode(struct aiNode* node, const struct aiScene* scene);
    Mesh ProcessMesh(struct aiMesh* mesh, const struct aiScene* scene);

public:
    Model();
    ~Model();

    void Load(const std::string& path);
    void Draw(GLuint program);
};
