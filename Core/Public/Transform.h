#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Quaternion.h"

namespace Vosgi
{
    class Transform
    {
    public:
        // Constructors
        Transform() = default;
        Transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale);
        ~Transform();

        // Getters
        const glm::mat4 &GetModel() const { return m_model; }

        // Directions
        glm::vec3 GetForward() const { return rotation * glm::vec3(0.0f, 0.0f, 1.0f); }
        glm::vec3 GetRight() const { return rotation * glm::vec3(1.0f, 0.0f, 0.0f); }
        glm::vec3 GetUp() const { return rotation * glm::vec3(0.0f, 1.0f, 0.0f); }

        // Getters
        glm::vec3 GetWorldScale() const { return glm::vec3(glm::length(GetRight()), glm::length(GetUp()), glm::length(GetForward())); }

        // Setters
        void SetPosition(glm::vec3 pos)
        {
            position = pos;
            m_isDirty = true;
        }

        void SetLocalPosition(glm::vec3 pos)
        {
            localPosition = pos;
            m_isDirty = true;
        }

        void SetRotation(glm::quat rotation)
        {
            this->rotation = rotation;
            m_isDirty = true;
        }

        void SetLocalRotation(glm::quat rotation)
        {
            localRotation = rotation;
            m_isDirty = true;
        }

        void SetLocalScale(glm::vec3 scale)
        {
            localScale = scale;
            m_isDirty = true;
        }

        void SetForward(glm::vec3 forward)
        {
            glm::vec3 newForward = glm::normalize(forward);
            glm::vec3 currentForward = GetForward();

            float angle = glm::acos(glm::dot(currentForward, newForward));
            glm::vec3 axis = glm::cross(currentForward, newForward);

            rotation = glm::angleAxis(angle, axis);
            m_isDirty = true;
        }

        void SetUp(glm::vec3 up)
        {
            glm::vec3 newUp = glm::normalize(up);
            glm::vec3 currentUp = GetUp();

            float angle = glm::acos(glm::dot(currentUp, newUp));
            glm::vec3 axis = glm::cross(currentUp, newUp);

            rotation = glm::angleAxis(angle, axis);
            m_isDirty = true;
        }

        void SetRight(glm::vec3 right)
        {
            glm::vec3 newRight = glm::normalize(right);
            glm::vec3 currentRight = GetRight();

            float angle = glm::acos(glm::dot(currentRight, newRight));
            glm::vec3 axis = glm::cross(currentRight, newRight);

            rotation = glm::angleAxis(angle, axis);
            m_isDirty = true;
        }

        // Dirty flag
        bool IsDirty() const { return m_isDirty; }
        void SetDirty() { m_isDirty = true; }

        // Model matrix computation
        void ComputeModelMatrix();
        void ComputeModelMatrix(glm::mat4 parentModel);

    public:
        // Member variables
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 localPosition = {0.0f, 0.0f, 0.0f};

        Quaternion rotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        Quaternion localRotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

        glm::vec3 localScale = {1.0f, 1.0f, 1.0f};

    protected:
        glm::mat4 m_model = glm::mat4(1.0f);

        bool m_isDirty = true;\

    private:
        // Helper function
        glm::mat4 GetLocalModelMatrix() const;
    };
}

#endif // !__TRANSFORM_H__
