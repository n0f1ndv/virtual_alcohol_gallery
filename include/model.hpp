#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void SetupMesh();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, 
         std::vector<Texture> textures);

    void Draw(unsigned int shaderProgram) const;

    ~Mesh();
};

class Model {
private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::string name;
    std::vector<std::string> loadedTexturePaths;

    void ProcessNode(aiNode* node, const aiScene* scene);

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, 
                                              const std::string& typeName);

    unsigned int LoadTextureFromFile(const std::string& path);

    bool IsTextureLoaded(const std::string& path) const;

public:
    Model();

    ~Model();

    bool LoadModel(const std::string& path);

    void Draw(unsigned int shaderProgram) const;

    size_t GetMeshCount() const { return meshes.size(); }

    const Mesh* GetMesh(size_t index) const;

    const std::string& GetDirectory() const { return directory; }

    const std::string& GetName() const { return name; }

    void Clear();
};
