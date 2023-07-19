#include "../Public/Light.h"

#include <glm/gtc/type_ptr.hpp>

Light::Light() : Behaviour()
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) : Behaviour()
{
    color = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
}

Light::~Light()
{
}