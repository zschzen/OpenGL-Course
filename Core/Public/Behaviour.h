#ifndef __BEHAVIOUR_H__
#define __BEHAVIOUR_H__

#pragma once

#include <memory>

#include "Transform.h"

// Forward declaration
class Entity;
class Shader;
struct Frustum;

/*
 * This cass wraps the basic behaviour of the entity's components.
 */
namespace Vosgi
{
    class Behaviour
    {
    public:
        Behaviour() {}
        virtual ~Behaviour() {}

        virtual void Initialize() {}
        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void LateUpdate(float deltaTime) {}
        virtual void Draw(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw) {}
        virtual void Terminate() {}

        inline bool IsActive() const { return m_isActive; }

    //protected:
        Entity* entity = nullptr;
        Vosgi::Transform* transform = nullptr;

        bool m_isActive = true;
    };
}
#endif // !__BEHAVIOUR_H__