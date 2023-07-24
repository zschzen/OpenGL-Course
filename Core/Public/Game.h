#ifndef __GAME_H__
#define __GAME_H__

#pragma once

#include <vector>
#include <memory>

#include "../Public/Window_OpenGL.h"

#include "../Public/Entity.h"
#include "../Public/Shader.h"
#include "../Public/Delegates.h"
#include "../Public/Camera.h"
#include "../Public/Model.h"
#include "../Public/Material.h"

// Forward declarations
class Entity;
class Shader;
class PointLight;
class SpotLight;
class DirectionalLight;

namespace Vosgi
{
    class Game : public WindowHandle
    {
    public:
        Game();
        ~Game();

        void Run();

        void Draw(float deltaTime, unsigned int& displayCount, unsigned int& drawCount, unsigned int& entityCount) override;

        // Callbacks
        void KeyCallback(int key, int scancode, int action, int mods) override;
        void MouseCallback(double xPos, double yPos) override;
        void ScrollCallback(double xOffset, double yOffset) override;

    private:
        Window *window = nullptr;
        Shader *shader = nullptr;

        bool keys[1024]{false};

        GLfloat lastX = 0, lastY = 0;
        GLfloat xChange = 0, yChange = 0;
        bool mouseFirstMoved = true;

        Camera* camera;
        Material shinyMaterial;

        std::vector<std::unique_ptr<Entity>> entities = std::vector<std::unique_ptr<Entity>>();
    };
} // namespace Vosgi

#endif // !__GAME_H__