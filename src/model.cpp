#include "model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "lodepng.h"

#include <iostream>
#include <cstring>

// ---------- Mesh implementation ----------
Mesh::Mesh() : texture(0), VAO(0), VBO(0), EBO(0) {}
Mesh::~Mesh() { Dispose(); }

void Mesh::Setup() {
    if (VAO) return;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // texcoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

// Move constructor
Mesh::Mesh(Mesh&& other) noexcept
    : vertices(std::move(other.vertices)), 
      indices(std::move(other.indices)), 
      texture(other.texture),
      VAO(other.VAO), VBO(other.VBO), EBO(other.EBO) 
{
    // Nullify the moved-from object's resources so its destructor doesn't delete them
    other.texture = 0;
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

// Move assignment
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        Dispose(); // Clean up current resources before taking new ones

        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        texture = other.texture;
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        other.texture = 0;
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }
    return *this;
}

void Mesh::Draw(GLuint program) {
    if (!VAO) Setup();
    GLint loc = glGetUniformLocation(program, "uTexture");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (loc >= 0) glUniform1i(loc, 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Dispose() {
    if (VAO) { glDeleteVertexArrays(1, &VAO); VAO = 0; }
    if (VBO) { glDeleteBuffers(1, &VBO); VBO = 0; }
    if (EBO) { glDeleteBuffers(1, &EBO); EBO = 0; }
    if (texture) { glDeleteTextures(1, &texture); texture = 0; }
}

// ---------- Simple PNG loader (lodepng) ----------
static GLuint LoadTexturePNG(const std::string& path) {
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, path);
    if (error) {
        std::cerr << "lodepng decode error (" << error << "): " << path << "\n";
        return 0;
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return tex;
}

// ---------- Model implementation ----------
Model::Model() {}
Model::~Model() {
    for (auto &m : meshes) m.Dispose();
}

void Model::Load(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_FlipUVs
    );
    if (!scene || !scene->mRootNode) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << "\n";
        return;
    }
    // directory for relative textures
    size_t p = path.find_last_of("/\\");
    directory = (p == std::string::npos) ? std::string() : path.substr(0, p+1);

    ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(GLuint program) {
    for (auto &m : meshes) m.Draw(program);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* amesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(amesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* amesh, const aiScene* scene) {
    Mesh mesh;
    mesh.vertices.reserve(amesh->mNumVertices);
    for (unsigned int i = 0; i < amesh->mNumVertices; ++i) {
        Vertex v{};
        v.Position[0] = amesh->mVertices[i].x;
        v.Position[1] = amesh->mVertices[i].y;
        v.Position[2] = amesh->mVertices[i].z;
        if (amesh->HasNormals()) {
            v.Normal[0] = amesh->mNormals[i].x;
            v.Normal[1] = amesh->mNormals[i].y;
            v.Normal[2] = amesh->mNormals[i].z;
        } else {
            v.Normal[0] = v.Normal[1] = v.Normal[2] = 0.0f;
        }
        if (amesh->mTextureCoords[0]) {
            v.TexCoords[0] = amesh->mTextureCoords[0][i].x;
            v.TexCoords[1] = amesh->mTextureCoords[0][i].y;
        } else {
            v.TexCoords[0] = v.TexCoords[1] = 0.0f;
        }
        mesh.vertices.push_back(v);
    }

    for (unsigned int f = 0; f < amesh->mNumFaces; ++f) {
        aiFace face = amesh->mFaces[f];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            mesh.indices.push_back(face.mIndices[j]);
    }

    // material diffuse texture (first) -> expects PNG; adjust loader for other formats
    if (amesh->mMaterialIndex >= 0 && scene->mMaterials) {
        aiMaterial* material = scene->mMaterials[amesh->mMaterialIndex];
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            std::string texPath = directory + std::string(str.C_Str());
            mesh.texture = LoadTexturePNG(texPath);
        }
    }

    mesh.Setup();
    return mesh;
}
