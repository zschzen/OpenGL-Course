#pragma once

#include <stdio.h>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();

    Window(GLint windowWidth, GLint windowHeight);

    int Initialize();

    // Getters
    inline GLint getBufferWidth() { return bufferWidth; }
    inline GLint getBufferHeight() { return bufferHeight; }
    inline GLfloat getAspectRatio() { return aspectRatio; }
    inline bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
    inline GLFWwindow* getWindow() { return mainWindow; }

    // Setters
    inline void setShouldClose(bool shouldClose) { glfwSetWindowShouldClose(mainWindow, shouldClose ? GL_TRUE : GL_FALSE); }
    inline void setWindowLabel(const char* label) { glfwSetWindowTitle(mainWindow, label); }

    // Other
    inline void swapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();

private:
    GLFWwindow* mainWindow = nullptr;

    GLint width = 0;
    GLint height = 0;
    GLint bufferWidth, bufferHeight = 0;
    GLfloat aspectRatio = 0;

    bool keys[1024]{};

    /*
    * @brief: Handles key presses and releases.
    * @param window: The window that received the event.
    * @param key: The keyboard key that was pressed or released.
    * @param code: The system-specific scancode of the key.
    * @param action: GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
    */
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};