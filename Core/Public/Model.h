#pragma once

#include <string.h>
#include <vector>

#include "Mesh.h"

// Forward declarations
class Shader;
struct Vertex;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb/stb_image.h"

#include "Mesh.h"
#include "Texture.h"

// Forward declaration
class Shader;

class Model
{
public:
    Model();
    Model(const char* path);
    ~Model();

    void Render(Shader& shader);
    void Clear();

    const std::vector<Mesh*>& GetMeshes() const { return meshes; }

private:
    std::vector<Mesh*> meshes = std::vector<Mesh*>();
    std::vector<Texture> texturesLoaded = std::vector<Texture>(); 
    std::string directory = std::string();   

    void LoadModel(const std::string& fileName);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};