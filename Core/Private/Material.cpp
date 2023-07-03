#include "../Public/Material.h"

#include "../Public/Shader.h"

Material::Material()
{
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
    specularIntensity = sIntensity;
    shininess = shine;
}

void Material::Use(Shader& shader)
{
    shader.SetFloat("material.specularIntensity", specularIntensity);
    shader.SetFloat("material.shininess", shininess);
}

Material::~Material()
{
}