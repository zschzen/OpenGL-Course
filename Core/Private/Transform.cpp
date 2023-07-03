#include "../Public/Transform.h"

#include <stdio.h>

Transform::Transform()
{
    m_localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    m_localEulerRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_localScale = glm::vec3(1.0f, 1.0f, 1.0f);

    m_model = glm::mat4(1.0f);

    m_isDirty = true;
}

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
    m_localPosition = pos;
    m_localEulerRotation = rot;
    m_localScale = scale;

    m_model = glm::mat4(1.0f);

    m_isDirty = true;
}

Transform::~Transform()
{
}

glm::mat4 Transform::GetLocalModelMatrix() const
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(m_localEulerRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(m_localEulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_localEulerRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), m_localPosition) * rotationMatrix * glm::scale(glm::mat4(1.0f), m_localScale);
}

void Transform::ComputeModelMatrix()
{
    m_model = GetLocalModelMatrix();
    m_isDirty = false;
}

void Transform::ComputeModelMatrix(glm::mat4 parentModel)
{
    m_model = parentModel * GetLocalModelMatrix();
    m_isDirty = false;
}