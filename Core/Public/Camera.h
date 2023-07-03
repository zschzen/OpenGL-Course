#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Forward declaration
struct Frustum;

class Camera
{
public:
    Camera();
    Camera(glm::vec3 startPos, GLfloat fov, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);
    ~Camera();

    void keyControl(bool* key, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    inline glm::vec3 getCameraPosition() const { return position; }
    inline glm::vec3 getCameraDirection() const { return glm::normalize(front); }

    inline glm::vec3 getFront() const { return front; }
    inline glm::vec3 getRight() const { return right; }
    inline glm::vec3 getUp() const { return up; }
    inline glm::vec3 getWorldUp() const { return worldUp; }

    inline glm::mat4 calculateViewMatrix() const { return glm::lookAt(position, position + front, up); }

    // Get frustum planes
    Frustum getFrustum() const;

    inline glm::mat4 getProjectionMatrix() const { return projection; }

public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    GLfloat yaw = -90.0f;
    GLfloat pitch = 0.0f;

    GLfloat movementSpeed = 5.0f;
    GLfloat turnSpeed = 0.1f;

    glm::mat4 projection = glm::mat4(1.0f);
    GLfloat fov = 45.0f;
    GLfloat aspectRatio = 800.0f / 600.0f;
    GLfloat nearPlane = 0.1f;
    GLfloat farPlane = 100.0f;

    void update();
};