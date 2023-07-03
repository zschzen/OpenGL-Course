#include "../Public/Camera.h"

#include "../Public/Frustum.h"

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = 90.0f;
    pitch = 0.0f;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    movementSpeed = 5.0f;
    turnSpeed = 0.1f;

    update();
}

Camera::Camera(glm::vec3 startPos, GLfloat fov, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane)
{
    position = startPos;
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = 90.0f;
    pitch = 0.0f;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    movementSpeed = 5.0f;
    turnSpeed = 0.1f;

    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    update();
}

void Camera::keyControl(bool *keys, GLfloat deltaTime)
{
    const GLfloat velocity = movementSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }

    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }

    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }

    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }

    if (keys[GLFW_KEY_Q])
    {
        position -= worldUp * velocity;
    }

    if (keys[GLFW_KEY_E])
    {
        position += worldUp * velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    // Clamp the pitch
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update();
}

void Camera::update()
{
    GLfloat yawCos = cos(glm::radians(yaw));
    GLfloat yawSin = sin(glm::radians(yaw));
    GLfloat pitchCos = cos(glm::radians(pitch));
    GLfloat pitchSin = sin(glm::radians(pitch));

    // Calculate front vector
    front.x = yawCos * pitchCos;
    front.y = pitchSin;
    front.z = yawSin * pitchCos;

    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    // Calculate the projection matrix
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
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