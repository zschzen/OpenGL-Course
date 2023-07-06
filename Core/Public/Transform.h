#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Vosgi
{

    class Transform
    {
    public:
        Transform();
        Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

        const glm::mat4 &GetModel() const { return m_model; }

        // Position
        const glm::vec3 &GetLocalPosition() const { return m_localPosition; }
        glm::vec3 GetWorldPosition() const { return glm::vec3(m_model[3]); }

        // Rotation
        const glm::vec3 &GetLocalRotation() const { return m_localEulerRotation; }
        glm::vec3 GetWorldRotation() const { return glm::vec3(glm::eulerAngles(glm::quat_cast(m_model))); }

        // Scale
        const glm::vec3 &GetLocalScale() const { return m_localScale; }
        glm::vec3 GetWorldScale() const { return {glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBack())}; }

        // Directions
        glm::vec3 GetForward() const { return glm::vec3(-m_model[2]); }
        glm::vec3 GetBack() const { return glm::vec3(m_model[2]); }
        glm::vec3 GetLeft() const { return glm::vec3(-m_model[0]); }
        glm::vec3 GetRight() const { return glm::vec3(m_model[0]); }
        glm::vec3 GetUp() const { return glm::vec3(m_model[1]); }
        glm::vec3 GetDown() const { return glm::vec3(-m_model[1]); }

        // Setters
        void SetLocalPosition(glm::vec3 pos)
        {
            m_localPosition = pos;
            m_isDirty = true;
        }
        void SetLocalRotation(glm::vec3 rot)
        {
            m_localEulerRotation = rot;
            m_isDirty = true;
        }
        void SetLocalScale(glm::vec3 scale)
        {
            m_localScale = scale;
            m_isDirty = true;
        }

        bool IsDirty() const { return m_isDirty; }

        void ComputeModelMatrix();
        void ComputeModelMatrix(glm::mat4 parentModel);

        ~Transform();

    protected:
        glm::vec3 m_localPosition = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_localEulerRotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_localScale = {1.0f, 1.0f, 1.0f};

        glm::mat4 m_model = glm::mat4(1.0f);

        bool m_isDirty = true;

    protected:
        glm::mat4 GetLocalModelMatrix() const;
    };
}
#endif // !__TRANSFORM_H__
