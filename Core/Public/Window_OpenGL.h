#ifndef __WINDOW_OPENGL_H__
#define __WINDOW_OPENGL_H__

#pragma once

#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Vosgi
{
    class Window_OpenGL : public Window
    {
    public:
        Window_OpenGL(WindowHandle* windowHandle, GLint width, GLint height);
        ~Window_OpenGL();

        int Initialize() override;
        void Run() override;
        void SwapBuffers() override;
        void PollEvents() override;
        void Terminate() override;
        void SetWindowLabel(const char *label) override;
        void SetMouseEnabled(bool enabled) override;

        void CreateCallbacks() override;
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MousePositionCallback(GLFWwindow* window, double xPos, double yPos);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        static void CharCallback(GLFWwindow* window, unsigned int codepoint);

        void SetClearColor(float r, float g, float b, float a);

    protected:
        GLFWwindow* window = nullptr;

        bool mouseEnabled = false;
    };
}

#endif // !__WINDOW_OPENGL_H__