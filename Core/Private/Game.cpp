#include "../Public/Game.h"

#include <imgui/imgui.h>

#include "../Public/Shader.h"
#include "../Public/Rotating.h"
#include "../Public/DirectionalLight.h"
#include "../Public/PointLight.h"
#include "../Public/SpotLight.h"

namespace Vosgi
{
    Game::Game()
    {
        window = new Window_OpenGL(reinterpret_cast<WindowHandle *>(this), (GLfloat)800, (GLfloat)600);
        window->Initialize();

        shader = new Shader("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");
        shinyMaterial = Material(0.5f, 32.0f);

        // Create objects
        Entity *mainLightEntity = new Entity("Main Light", "Light");
        directionalLight = mainLightEntity->AddBehaviour<DirectionalLight>(.5f, .5f, .5f, 1.0f, 1.0f);
        mainLightEntity->transform.SetRotation(glm::quat(glm::radians(glm::vec3(45.0f, 45.0f, 0.0f))));

        // point light
        Entity *pointLightEntity = new Entity("Point Light", "Light");
        pointLight = pointLightEntity->AddBehaviour<PointLight>(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.1f);
        pointLight->color = glm::vec3(0.0f, 0.0f, 1.0f);
        pointLightEntity->transform.SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));

        // second point light
        Entity *pointLightEntity2 = new Entity("Point Light 2", "Light");
        PointLight *pointLight2 = pointLightEntity2->AddBehaviour<PointLight>(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.1f);
        pointLight2->color = glm::vec3(0.0f, 1.0f, 0.0f);
        pointLightEntity2->transform.SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));

        // third point light
        Entity *pointLightEntity3 = new Entity("Point Light 3", "Light");
        PointLight *pointLight3 = pointLightEntity3->AddBehaviour<PointLight>(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.1f);
        pointLight3->color = glm::vec3(1.0f, 0.0f, 0.0f);
        pointLightEntity3->transform.SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));

        // point light parent
        Entity *pointLightParent = new Entity("Point Light Parent", "Light");
        pointLightParent->AddChild(std::unique_ptr<Entity>(pointLightEntity));
        pointLightParent->AddChild(std::unique_ptr<Entity>(pointLightEntity2));
        pointLightParent->AddChild(std::unique_ptr<Entity>(pointLightEntity3));

        // spot light
        Entity *spotLightEntity = new Entity("Spot Light", "Light");
        spotLight = spotLightEntity->AddBehaviour<SpotLight>(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.1f, 35.0f);
        spotLightEntity->transform.SetPosition(glm::vec3(0.0f, 0.0f, 15.0f));
        //spotLightEntity->transform.SetRotation(glm::quat(glm::radians(glm::vec3(0.0f, 80.0f, 0.0f))));

        Entity *cameraEntity = new Entity("Main Camera", "MainCamera");
        camera = cameraEntity->AddBehaviour<Camera>(glm::vec3(0.0f, 0.0f, 10.0f), 45, window->GetAspectRatio(), 0.1f, 1000.0f);

        // Floor
        Entity *floorEntity = new Entity("Floor", "Untagged");
        floorEntity->AddBehaviour<Model>("Assets/Models/Floor/Floor.obj");
        floorEntity->transform.SetPosition(glm::vec3(0.0f, -25.0f, 0.0f));
        floorEntity->transform.SetLocalScale(glm::vec3(10.0f, 10.0f, 10.0f));

        // Flower
        Entity *flowerEntity = new Entity("Flower", "Untagged");
        flowerEntity->AddBehaviour<Model>("Assets/Models/flower/flower.obj");
        flowerEntity->AddBehaviour<Rotating>();
        flowerEntity->transform.SetPosition(glm::vec3(0.0f, -4.0f, 0.0f));

        // Lantern
        Entity *lanternEntity = new Entity("Lantern", "Untagged");
        lanternEntity->AddBehaviour<Model>("Assets/Models/Lantern/Lantern.obj");
        lanternEntity->transform.SetPosition(glm::vec3(0.0f, -4.0f, 15.0f));
        lanternEntity->transform.SetRotation(glm::quat(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f))));

        entities.push_back(std::unique_ptr<Entity>(cameraEntity));
        entities.push_back(std::unique_ptr<Entity>(mainLightEntity));
        entities.push_back(std::unique_ptr<Entity>(pointLightParent));
        entities.push_back(std::unique_ptr<Entity>(spotLightEntity));
        entities.push_back(std::unique_ptr<Entity>(flowerEntity));
        entities.push_back(std::unique_ptr<Entity>(lanternEntity));
        entities.push_back(std::unique_ptr<Entity>(floorEntity));
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

    void Game::Draw(float deltaTime, unsigned int &displayCount, unsigned int &drawCount, unsigned int &entityCount)
    {
        camera->keyControl(keys, deltaTime);

        shader->Use();

        shinyMaterial.Use(*shader);

        Frustum frustum = camera->getFrustum();

        for (auto &entity : entities)
        {
            entity->DrawSelfAndChildren(deltaTime, frustum, *shader, displayCount, drawCount, entityCount);
        }

        // Unbind shader
        glUseProgram(0);

        // Draw hierarchy
        ImGui::Begin("Hierarchy");
        for (auto &entity : entities)
        {
            ImGui::PushID((void *)entity.get());
            bool isEnabled = entity->enabled;
            if (ImGui::Checkbox(("##" + entity->name).c_str(), &isEnabled))
            {
                entity->enabled = isEnabled;
            }
            ImGui::SameLine();
            if (ImGui::CollapsingHeader(entity->name.c_str()))
            {
                // ImGui::SameLine();
                ImGui::Text("Tag: %s", entity->tag.c_str());
                ImGui::Text("GUID: %s", entity->GetGUID().c_str());
                ImGui::Separator();

                // Transform
                ImGui::SliderFloat3("Position", (float *)&entity->transform.position, -100.0f, 100.0f);

                // Retrieve the Euler angles in degrees
                glm::vec3 eulerDegrees = entity->transform.rotation.GetEulerAnglesDegrees();
                // Create a Float3 widget to edit the Euler angles
                if (ImGui::SliderFloat3("Rotation", (float *)&eulerDegrees, -180.0f, 180.0f))
                {
                    // Set the Euler angles in degrees
                    entity->transform.rotation.SetEulerAngles(glm::radians(eulerDegrees));
                    entity->transform.SetDirty();
                }

                ImGui::SliderFloat3("Scale", (float *)&entity->transform.localScale, 0.0f, 100.0f);
                entity->transform.SetDirty();

                ImGui::Separator();
                for (auto &behaviour : entity->GetBehaviours())
                {
                    ImGui::PushID((void *)behaviour.get());

                    // Get the type name of the behaviour
                    std::string typeName = typeid(*behaviour).name();
                    // cut any numbers from the start of the string
                    typeName = typeName.substr(typeName.find_first_not_of("0123456789"));

                    // add empty space for indentation
                    ImGui::Indent();

                    bool isEnabled = behaviour->enabled;
                    if (ImGui::Checkbox(("##" + typeName).c_str(), &isEnabled))
                    {
                        behaviour->enabled = isEnabled;
                    }
                    ImGui::SameLine();
                    if (ImGui::CollapsingHeader(typeName.c_str()))
                    {
                        behaviour->DrawInspector();
                    }

                    ImGui::Unindent();
                    ImGui::PopID();
                    ImGui::Separator();
                }
            }
            ImGui::PopID();
        }
        ImGui::End();
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

        camera->mouseControl(xChange, yChange);
    }

    void Game::ScrollCallback(double xOffset, double yOffset)
    {
        if (!window->GetMouseEnabled())
            return;

        // control camera fov
        float fov = camera->getFov() - static_cast<float>(yOffset);
        fov = glm::clamp(fov, 1.0f, 120.0f);
        camera->setFov(fov);
    }

}