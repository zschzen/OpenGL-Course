#pragma once

#include <string.h>
#include <vector>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshData.h"

class Mesh
{
public:
    Mesh();
    Mesh(MeshData meshData);
    Mesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);

    void UpdateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void RenderMesh();
    void ClearMesh();

    ~Mesh();

protected:
    std::vector<SubMesh> subMeshes = std::vector<SubMesh>();
};