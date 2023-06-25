#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Public/Window.h"
#include "Core/Public/ModelLoader.h"
#include "Core/Public/Mesh.h"
#include "Core/Public/Shader.h"
#include "Core/Public/Delegates.h"

#include "Core/Public/Shapes.h"

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

const int MAX_FPS = 60;
double deltaTime = 0.0f;

// Vertex Shader
static const char *vShader = "./Shaders/shader.vert";

// Fragment Shader
static const char *fShader = "./Shaders/shader.frag";

// Model path
static const char *modelPath = "./Models/pyramid.obj";

void CreateObjects()
{
	// Load model
	MeshData modelData = ModelLoader::LoadModel(modelPath);

	// Create mesh
	Mesh *mesh1 = new Mesh(modelData);
	meshList.push_back(mesh1);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialize();

	// Calculate the desired frame time in seconds
	double desiredFrameTime = 1.0 / MAX_FPS;

	// Variables for frame rate control
	double previousFrameTime = glfwGetTime();
	double currentFrameTime = 0.0f;

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getAspectRatio(), 0.1f, 100.0f);

	glm::mat4 model(1.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Calculate the time elapsed since the previous frame
		currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - previousFrameTime;

		// Limit the frame rate
		if (deltaTime < desiredFrameTime)
		{
			// Sleep to limit the frame rate
			double sleepTime = (desiredFrameTime - deltaTime) * 1000;
			if (sleepTime > 0)
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime)));

			continue;
		}

		// Update the previous frame time
		previousFrameTime = currentFrameTime;

		// Get + Handle User Input
		glfwPollEvents();

		// Clear the window
		glClearColor(0.16f, 0.16f, 0.21f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		for (size_t i = 0; i < meshList.size(); ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -0.5f, -5.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			model = glm::rotate(model, glm::radians(45.0f) * -(GLfloat)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			meshList[i]->RenderMesh();
		}

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}