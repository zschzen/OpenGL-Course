#ifndef __ENTITY_H__
#define __ENTITY_H__

#pragma once

#include <glm/glm.hpp> //glm::mat4
#include <list> //std::list
#include <array> //std::array
#include <memory> //std::unique_ptr
#include <algorithm> //std::max
#include <limits> //std::numeric_limits

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.h"
#include "Frustum.h"
#include "Shader.h"
#include "Model.h"
#include "BoundingVolume.h"

class Entity
{
public:
    std::string name = "New Entity";
    std::string tag = "Untagged";

    Transform transform = Transform();

    std::list<std::unique_ptr<Entity>> children = {};
    Entity* parent = nullptr;

    Model* model = nullptr;
    std::unique_ptr<AABB> aabb = nullptr;

    Entity();
    Entity(std::string name, std::string tag, Model* model);
    Entity(std::string name, std::string tag, Transform transform, Model* model);

    ~Entity();

    void AddChild(std::unique_ptr<Entity> child);
    void RemoveChild(Entity* child);

    AABB* GetWorldAABB() const;

    void UpdateSelfAndChildren();
    void ForceUpdateSelfAndChildren();

    void DrawSelfAndChildren(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& total);
};

#endif // !__ENTITY_H__