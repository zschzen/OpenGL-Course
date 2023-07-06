#include "../Public/Engine.h"

namespace Vosgi
{
    Engine::Engine()
    {
    }

    Engine::~Engine()
    {
    }

    void Engine::Run()
    {
        Game *game = new Game();
        game->Run();
        //delete game;
    }
} // namespace Vosgi
