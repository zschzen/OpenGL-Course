#include "../Public/Entity.h"

#include "../Public/BoundingVolume.h"

Entity::Entity()
{
}

Entity::Entity(std::string name, std::string tag, Model *model)
    : name(name), tag(tag), model(model)
{
    aabb = std::make_unique<AABB>(generateAABB(*model));
}

Entity::Entity(std::string name, std::string tag, Transform transform, Model *model)
    : name(name), tag(tag), transform(transform), model(model)
{
    aabb = std::make_unique<AABB>(generateAABB(*model));
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

AABB *Entity::GetWorldAABB() const
{
    // Get global scale thanks to our transform
    const glm::vec3 globalCenter{transform.GetModel() * glm::vec4(aabb->center, 1.f)};

    // Scaled orientation
    const glm::vec3 right = transform.GetRight() * aabb->extents.x;
    const glm::vec3 up = transform.GetUp() * aabb->extents.y;
    const glm::vec3 forward = transform.GetForward() * aabb->extents.z;

    const float newIi = std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, right)) +
                        std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, up)) +
                        std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, forward));

    const float newIj = std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, right)) +
                        std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, up)) +
                        std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, forward));

    const float newIk = std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, right)) +
                        std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, up)) +
                        std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, forward));

    return new AABB(globalCenter, newIi, newIj, newIk);
}

void Entity::UpdateSelfAndChildren()
{
    if (transform.IsDirty())
    {
        ForceUpdateSelfAndChildren();
    }

    for (auto& child : children)
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

    for (auto& child : children)
    {
        child->ForceUpdateSelfAndChildren();
    }
}

void Entity::DrawSelfAndChildren(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& total)
{
    if (aabb->isOnFrustum(frustum, transform))
    {
        UpdateSelfAndChildren();

        shader.SetMat4("model", transform.GetModel());
        model->Render(shader);
        display++;
    }
    total++;

    for (auto &child : children)
    {
        child->DrawSelfAndChildren(frustum, shader, display, total);
    }
}