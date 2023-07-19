#ifndef __LIGHT_H__
#define __LIGHT_H__

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Behaviour.h"

// Forward declaration
class Shader;
struct Sphere;

class Light : public Vosgi::Behaviour
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

    ~Light();

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    GLfloat ambientIntensity = 1.0f;
    GLfloat diffuseIntensity = 1.0f;
};

#endif // !__LIGHT_H__