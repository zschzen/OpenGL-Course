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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Core/Public/Window.h"
#include "Core/Public/ModelLoader.h"
#include "Core/Public/Mesh.h"
#include "Core/Public/Shader.h"
#include "Core/Public/Delegates.h"

#include "Core/Public/Shapes.h"

// Scene rendering
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Rendering matrices and uniforms
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
GLuint uniformProjection = 0, uniformModel = 0;

// Frame rate
int MAX_FPS = 60;
double deltaTime = 0.0f;

// Sample fps counter
static float fps_values[180] = { 0 };
static int fps_values_offset = 0;

// Shader paths
static const char* vShader = "./Shaders/shader.vert";
static const char* fShader = "./Shaders/shader.frag";

// Model path
static const char* modelPath = "./Models/pyramid.obj";

void CreateObjects()
{
    // Load model
    MeshData modelData = ModelLoader::LoadModel(modelPath);

    // Create mesh
    Mesh* mesh1 = new Mesh(modelData);
    meshList.push_back(mesh1);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void ClearScreen()
{
    // Clear the window
    glClearColor(0.16f, 0.16f, 0.21f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderScene()
{
    // Update the shader
    shaderList[0].UseShader();
    uniformModel = shaderList[0].GetModelLocation();
    uniformProjection = shaderList[0].GetProjectionLocation();

    // Update the projection matrix
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

    // Render the scene
    for (size_t i = 0; i < meshList.size(); ++i)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -5.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f) * (GLfloat)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        // Update the model matrix
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        meshList[i]->RenderMesh();
    }

    // Unbind the shader
    glUseProgram(0);
}

int main()
{
    // Create the window
    mainWindow = Window(800, 600);
    mainWindow.Initialize();

    // Calculate the desired frame time in seconds
    double desiredFrameTime = 1.0 / MAX_FPS;

    // Variables for frame rate control
    double previousFrameTime = glfwGetTime();
    double currentFrameTime = 0.0f;

    // Create the scene
    CreateObjects();
    CreateShaders();

    // Create the projection matrix
    projection = glm::perspective(glm::radians(45.0f), mainWindow.getAspectRatio(), 0.1f, 100.0f);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(mainWindow.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

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
            if (sleepTime <= 0.0f)
                continue;

            // Sleep for the remaining time
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime)));

            continue;
        }

        // Update the previous frame time
        previousFrameTime = currentFrameTime;

        // Get + Handle User Input
        glfwPollEvents();

        ClearScreen();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Update the window
        RenderScene();

        ImGui::Begin("Performance");
        fps_values[fps_values_offset] = 1.0f / deltaTime;
        fps_values_offset = (fps_values_offset + 1) % IM_ARRAYSIZE(fps_values);
        // FPS: 60.00 (1.00ms)
        char buffer[32];
        sprintf(buffer, "FPS: %.2f (%.2fms)", 1.0f / deltaTime, deltaTime * 1000.0f);

        ImGui::PlotLines(buffer, fps_values, IM_ARRAYSIZE(fps_values), fps_values_offset, NULL, 0.0f, 100.0f, ImVec2(0, 80));

        // Set new fps
        ImGui::SliderInt("Max FPS", &MAX_FPS, 1, 144);
        desiredFrameTime = 1.0 / MAX_FPS;

        ImGui::End();

        // Render the ImGui frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the buffers
        mainWindow.swapBuffers();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
