#pragma once
#include <vector>
#include <unordered_set>
#include "enemy.h"
#include "wordManager.h"
#include <SDL3/SDL.h>

class EnemyManager {
public:
    EnemyManager(int windowW, int windowH);

    void update(float dt, float playerX, float playerY);
    void render(SDL_Renderer* renderer, float playerX, float playerY, int targetedEnemyIndex);

    void spawnEnemy(WordManager& wordManager, int minWordLength);

    std::vector<Enemy>& getEnemies();
    void removeDeadAndOffscreen(int& targetedEnemyIndex, std::string& currentTyped);

private:
    std::vector<Enemy> enemies;
    std::vector<std::string> enemyTextures;
    int WINDOW_W, WINDOW_H;
};
