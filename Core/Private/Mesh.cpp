#include "../Public/Mesh.h"

#include "../Public/MeshData.h"

Mesh::Mesh() {}

Mesh::Mesh(MeshData meshData)
{
    UpdateMesh(meshData.vertices.data(), meshData.indices.data(), 
               static_cast<unsigned int>(meshData.vertices.size()), 
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

    glGenBuffers(1, &subMesh.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &subMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 3, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    subMeshes.push_back(subMesh);
}

void Mesh::RenderMesh()
{
    for (auto& subMesh : subMeshes)
    {
        glBindVertexArray(subMesh.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.IBO);
        glDrawElements(GL_TRIANGLES, subMesh.indexCount, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void Mesh::ClearMesh()
{
    for (auto& subMesh : subMeshes)
    {
        glDeleteBuffers(1, &subMesh.IBO);
        glDeleteBuffers(1, &subMesh.VBO);
        glDeleteVertexArrays(1, &subMesh.VAO);
    }

    subMeshes.clear();
}

Mesh::~Mesh()
{
    ClearMesh();
}
