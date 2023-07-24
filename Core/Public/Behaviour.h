#ifndef __BEHAVIOUR_H__
#define __BEHAVIOUR_H__

#pragma once

#include <memory>
#include <string>

#include "Transform.h"
#include "Observable.h"
#include "imgui/imgui.h"
#include "GUID.h"

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
        virtual ~Behaviour() = default;

        virtual void Initialize() {}
        virtual void Start() {}
        virtual void OnEnable() {}
        virtual void OnDisable() {}
        virtual void Update(float deltaTime) {}
        virtual void LateUpdate(float deltaTime) {}
        virtual void Draw(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw) {}
        virtual void Terminate() {}

        virtual void DrawInspector() = 0;

        inline bool IsActive() { return enabled; }

    //protected:
        Entity* entity = nullptr;
        Vosgi::Transform* transform = nullptr;

        Observable<bool> enabled = true;

    public:
        [[nodiscard]] std::string GetGUID() const { return GUID; }

    private:
        // guid
        std::string GUID = RandomGUID(8);
    };
}
#endif // !__BEHAVIOUR_H__