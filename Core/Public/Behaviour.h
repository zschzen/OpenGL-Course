#ifndef __BEHAVIOUR_H__
#define __BEHAVIOUR_H__

#pragma once

#include <memory>

#include "Transform.h"
#include "Observable.h"

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
        Behaviour()
        {
            enabled.Subscribe([this](bool value) {
                if (value)
                    OnEnable();
                else
                    OnDisable();
            });
        }
        virtual ~Behaviour() {}

        virtual void Initialize() {}
        virtual void Start() {}
        virtual void OnEnable() {}
        virtual void OnDisable() {}
        virtual void Update(float deltaTime) {}
        virtual void LateUpdate(float deltaTime) {}
        virtual void Draw(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw) {}
        virtual void Terminate() {}

        inline bool IsActive() { return enabled; }

    //protected:
        Entity* entity = nullptr;
        Vosgi::Transform* transform = nullptr;

        Observable<bool> enabled = true;
    };
}
#endif // !__BEHAVIOUR_H__