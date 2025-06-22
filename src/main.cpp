#include "game.h"

Game game(800, 600);

int main(int argc, char *argv[])
{
    if (!game.init("Verlet Simulation"))
    {
        return -1;
    }
    Uint64 now = SDL_GetTicks();
    Uint64 last = now;

    while (game.running())
    {
        now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f; // convert to seconds
        last = now;

        game.handleEvents();
        game.update(dt);
        game.render();

    }

    game.clean();

    return 0;
}
