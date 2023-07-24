#pragma once

#include <cstring>
#include <vector>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshData.h"
#include "Texture.h"

// Forward declaration
class Shader;

class Mesh
{
public:
    Mesh();
    Mesh(SubMesh subMesh);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Update(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void ActivateTextures(Shader& shader);
    void Draw(Shader& shader);
    void Clear();

    // Get vertices, indices and textures
    const std::vector<Vertex>& GetVertices() const { return meshFilter.vertices; }
    const std::vector<unsigned int>& GetIndices() const { return meshFilter.indices; }
    const std::vector<Texture>& GetTextures() const { return meshFilter.textures; }

    ~Mesh();

protected:
    SubMesh meshFilter = SubMesh();
};