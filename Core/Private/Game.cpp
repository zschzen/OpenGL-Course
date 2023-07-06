#include "../Public/Game.h"

#include <imgui/imgui.h>

#include "../Public/Shader.h"

namespace Vosgi
{
    Game::Game()
    {
        window = new Window_OpenGL(reinterpret_cast<WindowHandle *>(this), (GLfloat)800, (GLfloat)600);
        window->Initialize();

        camera = Camera(glm::vec3(0.0f, 0.0f, -10.0f), 45, window->GetAspectRatio(), 0.1f, 100.0f);
        shader = new Shader("Shaders/shader.vert", "Shaders/shader.frag");

        // Create objects
        Model* model1 = new Model("Models/flower/flower.obj");
        Entity* entity1 = new Entity(model1);
        Entity* entity2 = new Entity(model1);
        entity1->AddChild(std::unique_ptr<Entity>(entity2));
        entity1->transform.SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        entity1->transform.SetLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));
        entity2->transform.SetLocalPosition(glm::vec3(30.0f, 0.0f, 0.0f));
        entities.push_back(std::unique_ptr<Entity>(entity1));
    }

    Game::~Game()
    {
        window->Terminate();
        // delete window;
    }

    void Game::Run()
    {
        window->Run();
    }

    void Game::Draw(float deltaTime)
    {
        unsigned int displayCount = 0;
        unsigned int drawCount = 0;
        unsigned int entityCount = 0;

        camera.keyControl(keys, deltaTime);

        shader->Use();

        shader->SetMat4("projection", camera.getProjectionMatrix());
        shader->SetMat4("view", camera.calculateViewMatrix());
        shader->SetVec3("eyePos", camera.getCameraPosition());

        mainLight.Use(*shader);
        shinyMaterial.Use(*shader);

        Frustum frustum = camera.getFrustum();

        for (auto &entity : entities)
        {
            entity->DrawSelfAndChildren(frustum, *shader, displayCount, drawCount, entityCount);
        }

        // Draw the displays, draws
        ImGui::Begin("Display");
        ImGui::Text("Display count: %d", displayCount);
        ImGui::Text("Draw count: %d", drawCount);
        ImGui::Text("Entity count: %d", entityCount);
        ImGui::End();

        // Unbind shader
        glUseProgram(0);
    }

    void Game::KeyCallback(int key, int scancode, int action, int mods)
    {
        // Check if the key is within the range of the array
        if (key < 0 || key >= 1024) return;

        // Set the key to true if pressed, false if released
        keys[key] = action != GLFW_RELEASE;
    }

    void Game::MouseCallback(double xPos, double yPos)
    {
        if (!mouseEnabled)
            return;

        if (mouseFirstMoved)
        {
            lastX = (GLfloat)xPos;
            lastY = (GLfloat)yPos;
            mouseFirstMoved = false;
        }

        xChange = (GLfloat)xPos - lastX;
        yChange = lastY - (GLfloat)yPos;

        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;

        camera.mouseControl(xChange, yChange);
    }

}