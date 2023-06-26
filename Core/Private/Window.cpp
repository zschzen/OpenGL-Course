#include "../Public/Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	aspectRatio = (GLfloat)width / (GLfloat)height;

	for (auto &key : keys)
	{
		key = false;
	}
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);				   // Set major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);				   // Set minor version to 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile (no backwards compatibility)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Allow forward compatibility
	glfwWindowHint(GLFW_SAMPLES, 8);							   // 8x antialiasing

	// Enable vsync
	glfwSwapInterval(1);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
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
	glEnable(GL_MULTISAMPLE);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Set the window to the current object
	glfwSetWindowUserPointer(mainWindow, this);

	// Handle key presses and releases
	glfwSetKeyCallback(mainWindow, handleKeys);

	return 0;
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
	// Get the window
	Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

	// Close the window if the escape key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		theWindow->setShouldClose(true);
		return;
	}

	// Check if the key is within the range of the array
	if (key < 0 || key >= 1024) return;

	// Set the key to true if pressed, false if released
	theWindow->keys[key] = action != GLFW_RELEASE;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
