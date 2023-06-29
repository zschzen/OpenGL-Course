#include "../Public/Light.h"


Light::Light()
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
{
    color = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;
}

void Light::Use(GLuint ambientIntensityLocation, GLuint colorLocation)
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
}

Light::~Light()
{
}