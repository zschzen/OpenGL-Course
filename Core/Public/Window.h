#pragma once

#include "stdio.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();

    Window(GLint windowWidth, GLint windowHeight);

    int Initialize();

    // Getters
     GLint getBufferWidth() { return bufferWidth; }
     GLint getBufferHeight() { return bufferHeight; }
     GLfloat getAspectRatio() { return aspectRatio; }
     bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    // Setters
     void setShouldClose(bool shouldClose) { glfwSetWindowShouldClose(mainWindow, shouldClose); }
     void setWindowLabel(const char* label) { glfwSetWindowTitle(mainWindow, label); }

    // Other
    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();

private:
    GLFWwindow* mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;
    GLfloat aspectRatio;
};