#include "../Public/Mesh.h"

#include "../Public/MeshData.h"

Mesh::Mesh() {}

Mesh::Mesh(MeshData meshData)
{
    UpdateMesh(meshData.vertices.data(), meshData.indices.data(), 
               static_cast<unsigned int>(meshData.vertices.size() / 3), 
               static_cast<unsigned int>(meshData.indices.size()));
}

Mesh::Mesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
    UpdateMesh(vertices, indices, numOfVertices, numOfIndices);
}

void Mesh::UpdateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
    subMeshes.clear();

    SubMesh subMesh;
    subMesh.Data.vertices.insert(subMesh.Data.vertices.end(), vertices, vertices + (numOfVertices * 3));
    subMesh.Data.indices.insert(subMesh.Data.indices.end(), indices, indices + numOfIndices);
    subMesh.indexCount = numOfIndices;

    glGenVertexArrays(1, &subMesh.VAO);
    glBindVertexArray(subMesh.VAO);

    glGenBuffers(1, &subMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, subMesh.Data.vertices.size() * sizeof(GLfloat), &subMesh.Data.vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &subMesh.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh.Data.indices.size() * sizeof(unsigned int), &subMesh.Data.indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    subMeshes.push_back(subMesh);
}

void Mesh::RenderMesh()
{
    for (auto& subMesh : subMeshes)
    {
        glBindVertexArray(subMesh.VAO);
        glDrawElements(GL_TRIANGLES, subMesh.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Mesh::ClearMesh()
{
    for (auto& subMesh : subMeshes)
    {
        glDeleteVertexArrays(1, &subMesh.VAO);
        glDeleteBuffers(1, &subMesh.VBO);
        glDeleteBuffers(1, &subMesh.IBO);
    }

    subMeshes.clear();
}

Mesh::~Mesh()
{
    ClearMesh();
}
