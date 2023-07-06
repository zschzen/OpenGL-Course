#ifndef __ENTITY_H__
#define __ENTITY_H__

#pragma once

#include <glm/glm.hpp> //glm::mat4
#include <list>        //std::list
#include <array>       //std::array
#include <memory>      //std::unique_ptr
#include <algorithm>   //std::max
#include <limits>      //std::numeric_limits

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.h"
#include "Frustum.h"
#include "Shader.h"
#include "Model.h"
#include "Behaviour.h"
#include "BoundingVolume.h"

namespace Vosgi
{
    class Entity
    {
    public:
        std::string name = "New Entity";
        std::string tag = "Untagged";

        Transform transform = Transform();

        std::list<std::unique_ptr<Entity>> children = std::list<std::unique_ptr<Entity>>();
        std::list<std::unique_ptr<Behaviour>> behaviours = std::list<std::unique_ptr<Behaviour>>();
        Entity *parent = nullptr;

        Entity();
        Entity(Model *model);
        Entity(std::string name, std::string tag, Transform transform, Model *model);

        ~Entity();

        void AddChild(std::unique_ptr<Entity> child);
        void RemoveChild(Entity *child);
        void RemoveChild(int index);

        template <typename T, typename... Args>
        T* AddBehaviour(Args&&...args)
        {
            T* behaviour = new T(std::forward<Args>(args)...);
            behaviour->transform = &transform;
            behaviours.push_back(std::unique_ptr<T>(behaviour));

            return behaviour;
        }

        template <typename T>
        T *GetBehaviour()
        {
            for (auto &behaviour : behaviours)
            {
                if (dynamic_cast<T *>(behaviour.get()))
                {
                    return dynamic_cast<T *>(behaviour.get());
                }
            }

            return nullptr;
        }

        template <typename T>
        void RemoveBehaviour()
        {
            for (auto &behaviour : behaviours)
            {
                if (dynamic_cast<T *>(behaviour.get()))
                {
                    behaviours.remove(behaviour);
                    return;
                }
            }
        }

        void UpdateSelfAndChildren();
        void ForceUpdateSelfAndChildren();

        void DrawSelfAndChildren(const Frustum &frustum, Shader &shader, unsigned int &display, unsigned int &draw, unsigned int &total);
    };
}
#endif // !__ENTITY_H__