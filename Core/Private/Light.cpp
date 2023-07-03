#include "../Public/Light.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Public/Shader.h"

Light::Light()
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
    GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
    color = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;

    direction = glm::vec3(xDir, yDir, zDir);
    diffuseIntensity = dIntensity;
}

void Light::Use(Shader& shader)
{
    shader.SetVec3("directionalLight.colour", color);
    shader.SetFloat("directionalLight.ambientIntensity", ambientIntensity);

    shader.SetVec3("directionalLight.direction", direction);
    shader.SetFloat("directionalLight.diffuseIntensity", diffuseIntensity);
}

Light::~Light()
{
}