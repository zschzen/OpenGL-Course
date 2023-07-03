#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#pragma once

#include <GL/glew.h>

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
    GLfloat specularIntensity = 0.0f;
    GLfloat shininess = 0.0f;

};

#endif