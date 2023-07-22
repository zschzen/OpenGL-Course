#include "../Public/Entity.h"

#include "../Public/GUID.h"

namespace Vosgi
{

    Entity::Entity()
    {
        name = std::string("New Entity");
        tag = std::string("Untagged");
        GUID = RandomGUID(8);

        AssignEvents();
    }

    Entity::Entity(std::string name, std::string tag)
        : name(name), tag(tag), GUID(RandomGUID(8))
    {
        AssignEvents();
    }

    Entity::~Entity()
    {
        for (auto &behaviour : behaviours)
        {
            behaviour->Terminate();
        }
        behaviours.clear();

        for (auto &child : children)
        {
            child->~Entity();
        }
        children.clear();
    }

    void Entity::AddChild(std::unique_ptr<Entity> child)
    {
        child->parent = this;
        children.push_back(std::move(child));
    }

    void Entity::RemoveChild(Entity *child)
    {
        children.remove_if([child](std::unique_ptr<Entity> &c)
                           { return c.get() == child; });
    }

    void Entity::RemoveChild(size_t index)
    {
        auto it = children.begin();
        std::advance(it, index);
        children.erase(it);
    }

    void Entity::UpdateSelfAndChildren()
    {
        if (transform.IsDirty())
        {
            ForceUpdateSelfAndChildren();
        }

        for (auto &child : children)
        {
            child->UpdateSelfAndChildren();
        }
    }

    void Entity::ForceUpdateSelfAndChildren()
    {
        if (parent != nullptr)
        {
            transform.ComputeModelMatrix(parent->transform.GetModel());
        }
        else
        {
            transform.ComputeModelMatrix();
        }

        for (auto &child : children)
        {
            child->ForceUpdateSelfAndChildren();
        }
    }

    void Entity::SetEnabled(bool value)
    {
        enabled = value;

        // Propagate to behaviours
        for (auto &behaviour : behaviours)
        {
            behaviour->enabled = value;
        }

        // Propagate to children
        for (auto &child : children)
        {
            child->SetEnabled(value);
        }
    }

    void Entity::DrawSelfAndChildren(float deltaTime, const Frustum &frustum, Shader &shader, unsigned int &display, unsigned int &draw, unsigned int &total)
    {
        if (!enabled) return;

        UpdateSelfAndChildren();

        for (auto &component : behaviours)
        {
            if (!component->IsActive()) continue;

            component->Update(deltaTime);
            // ...
            component->LateUpdate(deltaTime);
            component->Draw(frustum, shader, display, draw);
        }
        total++;

        for (auto &child : children)
        {
            child->DrawSelfAndChildren(deltaTime, frustum, shader, display, draw, total);
        }
    }

    void Entity::AssignEvents()
    {
        enabled.Subscribe(MakeMethodPointer(this, &Entity::SetEnabled));
    }

}