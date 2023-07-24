#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <glm/glm.hpp>
#include <list>
#include <memory>
#include <algorithm>
#include <string>

#include "Transform.h"
#include "Frustum.h"
#include "Shader.h"
#include "Model.h"
#include "Behaviour.h"
#include "BoundingVolume.h"
#include "Observable.h"

namespace Vosgi
{
    class Entity
    {
    public:
        Entity();
        Entity(std::string name, std::string tag);
        ~Entity();

        void AddChild(std::unique_ptr<Entity> child);
        void RemoveChild(Entity* child);
        void RemoveChild(size_t index);

        void SetEnabled(bool value);

        // Getters and Setters
        std::list<std::unique_ptr<Entity>>& GetChildren() { return children; }
        std::string GetGUID() const { return GUID; }
        std::list<std::unique_ptr<Behaviour>>& GetBehaviours() { return behaviours; }

        /* Add a behaviour to this entity. */
        template <typename T, typename... Args>
        T* AddBehaviour(Args&&... args)
        {
            static_assert(std::is_base_of<Behaviour, T>::value, "Must be derived from Behaviour");
            T* behaviour = new T(std::forward<Args>(args)...);
            behaviour->transform = &transform;
            behaviour->OnEnable();
            behaviours.push_back(std::unique_ptr<T>(behaviour));
            return behaviour;
        }

        /* Get the first behaviour of the given type. */
        template <typename T>
        T* GetBehaviour() const
        {
            static_assert(std::is_base_of<Behaviour, T>::value, "Must be derived from Behaviour");
            for (const auto& behaviour : behaviours)
            {
                T* castBehaviour = dynamic_cast<T*>(behaviour.get());

                if (!castBehaviour) continue;
                return castBehaviour;
            }
            return nullptr;
        }

        /* Get all behaviours of the given type. */
        template <typename T>
        std::list<T*> GetBehaviours() const
        {
            static_assert(std::is_base_of<Behaviour, T>::value, "Must be derived from Behaviour");
            std::list<T*> castBehaviours = std::list<T*>();
            for (const auto& behaviour : behaviours)
            {
                T* castBehaviour = dynamic_cast<T*>(behaviour.get());

                if (!castBehaviour) continue;
                castBehaviours.push_back(castBehaviour);
            }
            return castBehaviours;
        }

        /* Remove all behaviours of the given type. */
        template <typename T>
        void RemoveBehaviour()
        {
            static_assert(std::is_base_of<Behaviour, T>::value, "Must be derived from Behaviour");
            behaviours.remove_if([](const std::unique_ptr<Behaviour>& behaviour) {
                return dynamic_cast<T*>(behaviour.get()) != nullptr;
            });
        }

    public:
        void UpdateSelfAndChildren();
        void ForceUpdateSelfAndChildren();

        void DrawSelfAndChildren(float deltaTime, const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw, unsigned int& total);

        void DrawInspector();

    public:
        std::string name = "New Entity";
        std::string tag = "Untagged";
        Transform transform = Transform();

        Observable<bool> enabled = true;

    private:
        std::list<std::unique_ptr<Entity>> children = std::list<std::unique_ptr<Entity>>();
        std::list<std::unique_ptr<Behaviour>> behaviours = std::list<std::unique_ptr<Behaviour>>();
        Entity* parent = nullptr;

        std::string GUID = RandomGUID(8);

    private:
        void AssignEvents();
    };
} // namespace Vosgi

#endif // __ENTITY_H__
