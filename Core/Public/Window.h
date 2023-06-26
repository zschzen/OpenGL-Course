#pragma once

#include <stdio.h>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Delegates.h"

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
    inline bool* getKeys() { return keys; }

    // Setters
    inline void setShouldClose(bool shouldClose) { glfwSetWindowShouldClose(mainWindow, shouldClose ? GL_TRUE : GL_FALSE); }
    inline void setWindowLabel(const char* label) { glfwSetWindowTitle(mainWindow, label); }

    // Other
    inline void swapBuffers() { glfwSwapBuffers(mainWindow); }

    // Callbacks
    Delegate<int, int, int, int> keyCallback = Delegate<int, int, int, int>();
    Delegate<double, double> mouseCallback = Delegate<double, double>();

    ~Window();

private:
    GLFWwindow* mainWindow = nullptr;

    GLint width = 0;
    GLint height = 0;
    GLint bufferWidth, bufferHeight = 0;
    GLfloat aspectRatio = 0;

    // Keys and mouse
    bool keys[1024]{};

    GLfloat lastX = 0, lastY = 0;
    GLfloat xChange = 0, yChange = 0;
    bool mouseFirstMoved = true;

    // Callbacks
    void createCallbacks();

    /*
    * @brief: Handles key presses and releases.
    * @param window: The window that received the event.
    * @param key: The keyboard key that was pressed or released.
    * @param code: The system-specific scancode of the key.
    * @param action: GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
    */
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);

    /*
    * @brief: Handles mouse movement.
    * @param window: The window that received the event.
    * @param xPos: The new cursor x-coordinate, relative to the left edge of the client area.
    * @param yPos: The new cursor y-coordinate, relative to the top edge of the client area.
    */
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};