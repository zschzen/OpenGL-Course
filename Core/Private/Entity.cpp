#include "../Public/Entity.h"

namespace Vosgi
{

    Entity::Entity()
    {
        name = std::string("New Entity");
        tag = std::string("Untagged");

        AssignEvents();
    }

    Entity::Entity(std::string name, std::string tag)
        : name(name), tag(tag)
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

    void Entity::DrawInspector()
    {
        ImGui::PushID((void*)GUID.c_str());

        bool isEnabled = enabled;
        if (ImGui::Checkbox(("##" + name).c_str(), &isEnabled)) {
            enabled = isEnabled;
        }
        ImGui::SameLine();
        if (!ImGui::CollapsingHeader(name.c_str())) {
            ImGui::PopID(); // Corrected: Added PopID here
            return;
        }

        // ImGui::SameLine();
        ImGui::Text("Tag: %s", tag.c_str());
        ImGui::Text("GUID: %s", GetGUID().c_str());
        ImGui::Separator();

        // Transform
        ImGui::SliderFloat3("Position", (float*)&transform.position, -100.0f, 100.0f);

        // Retrieve the Euler angles in degrees
        glm::vec3 eulerDegrees = transform.rotation.GetEulerAnglesDegrees();
        // Create a Float3 widget to edit the Euler angles
        if (ImGui::SliderFloat3("Rotation", (float*)&eulerDegrees, -180.0f, 180.0f)) {
            // Set the Euler angles in degrees
            transform.rotation.SetEulerAngles(glm::radians(eulerDegrees));
            transform.SetDirty();
        }

        ImGui::SliderFloat3("Scale", (float*)&transform.localScale, 0.0f, 100.0f);
        transform.SetDirty();

        ImGui::Separator();
        for (auto& behaviour : behaviours) {
            ImGui::PushID((void*)behaviour.get());

            // Get the type name of the behaviour
            std::string typeName = typeid(*behaviour).name();
            // cut any numbers from the start of the string
            typeName = typeName.substr(typeName.find_first_not_of("0123456789"));

            // add empty space for indentation
            ImGui::Indent();

            bool isEnabled1 = behaviour->enabled;
            if (ImGui::Checkbox(("##" + typeName).c_str(), &isEnabled1)) {
                behaviour->enabled = isEnabled1;
            }
            ImGui::SameLine();
            if (ImGui::CollapsingHeader(typeName.c_str())) {
                ImGui::PushID((void*)behaviour->GetGUID().c_str());
                ImGui::Text("GUID: %s", behaviour->GetGUID().c_str());
                behaviour->DrawInspector();
                ImGui::PopID();
            }

            ImGui::PopID();
            ImGui::Unindent();
            ImGui::Separator();
        }

        // Children
        if (!children.empty())
        {
            ImGui::Text("Children: %d", static_cast<int>(children.size()));
            ImGui::Indent();
            for (auto &child: children) {
                ImGui::PushID((void *) child.get());
                child->DrawInspector();
                ImGui::PopID();
            }
            ImGui::Unindent();
        }

        ImGui::PopID(); // Corrected: Added PopID here
    }


    void Entity::AssignEvents()
    {
        enabled.Subscribe(MakeMethodPointer(this, &Entity::SetEnabled));
    }

}