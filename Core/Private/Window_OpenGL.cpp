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
        glfwWindowHint(GLFW_SAMPLES, 8);                               // 8x antialiasing

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

        // Get buffer size information
        glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

        // Set the current context
        glfwMakeContextCurrent(window);

        // Handle key and mouse input
        CreateCallbacks();

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

        lastTime = 0.0f;
        currentTime = 0.0f;
        do
        {
            // Calculate delta time
            CalculateDeltaTime();

            // Limit FPS
            if (deltaTime < desiredFrameTime)
            {
                int sleepTime = static_cast<int>((desiredFrameTime - deltaTime) * 1000);
                if (sleepTime > 0)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                    CalculateDeltaTime();
                }
            }

            // Update the previous frame time
            lastTime = currentTime;

            // Get + Handle User Input
            PollEvents();

            // Clear the window
            glClearColor(.95f, 0.65f, 0.95f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Start the ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Draw FPS sampler
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(0, 0));
            ImGui::Begin("FPS Sampler");
            ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
            ImGui::End();

            // Update the window
            windowHandle->Draw(deltaTime);

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
        glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    void Window_OpenGL::CreateCallbacks()
    {
        glfwSetCursorPosCallback(window, MousePositionCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetKeyCallback(window, KeyCallback);
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