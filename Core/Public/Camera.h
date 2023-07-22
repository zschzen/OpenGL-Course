#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Behaviour.h"

class Camera : public Vosgi::Behaviour
{
public:
    Camera();
    Camera(glm::vec3 startPos, GLfloat fov, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);
    ~Camera();

    void keyControl(bool *key, float deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    void Update(float deltaTime) override;
    void DrawInspector() override;

    inline glm::vec3 getCameraPosition() const { return transform->position; }
    inline glm::vec3 getCameraDirection() const { return transform->GetForward(); }

    // Getters
    inline glm::vec3 getFront() const { return transform->GetForward(); }
    inline glm::vec3 getRight() const { return transform->GetRight(); }
    inline glm::vec3 getUp() const { return transform->GetUp(); }
    inline glm::vec3 getWorldUp() const { return worldUp; }
    inline GLfloat getFov() const { return fov; }

    inline glm::mat4 calculateViewMatrix() const
    {
        glm::vec3 pos = transform->position;
        return glm::lookAt(pos, pos - transform->GetForward(), transform->GetUp());
    }

    // Get frustum planes
    Frustum getFrustum() const;

    inline glm::mat4 getProjectionMatrix() const { return projection; }

    // Setters
    inline void setFov(GLfloat newFov)
    {
        fov = newFov;
        projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

public:
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    GLfloat movementSpeed = 5.0f;
    GLfloat turnSpeed = 0.1f;

    glm::mat4 projection = glm::mat4(1.0f);
    GLfloat fov = 45.0f;
    GLfloat aspectRatio = 800.0f / 600.0f;
    GLfloat nearPlane = 0.1f;
    GLfloat farPlane = 100.0f;
};