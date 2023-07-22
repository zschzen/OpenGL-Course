#ifndef __MODEL_H__
#define __MODEL_H__

#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <string.h>
#include <vector>

#include "Mesh.h"

// Forward declarations
class Shader;
struct Vertex;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"
#include "Behaviour.h"
#include "BoundingVolume.h"

// Forward declaration
class Shader;

class Model : public Vosgi::Behaviour
{
public:
    Model();
    explicit Model(const char* path);
    Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    ~Model() override;

    void Render(Shader& shader, unsigned int& draw);
    void Clear();

    void Draw(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw) override;

    const std::vector<Mesh*>& GetMeshes() const { return meshes; }

    Vosgi::AABB* GetWorldAABB();

private:
    std::vector<Mesh*> meshes = std::vector<Mesh*>();
    std::vector<Texture> texturesLoaded = std::vector<Texture>(); 
    std::string directory = std::string();   
    std::unique_ptr<Vosgi::AABB> aabb;

    void LoadModel(const std::string& fileName);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};

#endif // !__MODEL_H__