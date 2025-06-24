#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "Player.h"
#include "enemy.h"
#include "parallaxLayer.h"
#include "bullet.h"
#include "wordManager.h"
#include "animation.h"
#include "textManager.h"


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


    bool initSDL();
    bool initWindowAndRenderer(const char * title);
    bool loadTextures();


    void handleEnemeyBulletCollison();
    bool showGrid = true;

private:
    int WINDOW_W;
    int WINDOW_H;
    bool isRunning;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    Player *player;

    std::vector<Enemy> enemies;
    Uint32 lastSpawnTime = 0;
    Uint32 spawnDelay = 1500;

    std::vector<ParallaxLayer> bgLayer;
    std::vector<ParallaxLayer> planetLayers;
    Uint32 lastPlanetSpawnTime = 0;
    const Uint32 planetSpawnDelay = 8000;

    std::vector<Bullet> bullets;
    Uint32 lastBulletTime = 0;
    Uint32 bulletDelay = 0;

    WordManager wordManager;
    Animation *anim;
    
    TextManager * textManager;
};
