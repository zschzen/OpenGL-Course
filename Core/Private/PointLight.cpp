#include "../Public/PointLight.h"

#include "../Public/Shader.h"

unsigned int PointLight::pointLightCount = 0;

PointLight::PointLight() : Light()
{
    constant = 1.0f;
    linear = 0.0f;
    quadratic = 0.0f;

    id = pointLightCount++;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
                       GLfloat con, GLfloat lin, GLfloat exp)
    : Light(red, green, blue, aIntensity, dIntensity)
{
    constant = con;
    linear = lin;
    quadratic = exp;

    id = pointLightCount++;
}

void PointLight::Draw(const Frustum &frustum, Shader &shader, unsigned int &display, unsigned int &draw)
{
    char buffer[100] = {'\0'};
    snprintf(buffer, sizeof(buffer), "pointLights[%d].", id);
    std::string str(buffer);

    //shader.SetInt((str + "base.enabled").c_str(), enabled ? 1 : 0);
    shader.SetInt("pointLightCount", pointLightCount);

    shader.SetVec3((str + "base.colour").c_str(), color);
    shader.SetFloat((str + "base.ambientIntensity").c_str(), ambientIntensity);
    shader.SetFloat((str + "base.diffuseIntensity").c_str(), diffuseIntensity);

    shader.SetVec3((str + "position").c_str(), transform->GetModel()[3]);
    shader.SetFloat((str + "constant").c_str(), constant);
    shader.SetFloat((str + "linear").c_str(), linear);
    shader.SetFloat((str + "quadratic").c_str(), quadratic);
}

void PointLight::DrawInspector()
{
    ImGui::ColorEdit3("Color", &color[0]);
    ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Diffuse Intensity", &diffuseIntensity, 0.0f, 1.0f);

    ImGui::SliderFloat("Constant", &constant, 0.0f, 1.0f);
    ImGui::SliderFloat("Linear", &linear, 0.0f, 1.0f);
    ImGui::SliderFloat("Quadratic", &quadratic, 0.0f, 1.0f);
}

PointLight::~PointLight()
{
    pointLightCount--;
}