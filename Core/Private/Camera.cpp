#include "../Public/Camera.h"

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f; 
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    movementSpeed = 5.0f;
    turnSpeed = 0.1f;

    update();
}

Camera::Camera(glm::vec3 startPos, glm::vec3 startUp, GLfloat startYaw,
               GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPos;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    movementSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
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

    if (keys[GLFW_KEY_UP])
    {
        pitch += turnSpeed * deltaTime;
    }

    if (keys[GLFW_KEY_DOWN])
    {
        pitch -= turnSpeed * deltaTime;
    }

    if (keys[GLFW_KEY_LEFT])
    {
        yaw -= turnSpeed * deltaTime;
    }

    if (keys[GLFW_KEY_RIGHT])
    {
        yaw += turnSpeed * deltaTime;
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
}

Camera::~Camera()
{
}