#ifndef __LIGHT_H__
#define __LIGHT_H__

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

// Forward declaration
class Shader;

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
          GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

    void Use(Shader& shader);

    ~Light();

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    GLfloat ambientIntensity = 1.0f;

    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
    GLfloat diffuseIntensity = 1.0f;
};

#endif // !__LIGHT_H__