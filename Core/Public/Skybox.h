#ifndef __SKYBOX_H
#define __SKYBOX_H

#pragma once

#include <vector>
#include <cstring>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

class Skybox
{
public:
    Skybox();

    Skybox(std::vector<std::string> faceLocations);

    void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    ~Skybox();

private:
    Mesh* skyMesh;
    Shader* skyShader;

    GLuint textureId;
    GLuint uniformProjection, uniformView;
};


#endif //__SKYBOX_H
