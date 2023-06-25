#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Forward declarations
struct MeshData;

class ModelLoader
{
public:
    static MeshData LoadModel(const std::string& fileName);

private:
    static void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& modelData);
};
