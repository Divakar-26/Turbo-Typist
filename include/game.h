#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include<vector>
#include"Player.h"
#include "enemy.h"
#include "parallaxLayer.h"
#include <cstdlib>

class Game
{
public:
    Game(int width, int height);
    ~Game();

    bool init(const char *title);
    void handleEvents();
    void render();
    void update(float dt);

    bool running() const { return isRunning; }
    void clean();

    void spawnEnemy();

    bool showGrid = true;
private:
    int WINDOW_W;
    int WINDOW_H;
    bool isRunning;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    Player * player;

    std::vector<Enemy> enemies;
    Uint32 lastSpawnTime = 0;
    Uint32 spawnDelay = 1500;

    std::vector<ParallaxLayer> bgLayer;
};
