#include "../Public/SpotLight.h"

#include "../Public/Shader.h"

unsigned int SpotLight::spotLightCount = 0;

SpotLight::SpotLight() : PointLight()
{
    constant = 1.0f;
    linear = 0.0f;
    quadratic = 0.0f;

    edge = 0.0f;
    procEdge = 0.0f;

    id = spotLightCount++;
}

SpotLight::SpotLight(float red, float green, float blue, float aIntensity, float dIntensity, float con, float lin,
                     float exp, float edg)
    : PointLight(red, green, blue, aIntensity, dIntensity, con, lin, exp)
{
    constant = con;
    linear = lin;
    quadratic = exp;

    edge = edg;
    procEdge = cosf(glm::radians(edge));

    id = spotLightCount++;
}

void SpotLight::Draw(const Frustum &frustum, Shader &shader, unsigned int &display, unsigned int &draw)
{
    char buffer[100] = {'\0'};
    snprintf(buffer, sizeof(buffer), "spotLights[%d].", id);
    std::string str(buffer);

    //shader.SetInt((str + "base.base.enabled").c_str(), enabled);
    shader.SetInt("spotLightCount", spotLightCount);

    shader.SetVec3((str + "base.base.colour").c_str(), color);
    shader.SetFloat((str + "base.base.ambientIntensity").c_str(), ambientIntensity);
    shader.SetFloat((str + "base.base.diffuseIntensity").c_str(), diffuseIntensity);

    shader.SetVec3((str + "base.position").c_str(), transform->position);
    shader.SetFloat((str + "base.constant").c_str(), constant);
    shader.SetFloat((str + "base.linear").c_str(), linear);
    shader.SetFloat((str + "base.quadratic").c_str(), quadratic);

    shader.SetVec3((str + "direction").c_str(), transform->GetForward());
    shader.SetFloat((str + "edge").c_str(), cosf(glm::radians(edge)));
}

SpotLight::~SpotLight()
{
    spotLightCount--;
}