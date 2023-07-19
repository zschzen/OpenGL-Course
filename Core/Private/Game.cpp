#include "../Public/Game.h"

#include <imgui/imgui.h>

#include "../Public/Shader.h"
#include "../Public/Rotating.h"
#include "../Public/DirectionalLight.h"
#include "../Public/PointLight.h"

namespace Vosgi
{
    Game::Game()
    {
        window = new Window_OpenGL(reinterpret_cast<WindowHandle *>(this), (GLfloat)800, (GLfloat)600);
        window->Initialize();

        shader = new Shader("Shaders/shader.vert", "Shaders/shader.frag");
        shinyMaterial = Material(0.5f, 32.0f);

        // Create objects
        Entity* mainLightEntity = new Entity("Main Light", "Light");
        mainLightEntity->AddBehaviour<DirectionalLight>(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        mainLightEntity->transform.SetRotation(glm::quat(glm::radians(glm::vec3(45.0f, 45.0f, 0.0f))));

        // point light
        Entity* pointLightEntity = new Entity("Point Light", "Light");
        pointLight = pointLightEntity->AddBehaviour<PointLight>(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.1f);
        pointLight->color = glm::vec3(0.0f, 0.0f, 1.0f);
        pointLightEntity->transform.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));

        Entity* cameraEntity = new Entity("Camera", "Untagged");
        camera = cameraEntity->AddBehaviour<Camera>(glm::vec3(0.0f, 0.0f, 10.0f), 45, window->GetAspectRatio(), 0.1f, 1000.0f);

        Entity* flowerEntity = new Entity("Flower", "Untagged");

        flowerEntity->AddBehaviour<Model>("Models/flower/flower.obj");
        flowerEntity->AddBehaviour<Rotating>();
        flowerEntity->transform.SetPosition(glm::vec3(0.0f, -5.0f, -15.0f));
        flowerEntity->transform.SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));

        entities.push_back(std::unique_ptr<Entity>(cameraEntity));
        entities.push_back(std::unique_ptr<Entity>(mainLightEntity));
        entities.push_back(std::unique_ptr<Entity>(pointLightEntity));
        entities.push_back(std::unique_ptr<Entity>(flowerEntity));
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

        // Draw rendering controls
        Light &mainLight = *entities[1]->GetBehaviour<Light>();
        ImGui::Begin("Light");
        ImGui::ColorEdit3("Light Color", glm::value_ptr(mainLight.color));
        ImGui::SliderFloat("Ambient Intensity", &mainLight.ambientIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse Intensity", &mainLight.diffuseIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular Intensity", &shinyMaterial.specularIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &shinyMaterial.shininess, 1.0f, 256.0f);
        ImGui::Separator();
        // point light
        ImGui::Text("Point Light");
        ImGui::ColorEdit3("Color", glm::value_ptr(pointLight->color));
        ImGui::SliderFloat("Intensity", &pointLight->ambientIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse", &pointLight->diffuseIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Constant", &pointLight->constant, 0.0f, 1.0f);
        ImGui::SliderFloat("Linear", &pointLight->linear, 0.0f, 1.0f);
        ImGui::SliderFloat("Quadratic", &pointLight->quadratic, 0.0f, 1.0f);
        ImGui::End();

        // Draw hierarchy
        ImGui::Begin("Hierarchy");
        for (auto &entity : entities)
        {
            ImGui::Checkbox(("##" + entity->GetGUID()).c_str(), &entity->enabled);
            ImGui::SameLine();
            if (ImGui::CollapsingHeader((entity->name + " (" + entity->GetGUID() + ")").c_str()))
            {
                //ImGui::SameLine();
                ImGui::Text("Tag: %s", entity->tag.c_str());
                ImGui::Separator();
                if (ImGui::CollapsingHeader("Transform"))
                {
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
                }
                ImGui::Separator();
                for (auto &behaviour : entity->GetBehaviours())
                {
                    // Get the type name of the behaviour
                    std::string typeName = typeid(*behaviour).name();
                    // cut any numbers from the start of the string
                    typeName = typeName.substr(typeName.find_first_not_of("0123456789"));

                    ImGui::Checkbox(("##" + typeName).c_str(), &behaviour->enabled);
                    ImGui::SameLine();
                    if (ImGui::CollapsingHeader(typeName.c_str()))
                    {
                        ImGui::Text("Type: %s", typeName.c_str());
                    }
                    ImGui::Separator();
                }
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