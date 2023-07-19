#include "../Public/Camera.h"

#include "../Public/Frustum.h"
#include "../Public/Shader.h"

Camera::Camera()
{
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    movementSpeed = 5.0f;
    turnSpeed = 0.1f;

    // Calculate the projection matrix
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

Camera::Camera(glm::vec3 startPos, GLfloat fov, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane)
{
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    movementSpeed = 5.0f;
    turnSpeed = 0.1f;

    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    // Calculate the projection matrix
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::keyControl(bool *keys, GLfloat deltaTime)
{
    // if shift is pressed, move faster, if not, move slower
    if (keys[GLFW_KEY_LEFT_SHIFT])
    {
        movementSpeed = 10.0f;
    }
    else if (keys[GLFW_KEY_LEFT_CONTROL])
    {
        movementSpeed = 1.0f;
    }
    else
    {
        movementSpeed = 5.0f;
    }

    const GLfloat velocity = movementSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        transform->position += transform->GetForward() * velocity;
        transform->SetDirty();
    }

    if (keys[GLFW_KEY_S])
    {
        transform->position -= transform->GetForward() * velocity;
        transform->SetDirty();
    }

    if (keys[GLFW_KEY_A])
    {
        transform->position -= transform->GetRight() * velocity;
        transform->SetDirty();
    }

    if (keys[GLFW_KEY_D])
    {
        transform->position += transform->GetRight() * velocity;
        transform->SetDirty();
    }

    if (keys[GLFW_KEY_Q])
    {
        transform->position -= worldUp * velocity;
        transform->SetDirty();
    }

    if (keys[GLFW_KEY_E])
    {
        transform->position += worldUp * velocity;
        transform->SetDirty();
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= -turnSpeed;
    yChange *= turnSpeed;

    glm::quat rotation = transform->rotation;

    // create rotation quaternion
    glm::quat pitch = glm::angleAxis(glm::radians(yChange), transform->GetRight());
    glm::quat yaw = glm::angleAxis(glm::radians(xChange), worldUp);

    // apply rotation
    rotation = glm::normalize(yaw * pitch * rotation);

    transform->SetRotation(rotation);
}

void Camera::Draw(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw)
{
    // Set the projection matrix
    shader.SetMat4("projection", projection);

    // Set the view matrix
    shader.SetMat4("view", calculateViewMatrix());

    // Set the camera position
    shader.SetVec3("eyePos", getCameraPosition());
}

// TODO: Cache frustum planes
Frustum Camera::getFrustum() const
{
    Frustum frustum;
    const float halfVSide = farPlane * tanf(glm::radians(fov) * .5f);
    const float halfHSide = halfVSide * aspectRatio;
    const glm::vec3 frontMultFar = farPlane * getFront();

    frustum.nearFace = {getCameraPosition() + nearPlane * getFront(), getFront()};
    frustum.farFace = {getCameraPosition() + frontMultFar, -getFront()};
    frustum.rightFace = {getCameraPosition(), glm::cross(frontMultFar - getRight() * halfHSide, getUp())};
    frustum.leftFace = {getCameraPosition(), glm::cross(getUp(), frontMultFar + getRight() * halfHSide)};
    frustum.topFace = {getCameraPosition(), glm::cross(getRight(), frontMultFar - getUp() * halfVSide)};
    frustum.bottomFace = {getCameraPosition(), glm::cross(frontMultFar + getUp() * halfVSide, getRight())};

    return frustum;
}

Camera::~Camera()
{
}