#include "../Public/Mesh.h"

#include "../Public/MeshData.h"

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    Update(vertices, indices, textures);
}

void Mesh::Update(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    SubMesh subMesh = SubMesh(vertices, indices, textures);

    glGenVertexArrays(1, &subMesh.VAO);
    glBindVertexArray(subMesh.VAO);

    glGenBuffers(1, &subMesh.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &subMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    meshFilter = subMesh;
}

void Mesh::Draw()
{
    glBindVertexArray(meshFilter.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshFilter.IBO);

    glDrawElements(GL_TRIANGLES, meshFilter.indices.size(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Clear()
{
    if (meshFilter.IBO != 0)
    {
        glDeleteBuffers(1, &meshFilter.IBO);
        meshFilter.IBO = 0;
    }

    if (meshFilter.VBO != 0)
    {
        glDeleteBuffers(1, &meshFilter.VBO);
        meshFilter.VBO = 0;
    }

    if (meshFilter.VAO != 0)
    {
        glDeleteVertexArrays(1, &meshFilter.VAO);
        meshFilter.VAO = 0;
    }

    meshFilter.indices.clear();
    meshFilter.vertices.clear();
    meshFilter.textures.clear();
    meshFilter = SubMesh();
}

Mesh::~Mesh()
{
    Clear();
}
