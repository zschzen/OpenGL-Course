#include "../Public/Window_OpenGL.h"

#include <iostream>

#include <chrono>
#include <thread>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Vosgi
{
    Window_OpenGL::Window_OpenGL(WindowHandle *windowHandle, GLint width, GLint height)
        : Window(windowHandle, width, height) {}

    Window_OpenGL::Window_OpenGL(WindowHandle *windowHandle)
        : Window(windowHandle) {}

    Window_OpenGL::~Window_OpenGL()
    {
        Terminate();
    }

    int Window_OpenGL::Initialize()
    {
        if (!glfwInit())
        {
            printf("ERROR::GLFW_INIT_FAILED\n");
            glfwTerminate();
            return 1;
        }

        // Setup GLFW
        // OpenGL 3.3 (MAJOR.MINOR)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Set major version to 3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Set minor version to 3
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile (no backwards compatibility)
        glfwWindowHint(GLFW_SAMPLES, 4);                               // 4x antialiasing

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for Mac
#endif

        // Enable vsync
        glfwSwapInterval(1);

        // Create the window
        window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
        if (!window)
        {
            printf("ERROR::GLFW_WINDOW_CREATION_FAILED\n");
            glfwTerminate();
            return 1;
        }

        // Maximize the window
        //glfwMaximizeWindow(window);

        // Get buffer size information
        glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

        // Set the current context
        glfwMakeContextCurrent(window);

        // Handle key and mouse input
        CreateCallbacks();

        // Handle window resizing
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

        // Lock the cursor to the window
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Allow modern extension access
        glewExperimental = GL_TRUE;

        GLenum error = glewInit();
        if (error != GLEW_OK)
        {
            printf("ERROR::GLEW_INIT_FAILED: %s\n", glewGetErrorString(error));
            glfwDestroyWindow(window);
            glfwTerminate();
            return 1;
        }

        // Enable depth
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);

        // Create Viewport
        glViewport(0, 0, bufferWidth, bufferHeight);

        // Set the window to the current object
        glfwSetWindowUserPointer(window, reinterpret_cast<void *>(windowHandle));

        return 0;
    }

    void Window_OpenGL::Run()
    {
        // Setup ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        do
        {
            // Resize the viewport
            glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
            glViewport(0, 0, bufferWidth, bufferHeight);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            // Calculate delta time
            CalculateDeltaTime();

            // Limit FPS
            if (deltaTime < desiredFrameTime)
            {
                int sleepTime = static_cast<int>((desiredFrameTime - deltaTime) * 1000);
                if (sleepTime > 0)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                    continue;
                }
            }

            // Update the previous frame time
            lastTime = currentTime;

            // Reset counters
            unsigned int displayCount = 0;
            unsigned int drawCount = 0;
            unsigned int entityCount = 0;

            // Get + Handle User Input
            PollEvents();

            // Clear the window
            glClearColor(.95f, 0.65f, 0.95f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Start the ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Update the window
            windowHandle->Draw(deltaTime, displayCount, drawCount, entityCount);

            # if 1
            // Draw FPS sampler
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(0, 0));
            ImGui::Begin("FPS");
            fps_values[fps_values_offset] = 1.0f / deltaTime;
            fps_values_offset = (fps_values_offset + 1) % IM_ARRAYSIZE(fps_values);
            char buffer[32];
            sprintf(buffer, "FPS: %.2f (%.2fms)\n\nDisplay: %d\nDraw: %d\nEntity: %d", 1.0f / deltaTime, deltaTime * 1000, displayCount, drawCount, entityCount);

            ImGui::PlotLines(buffer, fps_values, IM_ARRAYSIZE(fps_values), fps_values_offset, NULL, 0.0f, 100.0f, ImVec2(0, 120));

            // Set new fps
            ImGui::SliderInt("Max FPS", &maxFPS, 1, 144);
            desiredFrameTime = 1.0 / maxFPS;

            ImGui::End();
            # endif

            // Render ImGui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            SwapBuffers();

        } while (!glfwWindowShouldClose(window));

        // Terminate window
        Terminate();
    }

    void Window_OpenGL::SwapBuffers()
    {
        glfwSwapBuffers(window);
    }

    void Window_OpenGL::PollEvents()
    {
        glfwPollEvents();
    }

    void Window_OpenGL::Terminate()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();

        exit(0);
    }

    void Window_OpenGL::SetWindowLabel(const char *label)
    {
        glfwSetWindowTitle(window, label);
    }

    void Window_OpenGL::SetMouseEnabled(bool enabled)
    {
        m_mouseEnabled = enabled;
        glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    void Window_OpenGL::CreateCallbacks()
    {
        glfwSetCursorPosCallback(window, MousePositionCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetKeyCallback(window, KeyCallback);
    }

    void Window_OpenGL::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
        
        Window* windowHandle = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        windowHandle->CalculateAspectRatio();
    }

    void Window_OpenGL::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        WindowHandle *windowHandle = reinterpret_cast<WindowHandle *>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS)
        {
            if (key == GLFW_KEY_ESCAPE)
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
        }

        windowHandle->KeyCallback(key, scancode, action, mods);
    }

    void Window_OpenGL::MousePositionCallback(GLFWwindow *window, double xPos, double yPos)
    {
        WindowHandle *windowHandle = reinterpret_cast<WindowHandle *>(glfwGetWindowUserPointer(window));
        windowHandle->MouseCallback(xPos, yPos);
    }

    void Window_OpenGL::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        WindowHandle *windowHandle = reinterpret_cast<WindowHandle *>(glfwGetWindowUserPointer(window));
        windowHandle->MouseButtonCallback(button, action, mods);
    }

    void Window_OpenGL::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
    {
        WindowHandle *windowHandle = reinterpret_cast<WindowHandle *>(glfwGetWindowUserPointer(window));
        windowHandle->ScrollCallback(xOffset, yOffset);
    }

    void Window_OpenGL::CharCallback(GLFWwindow *window, unsigned int codepoint)
    {
        WindowHandle *windowHandle = reinterpret_cast<WindowHandle *>(glfwGetWindowUserPointer(window));
        windowHandle->CharCallback(codepoint);
    }
}