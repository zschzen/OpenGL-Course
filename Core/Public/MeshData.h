#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <utility>

#include "Texture.h"

struct Vertex
{
    glm::vec3 position{};
    glm::vec3 normal{};
    glm::vec2 texCoords{};

    // Constructor
    Vertex() = default;

    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex)
    {
        position = pos;
        normal = norm;
        texCoords = tex;
    }
};

struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    GLuint VAO{}, VBO{}, IBO{};

    // Constructor
    MeshData() = default;

    MeshData(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> texs)
    {
        vertices = std::move(verts);
        indices = std::move(inds);
        textures = std::move(texs);
    }
};
