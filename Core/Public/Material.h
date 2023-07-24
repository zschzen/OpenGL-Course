#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>

// Forward declaration
class Shader;

class Material
{
public:
    Material();
    Material(GLfloat sIntensity, GLfloat shine);
    
    void Use(Shader& shader);
    
    ~Material();

public:
    glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    GLfloat specularIntensity = 0.5f;
    GLfloat shininess = 32.0f;

};

#endif