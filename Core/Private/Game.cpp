#include "../Public/Game.h"

#include <imgui/imgui.h>

#include "../Public/Skybox.h"

#include "../Public/Rotating.h"
#include "../Public/DirectionalLight.h"
#include "../Public/PointLight.h"
#include "../Public/SpotLight.h"

#include "../Public/Shapes.h"

namespace Vosgi
{
    Game::Game()
    {
        window = new Window_OpenGL(reinterpret_cast<WindowHandle *>(this), (GLfloat)1000, (GLfloat)750);
        window->Initialize();

        shader = new Shader("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");

        // Create objects
        Entity *mainLightEntity = new Entity("Main Light", "Light");
        mainLightEntity->AddBehaviour<DirectionalLight>(.5f, .5f, .5f, 1.0f, 1.0f);
        mainLightEntity->transform.SetRotation(glm::quat(glm::radians(glm::vec3(45.0f, 45.0f, 0.0f))));

        // point light
        Entity *pointLightEntity = new Entity("Point Light", "Light");
        PointLight *pointLight = pointLightEntity->AddBehaviour<PointLight>(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.1f);
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
        pointLightParent->AddBehaviour<Rotating>();

        // spot light
        Entity *spotLightEntity = new Entity("Spot Light", "Light");
        spotLightEntity->AddBehaviour<SpotLight>(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.1f, 35.0f);
        spotLightEntity->transform.SetPosition(glm::vec3(0.0f, 0.0f, 15.0f));
        spotLightEntity->transform.SetRotation(glm::quat(glm::radians(glm::vec3(0.0f, 180.0f, 0.0f))));

        Entity *cameraEntity = new Entity("Main Camera", "MainCamera");
        camera = cameraEntity->AddBehaviour<Camera>(glm::vec3(0.0f, 0.0f, 10.0f), 45, window->GetAspectRatio(), 0.1f, 1000.0f);

        // Floor
        Entity *floorEntity = new Entity("Floor", "Untagged");
        floorEntity->AddBehaviour<Model>("Assets/Models/Floor/Floor.obj");
        floorEntity->transform.SetPosition(glm::vec3(0.0f, -23.0f, 0.0f));
        floorEntity->transform.SetLocalScale(glm::vec3(10.0f, 10.0f, 10.0f));

        // Calvin and Hobbes
        Entity *calvinEntity = new Entity("Calvin And Hobbes by npbehunin", "Untagged");
        auto calvinModel = calvinEntity->AddBehaviour<Model>("Assets/Models/calvinhobbes/scene.gltf");
        calvinModel->GetMaterial().specularIntensity = 0.0f;

        // Generated Mesh
        //Entity *cubeEntity = new Entity("Cube", "Untagged");
        //cubeEntity->AddBehaviour<Model>(Shapes::Cube());

        entities.push_back(std::unique_ptr<Entity>(cameraEntity));
        entities.push_back(std::unique_ptr<Entity>(mainLightEntity));
        entities.push_back(std::unique_ptr<Entity>(pointLightParent));
        entities.push_back(std::unique_ptr<Entity>(spotLightEntity));
        entities.push_back(std::unique_ptr<Entity>(calvinEntity));
        //entities.push_back(std::unique_ptr<Entity>(cubeEntity));
        entities.push_back(std::unique_ptr<Entity>(floorEntity));

        // Create skybox
        std::vector<std::string> cubeMapFaces = {
                "Assets/Textures/Skybox/FS002_Day_Cubemap_right.png",
                "Assets/Textures/Skybox/FS002_Day_Cubemap_left.png",
                "Assets/Textures/Skybox/FS002_Day_Cubemap_up.png",
                "Assets/Textures/Skybox/FS002_Day_Cubemap_down.png",
                "Assets/Textures/Skybox/FS002_Day_Cubemap_back.png",
                "Assets/Textures/Skybox/FS002_Day_Cubemap_front.png"
        };
        skybox = new Skybox(cubeMapFaces);
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
        skybox->Draw(camera->calculateViewMatrix(), camera->getProjectionMatrix());

        camera->keyControl(keys, deltaTime);

        shader->Use();

        Frustum frustum = camera->getFrustum();

        for (auto &entity : entities)
        {
            entity->DrawSelfAndChildren(deltaTime, frustum, *shader, displayCount, drawCount, entityCount);

            ImGui::Begin("Hierarchy");
            entity->DrawInspector();
            ImGui::End();
        }

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