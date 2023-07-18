#include "../Public/Light.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Public/Frustum.h"
#include "../Public/Shader.h"

Light::Light() : Behaviour()
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) : Behaviour()
{
    color = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
}

void Light::Draw(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw)
{
    shader.SetVec3("directionalLight.colour", color);
    shader.SetFloat("directionalLight.ambientIntensity", ambientIntensity);

    shader.SetVec3("directionalLight.direction", transform->rotation);
    shader.SetFloat("directionalLight.diffuseIntensity", diffuseIntensity);
}

Light::~Light()
{
}