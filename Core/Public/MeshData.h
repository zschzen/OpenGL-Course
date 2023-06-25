#pragma once

#include <GL/glew.h>

struct MeshData
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;
};

struct SubMesh
{
    MeshData Data;
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};
