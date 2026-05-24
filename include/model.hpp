#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
public:
    Model();
    ~Model();

    void Load(const std::string& path);
    void Draw(GLuint program, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color);
    void DrawMatrix(GLuint program, glm::mat4 modelMatrix, glm::vec3 color);
    
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void ProcessNode(struct aiNode* node, const struct aiScene* scene);
    Mesh ProcessMesh(struct aiMesh* mesh, const struct aiScene* scene);
};
