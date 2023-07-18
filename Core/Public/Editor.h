#ifndef __EDITOR_H__
#define __EDITOR_H__

#pragma once

#include "Window_OpenGL.h"

namespace Vosgi
{
    class Engine;
    class Entity;

    class Editor : public WindowHandle
    {
    public:
        Editor(Engine* engine);
        ~Editor();

        void Run();

    private:
        void DrawMenuBar();
        void DrawHierarchy();
        void DrawInspector();
        
        void Draw(float deltaTime, unsigned int& displayCount, unsigned int& drawCount, unsigned int& entityCount) override;

        void RenderUI();
        void RenderMainMenuBar();

    private:
        Engine* engine = nullptr;
        Window_OpenGL* window = nullptr;
        Entity* selectedEntity = nullptr;
    };
} // namespace Vosgi


#endif // !__EDITOR_H__