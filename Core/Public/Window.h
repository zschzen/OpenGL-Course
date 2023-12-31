#ifndef __WINDOW_H__
#define __WINDOW_H__

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Vosgi
{
    class WindowHandle
    {
    public:
        void virtual Draw(float deltaTime, unsigned int &displayCount, unsigned int &drawCount, unsigned int &entityCount) {}
        void virtual KeyCallback(int key, int scancode, int action, int mods) {}
        void virtual MouseCallback(double xPos, double yPos) {}
        void virtual MouseButtonCallback(int button, int action, int mods) {}
        void virtual ScrollCallback(double xOffset, double yOffset) {}
        void virtual CharCallback(unsigned int codepoint) {}
    };

    class Window
    {
    public:
        Window(WindowHandle *windowHandle, GLint width, GLint height)
            : windowHandle(windowHandle), width(width), height(height)
        {
            bufferWidth = width;
            bufferHeight = height;
            aspectRatio = (GLfloat)width / (GLfloat)height;
        }

        Window(WindowHandle *windowHandle)
            : windowHandle(windowHandle)
        {
            width = 800;
            height = 600;
            bufferWidth = width;
            bufferHeight = height;
            aspectRatio = (GLfloat)width / (GLfloat)height;
        }

        virtual int Initialize() = 0;
        virtual void Run() = 0;
        virtual void SwapBuffers() = 0;
        virtual void PollEvents() = 0;
        virtual void Terminate() = 0;

        virtual void CreateCallbacks() = 0;

        // Setters
        virtual void SetWindowLabel(const char *label) = 0;
        virtual void SetMouseEnabled(bool enabled) = 0;

        // Getters
        inline GLint GetBufferWidth() const { return bufferWidth; }
        inline GLint GetWidth() const { return width; }
        inline GLint GetHeight() const { return height; }
        inline GLint GetBufferHeight() const { return bufferHeight; }
        inline GLfloat GetAspectRatio() const { return aspectRatio; }
        inline WindowHandle *GetWindowHandle() const { return windowHandle; }
        inline bool GetMouseEnabled() const { return m_mouseEnabled; }

        inline float GetDeltaTime() const { return deltaTime; }

        // Calculate aspect ratio
        virtual void CalculateAspectRatio()
        {
            aspectRatio = (GLfloat)width / (GLfloat)height;
        }

    protected:
        WindowHandle *windowHandle;
        GLint width = 800, height = 600;
        GLint bufferWidth = 0, bufferHeight = 0;
        GLfloat aspectRatio = 0;

        bool m_mouseEnabled = true;

        int maxFPS = 30;
        float deltaTime = 0.0f;
        float lastTime = 0.0f;
        float currentTime = 0.0f;
        float desiredFrameTime = 1.0f / maxFPS;

        // Calculate delta time
        virtual void CalculateDeltaTime()
        {
            currentTime = static_cast<float>(glfwGetTime());
            deltaTime = currentTime - lastTime;
        }

    public:
        virtual ~Window() = default;
    };

} // namespace Vosgi

#endif // !__WINDOW_H__