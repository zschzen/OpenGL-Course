#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW initialization failed!");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW
    // OpenGL 3.3 (MAJOR.MINOR)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set minor version to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile (no backwards compatibility)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Allow forward compatibility

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Udemy OpenGL Course", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW Window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set GLEW context
    glfwMakeContextCurrent(mainWindow);

    // Allow modern experimental features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initalization failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup viewport size
    glViewport(0,0, bufferWidth, bufferHeight);

    // Main loop
    do
    {
        // Get input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.16f, 0.16f, 0.21f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    } while (!glfwWindowShouldClose(mainWindow));

    return 0;
}