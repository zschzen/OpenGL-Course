#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 startPos, glm::vec3 startUp, GLfloat startYaw,
           GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    ~Camera();

    void keyControl(bool* key, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    inline glm::vec3 getCameraPosition() const { return position; }
    inline glm::mat4 calculateViewMatrix() const { return glm::lookAt(position, position + front, up); }

private:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    GLfloat yaw = -90.0f;
    GLfloat pitch = 0.0f;

    GLfloat movementSpeed = 5.0f;
    GLfloat turnSpeed = 0.1f;

    void update();
};