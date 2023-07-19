#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

    void Draw(const Frustum &frustum, Shader &shader, unsigned int &display, unsigned int &draw) override;

    ~DirectionalLight();
};

#endif // !__DIRECTIONAL_LIGHT_H__