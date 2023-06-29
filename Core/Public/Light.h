#ifndef __LIGHT_H__
#define __LIGHT_H__

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

    void Use(GLuint ambientIntensityLocation, GLuint colorLocation);

    ~Light();

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    GLfloat ambientIntensity = 1.0f;

};

#endif // !__LIGHT_H__