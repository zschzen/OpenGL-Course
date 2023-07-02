#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#pragma once

#include <GL/glew.h>

class Material
{
public:
    Material();
    Material(GLfloat sIntensity, GLfloat shine);
    
    void Use(GLuint specularIntensityLocation, GLuint shininessLocation);
    
    ~Material();

public:
    GLfloat specularIntensity = 0.0f;
    GLfloat shininess = 0.0f;

};

#endif