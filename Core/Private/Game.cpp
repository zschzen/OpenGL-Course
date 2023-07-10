#include "../Public/Game.h"

#

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
        Entity *entity1 = new Entity("Flower", "Untagged");
        Entity *entity2 = new Entity("Flower (1)", "Untagged");
        entity1->AddBehaviour<Model>("Models/flower/flower.obj");
        entity2->AddBehaviour<Model>("Models/flower/flower.obj");
        entity1->transform.SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));
        entity2->transform.SetLocalPosition(glm::vec3(15.0f, 0.0f, 0.0f));
        entity2->transform.SetLocalScale(glm::vec3(.5f, .5f, .5f));
        entity1->AddChild(std::unique_ptr<Entity>(entity2));
        entities.push_back(std::unique_ptr<Entity>(entity1));
    }

    Game::~Game()
    {
        window->Terminate();
        delete window;
    }

    void Game::Run()
    {
        window->Run();
    }

    void Game::Draw(float deltaTime, unsigned int& displayCount, unsigned int& drawCount, unsigned int& entityCount)
    {
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

        // Draw rendering controls
        ImGui::Begin("Light");
        ImGui::ColorEdit3("Light Color", glm::value_ptr(mainLight.color));
        ImGui::DragFloat3("Light Position", glm::value_ptr(mainLight.direction), 0.01f, -1.0f, 1.0f);
        ImGui::SliderFloat("Ambient Intensity", &mainLight.ambientIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse Intensity", &mainLight.diffuseIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular Intensity", &shinyMaterial.specularIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &shinyMaterial.shininess, 1.0f, 256.0f);
        ImGui::Separator();
        ImGui::Text("FOV: %f", camera.getFov());
        ImGui::End();

        // Draw entity controls
        ImGui::Begin("Entities");
        for (auto &entity : entities)
        {
            if (ImGui::CollapsingHeader((entity->name + " (" + entity->GetGUID() + ")").c_str()))
            {
                ImGui::Checkbox("Active", &entity->active);
                ImGui::SliderFloat3("Position", (float *)&entity->transform.m_localPosition, -100.0f, 100.0f);
                ImGui::SliderFloat3("Rotation", (float *)&entity->transform.m_localEulerRotation, -180.0f, 180.0f);
                ImGui::SliderFloat3("Scale", (float *)&entity->transform.m_localScale, 0.0f, 100.0f);
                entity->transform.SetDirty();
            }
        }
        ImGui::End();

        // Unbind shader
        glUseProgram(0);
    }

    void Game::KeyCallback(int key, int scancode, int action, int mods)
    {
        // Check if the key is within the range of the array
        if (key < 0 || key >= 1024)
            return;

        // Toggle mouse
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            window->SetMouseEnabled(!window->GetMouseEnabled());
        }

        // Set the key to true if pressed, false if released
        keys[key] = action != GLFW_RELEASE;
    }

    void Game::MouseCallback(double xPos, double yPos)
    {
        if (!window->GetMouseEnabled())
        {
            if (!mouseFirstMoved)
            {
                mouseFirstMoved = true;
            }
            return;
        }

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

    void Game::ScrollCallback(double xOffset, double yOffset)
    {
        if (!window->GetMouseEnabled()) return;

        // control camera fov
        float fov = camera.getFov() - static_cast<float>(yOffset);
        fov = glm::clamp(fov, 1.0f, 120.0f);
        camera.setFov(fov);
    }

}