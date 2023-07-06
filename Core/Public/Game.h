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
#include "../Public/Light.h"
#include "../Public/Material.h"

// Forward declarations
class Entity;
class Shader;

namespace Vosgi
{
    class Game : public WindowHandle
    {
    public:
        Game();
        ~Game();

        void Run();

        void Draw(float deltaTime) override;

        // Callbacks
        void KeyCallback(int key, int scancode, int action, int mods) override;
        void MouseCallback(double xPos, double yPos) override;

    private:
        Window *window = nullptr;
        Shader *shader = nullptr;

        bool keys[1024]{false};

        GLfloat lastX = 0, lastY = 0;
        GLfloat xChange = 0, yChange = 0;
        bool mouseFirstMoved = true;
        bool mouseEnabled = true;

        Camera camera;
        Light mainLight = Light(1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f);
        Material shinyMaterial = Material(1.0f, 32);

        std::vector<std::unique_ptr<Entity>> entities = std::vector<std::unique_ptr<Entity>>();
    };
} // namespace Vosgi

#endif // !__GAME_H__