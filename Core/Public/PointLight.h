#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
               GLfloat con, GLfloat lin, GLfloat exp);

    void Draw(const Frustum &frustum, Shader &shader, unsigned int &display, unsigned int &draw) override;

    ~PointLight();

public:
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;

private:
    int id = -1;

    // static point light count
    static unsigned int pointLightCount;

public:
    static unsigned int GetPointLightCount() { return pointLightCount; }
};

#endif // !__POINT_LIGHT_H__