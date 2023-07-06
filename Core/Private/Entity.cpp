#include "../Public/Entity.h"

#include "../Public/BoundingVolume.h"

namespace Vosgi
{

    Entity::Entity()
    {
        name = std::string("New Entity");
        tag = std::string("Untagged");
    }

    Entity::Entity(std::string name, std::string tag, Transform transform, Model *model)
        : name(name), tag(tag), transform(transform)
    {
    }

    Entity::~Entity()
    {
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

    void Entity::RemoveChild(int index)
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

    void Entity::DrawSelfAndChildren(const Frustum &frustum, Shader &shader, unsigned int &display, unsigned int &draw, unsigned int &total)
    {
        UpdateSelfAndChildren();

        for (auto& component : behaviours)
        {
            if (!component->IsActive()) continue;

            component->Update(0.0f);
            // ...
            component->LateUpdate(0.0f);
            component->Draw(frustum, shader, display, draw);
        }
        total++;

        for (auto &child : children)
        {
            child->DrawSelfAndChildren(frustum, shader, display, draw, total);
        }
    }
}