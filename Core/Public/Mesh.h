#pragma once

#include <cstring>
#include <vector>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshData.h"

// Forward declaration
class Shader;

class Mesh
{
public:
    Mesh() = default;
    Mesh(MeshData meshData);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    ~Mesh();

    void Update(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void ActivateTextures(Shader& shader);
    void Draw(Shader& shader);
    void RenderMesh();
    void Clear();

    // Get vertices, indices and textures
    const std::vector<Vertex>& GetVertices() const { return meshFilter.vertices; }
    const std::vector<unsigned int>& GetIndices() const { return meshFilter.indices; }
    const std::vector<Texture>& GetTextures() const { return meshFilter.textures; }

protected:
    MeshData meshFilter = MeshData();
};