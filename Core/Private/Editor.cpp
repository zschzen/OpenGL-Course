#include "../Public/Engine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Public/Editor.h"
#include "../Public/Entity.h"

namespace Vosgi
{
    Editor::Editor(Engine *engine) : engine(engine)
    {
    }

    Editor::~Editor()
    {
    }

    void Editor::Run()
    {
        window = new Window_OpenGL(reinterpret_cast<WindowHandle *>(this));
        window->Initialize();
        window->SetMouseEnabled(false);
        window->Run();
    }

    void Editor::DrawMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New"))
                {
                }
                if (ImGui::MenuItem("Open"))
                {
                }
                if (ImGui::MenuItem("Save"))
                {
                }
                if (ImGui::MenuItem("Save As"))
                {
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit"))
                {
                    window->Terminate();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo"))
                {
                }
                if (ImGui::MenuItem("Redo"))
                {
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Cut"))
                {
                }
                if (ImGui::MenuItem("Copy"))
                {
                }
                if (ImGui::MenuItem("Paste"))
                {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Scene"))
                {
                }
                if (ImGui::MenuItem("Game"))
                {
                }
                if (ImGui::MenuItem("Inspector"))
                {
                }
                if (ImGui::MenuItem("Hierarchy"))
                {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void Editor::DrawHierarchy()
    {
        ImGui::Begin("Hierarchy");
        ImGui::Text("Hierarchy");
        ImGui::Separator();
        ImGui::End();
    }

    void Editor::DrawInspector()
    {
        ImGui::Begin("Inspector");
        ImGui::Text("Inspector");
        ImGui::Separator();
        ImGui::End();
    }

    void Editor::Draw(float deltaTime, unsigned int &displayCount, unsigned int &drawCount, unsigned int &entityCount)
    {
        RenderUI();
    }

    void Editor::RenderUI()
    {
        DrawMenuBar();
        DrawHierarchy();
        DrawInspector();
    }

} // namespace Vosgi