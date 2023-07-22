#pragma once

#include "Behaviour.h"

class Rotating : public Vosgi::Behaviour
{
public:
    Rotating() : Behaviour() {}
    explicit Rotating(float speed) : Behaviour(), speed(speed) {}
    ~Rotating() override = default;

    // rotate the object around the Y-axis using quaternions angle-axis rotation
    void Update(float deltaTime) override
    {
        glm::quat rotation = glm::angleAxis(glm::radians(speed * deltaTime), axis);
        transform->SetRotation(transform->rotation * rotation);
    }

    void DrawInspector() override
    {
        ImGui::SliderFloat("Speed", &speed, 0.f, 360.f);
    }

    float speed = 90.f;

private:
    glm::vec3 axis = glm::vec3(0.f, 1.f, 0.f);
};