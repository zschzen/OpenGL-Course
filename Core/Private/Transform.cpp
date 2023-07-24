#include "../Public/Transform.h"
#include <glm/gtc/quaternion.hpp>

namespace Vosgi
{
    Transform::Transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
    {
        position = pos;
        rotation = rot;
        localScale = scale;
    }

    Transform::~Transform()
    {
    }

    glm::mat4 Transform::GetLocalModelMatrix() const
    {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = static_cast<glm::mat4>(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), localScale);

        return translationMatrix * rotationMatrix * scaleMatrix;
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
}
