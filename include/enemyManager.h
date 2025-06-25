#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <string>
#include "enemy.h"
#include "wordManager.h"
#include "textManager.h"
#include "Player.h"
#include "bullet.h"

class EnemyManager
{
public:
    EnemyManager(int w, int h, WordManager *wm, TextManager *tm);

    void spawnEnemy();
    void update(float dt, Player *player);
    void render(SDL_Renderer *renderer, Player *player);
    void handleTyping(std::string &inputBuffer, Player *player, std::vector<Bullet> &bullets);
    void handleEnemyBulletCollision(std::vector<Bullet> &bullets);
    bool checkCollision(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2)
    {
        return !(x1 + w1 < x2 || x1 > x2 + w2 ||
                 y1 + h1 < y2 || y1 > y2 + h2);
    }

    void startWave(int count, float speed, float delay);
    bool allEnemiesDefeated() const;
    void clear()
    {
        enemies.clear();
        targetedEnemyIndex = -1;
        spawning = false;
        totalEnemiesSpawned = 0;
    }

private:
    std::vector<Enemy> enemies;
    Uint32 lastSpawnTime = 0;
    Uint32 spawnDelay = 1500;

    WordManager *wordManager;
    TextManager *textManager;

    int windowW, windowH;

    int targetedEnemyIndex = -1;

    int enemiesToSpawn = 0;
    float enemySpeed = 50.0f;

    bool spawning = false;
    int totalEnemiesSpawned = 0;
};