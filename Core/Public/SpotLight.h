#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
    SpotLight();
    SpotLight(float red, float green, float blue, float aIntensity, float dIntensity, float con, float lin, float exp, float edg);

    void OnDisable() override;
    void Draw(const Frustum &frustum, Shader &shader, unsigned int &display, unsigned int &draw) override;
    void DrawInspector() override;

    ~SpotLight();

public:
    float edge;
    float procEdge;

private:
    int id = -1;

    // static spot light count
    static unsigned int spotLightCount;

public:
    static unsigned int GetSpotLightCount() { return spotLightCount; }

};

#endif // __SPOTLIGHT_H__