#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct SubMesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    GLuint VAO, VBO, IBO;

    // Constructor
    SubMesh() {}

    SubMesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> texs)
    {
        vertices = verts;
        indices = inds;
        textures = texs;
    }
};
