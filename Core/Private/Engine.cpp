#include "../Public/Engine.h"

#include "../Public/Game.h"
#include "../Public/Editor.h"

namespace Vosgi
{
    Engine::Engine()
    {
    }

    Engine::~Engine()
    {
    }

    void Engine::RunEditor()
    {
        Editor *editor = new Editor(this);
        editor->Run();
        //delete editor;
    }

    void Engine::Run()
    {
        Game *game = new Game();
        game->Run();
        //delete game;
    }
} // namespace Vosgi
