#include "../Public/DirectionalLight.h"

#include "../Public/Shader.h"

DirectionalLight::DirectionalLight() : Light()
{
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
    : Light(red, green, blue, aIntensity, dIntensity)
{
}

void DirectionalLight::Draw(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw)
{
    //shader.SetInt("directionalLight.base.enabled", enabled ? 1 : 0);

    shader.SetVec3("directionalLight.base.colour", color);
    shader.SetFloat("directionalLight.base.ambientIntensity", ambientIntensity);

    shader.SetFloat("directionalLight.base.diffuseIntensity", diffuseIntensity);
    shader.SetVec3("directionalLight.direction", transform->GetForward());
}

DirectionalLight::~DirectionalLight()
{
}