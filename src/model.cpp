#include "model.hpp"

Model::Model(std::string model_path) {
    Assimp::Importer importer;

    model = importer.ReadFile(
        model_path,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType
    );

    if (nullptr == model) {
        std::cout << "Error importing model\n";
    }
}