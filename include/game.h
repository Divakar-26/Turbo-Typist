#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>

#include "Player.h"
#include "parallaxLayer.h"
#include "bullet.h"
#include "wordManager.h"
#include "animation.h"
#include "textManager.h"
#include "enemyManager.h"
#include "UIButton.h"
#include "UILabel.h"
#include "TextureObject.h"

enum class GameState
{
    MAIN_MENU,
    PLAYING,
    PAUSED,
    LEVEL_CLEARED,
    GAME_OVER
};

struct Level
{
    int levelNumber;
    int numEnemies;
    int wordLengthMin;
    int wordLengthMax;
    float enemySpeed;
    float spawnDelay; 

    Level(int lvl, int count, float speed, float delay, int minLen, int maxLen)
        : levelNumber(lvl), numEnemies(count), enemySpeed(speed), spawnDelay(delay),
          wordLengthMin(minLen), wordLengthMax(maxLen) {}
};


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
    bool initWindowAndRenderer(const char *title);
    bool loadTextures();

private:
    int WINDOW_W;
    int WINDOW_H;
    bool isRunning;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    Player *player;

    std::vector<ParallaxLayer> bgLayer;

    std::vector<Bullet> bullets;
    Uint32 lastBulletTime = 0;
    Uint32 bulletDelay = 0;

    WordManager wordManager;
    TextManager *textManager;
    TextManager *uiTextManager;

    Animation *planetAnim;
    GameState gameState = GameState::MAIN_MENU;

    EnemyManager *enemyManager = nullptr;

    UIButton *startButton = nullptr;
    UIButton *exitButton = nullptr;
    UIButton *settingsButton = nullptr;
    UIButton *resumeButton = nullptr;
    UIButton *quitToMainMenu = nullptr;
    UIButton *quitButton = nullptr;
    UILabel *label = nullptr;

    TextureObject *logoTex;
    // UILabel *levelLabel = nullptr;

    int currentLevelIndex = 0;
    float levelTimer = 0.0f;
    bool waitingForNextLevel = false;

    std::vector<Level> levels;
    UILabel *levelLabel;
};
