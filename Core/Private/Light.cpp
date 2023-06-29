#include "../Public/Light.h"


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

void Light::Use(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint diffuseDirectionLocation)
{
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);

    glUniform3f(diffuseDirectionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light()
{
}