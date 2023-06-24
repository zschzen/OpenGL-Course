#include "../Public/Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	aspectRatio = (GLfloat)width / (GLfloat)height;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	aspectRatio = (GLfloat)width / (GLfloat)height;
}

int Window::Initialize()
{
	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

    // Setup GLFW
    // OpenGL 3.3 (MAJOR.MINOR)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Set major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Set minor version to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile (no backwards compatibility)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Allow forward compatibility

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Udemy OpenGL Course", NULL, NULL);
	if (!mainWindow)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the current context
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

    // Enable depth
	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
