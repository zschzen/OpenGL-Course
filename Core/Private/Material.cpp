#include "../Public/Material.h"

Material::Material()
{
}

Material::Material(GLfloat sIntensity, GLfloat shine) : specularIntensity(sIntensity), shininess(shine)
{
}

void Material::Use(GLuint specularIntensityLocation, GLuint shininessLocation)
{
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}

Material::~Material()
{
}