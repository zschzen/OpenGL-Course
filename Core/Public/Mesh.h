#pragma once

#include <string.h>
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
    void Draw(Shader& shader);
    void Clear();

    ~Mesh();

protected:
    SubMesh meshFilter = SubMesh();
};