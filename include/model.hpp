#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>

class Model {
    private:
        const aiScene* model;

    public:
        Model(std::string model_path);
};
