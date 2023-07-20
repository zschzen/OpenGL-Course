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
        explicit Window_OpenGL(WindowHandle* windowHandle);
        ~Window_OpenGL() override;

        int Initialize() override;
        void Run() override;
        void SwapBuffers() override;
        void PollEvents() override;
        void Terminate() override;

        void SetWindowLabel(const char *label) override;
        void SetMouseEnabled(bool enabled) override;

        void CreateCallbacks() override;

        // Frame buffer size callback
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

        // Input callbacks
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MousePositionCallback(GLFWwindow* window, double xPos, double yPos);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        static void CharCallback(GLFWwindow* window, unsigned int codepoint);

        void SetClearColor(float r, float g, float b, float a);

        // Getters
        inline GLFWwindow* GetWindow() const { return window; }

    protected:
        GLFWwindow* window = nullptr;

    private:
        // Sample fps counter
        float fps_values[180]{0};
        int fps_values_offset = 0;
    };
}

#endif // !__WINDOW_OPENGL_H__