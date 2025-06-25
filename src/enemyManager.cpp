#include "enemyManager.h"

EnemyManager::EnemyManager(int w, int h, WordManager *wm, TextManager *tm)
{
    windowW = w;
    windowH = h;
    wordManager = wm;
    textManager = tm;
}

void EnemyManager::spawnEnemy()
{
    static std::vector<std::string> textures = {"enemy_1", "enemy_2", "enemy_3", "enemy_4", "enemy_5"};

    // Step 1: Collect used first letters
    std::unordered_set<char> usedLetters;
    for (const auto &e : enemies)
    {
        if (!e.getWord().empty())
            usedLetters.insert(std::tolower(e.getWord()[0]));
    }

    // Step 2: Get a new word avoiding used letters
    std::string word = wordManager->getWordAvoiding(usedLetters, 8);
    if (word.empty())
    {
        SDL_Log("No valid word with unique starting letter available!");
        return;
    }

    SDL_Log("Spawning enemy with word: %s", word.c_str());

    std::string texId = textures[rand() % textures.size()];
    float randX = rand() % (windowW - 48);
    float speed = enemySpeed + rand() % 20 - 10; // variation
    int s = rand() % 40 + 40;

    enemies.emplace_back(s, randX, -48.0f, speed, word, texId);
}

void EnemyManager::update(float dt, Player *player)
{
    if (spawning && totalEnemiesSpawned < enemiesToSpawn)
    {
        if (SDL_GetTicks() - lastSpawnTime >= spawnDelay)
        {
            spawnEnemy();
            totalEnemiesSpawned++;
            lastSpawnTime = SDL_GetTicks();
        }
    }

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if (it->isDead())
        {
            if (it->isDeathAnimFinished())
            {
                int index = std::distance(enemies.begin(), it);

                if (index == targetedEnemyIndex)
                {
                    targetedEnemyIndex = -1;
                    enemies[index].isTargeted = false;
                }
                else if (index < targetedEnemyIndex)
                {
                    targetedEnemyIndex--;
                }
                it = enemies.erase(it);
                continue;
            }
        }
        else if (it->isOffScreen(windowH))
        {
            int index = std::distance(enemies.begin(), it);

            if (index == targetedEnemyIndex)
            {
                targetedEnemyIndex = -1;
                enemies[index].isTargeted = false;
            }
            else if (index < targetedEnemyIndex)
            {
                targetedEnemyIndex--;
            }

            it = enemies.erase(it);
            continue;
        }
        ++it;
    }

    for (auto &it : enemies)
    {
        it.update(dt, player->getX(), player->getY());
    }

    if (targetedEnemyIndex >= 0 && targetedEnemyIndex < enemies.size())
    {
        auto &enemy = enemies[targetedEnemyIndex];
        player->lookAt(enemy.getX() + enemy.getSize() / 2, enemy.getY() + enemy.getSize() / 2);
    }
}

void EnemyManager::render(SDL_Renderer *renderer, Player *player)
{
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        auto &it = enemies[i];
        it.render(renderer, player->getX(), player->getY());

        if (i == targetedEnemyIndex)
        {
            textManager->renderText(it.getWord(),
                                    it.getX() + it.getSize() / 2.0f,
                                    it.getY() - 20,
                                    {255, 0, 0, 255}, // Red color for targeted enemy
                                    {0, 0, 0, 128},
                                    true);
        }
        else
        {
            textManager->renderText(it.getWord(),
                                    it.getX() + it.getSize() / 2.0f,
                                    it.getY() - 20,
                                    {255, 255, 255, 255}, // White color for normal enemies
                                    {0, 0, 0, 128},
                                    true);
        }

        if (it.lockAnim && !it.lockAnim->isFinished())
        {
            SDL_FRect src = it.lockAnim->getSrcRect();
            SDL_FRect dst = {
                it.getX() + it.getSize() / 2 - 32,
                it.getY() + it.getSize() / 2 - 32,
                64, 64};
            SDL_FPoint center = {32, 32};
            TextureManager::drawRotated("lock", dst, renderer, &src, it.lockAnimTime * 100.0f, &center);
        }
    }
}

void EnemyManager::handleTyping(std::string &inputBuffer, Player *player, std::vector<Bullet> &bullets)
{

    for (char inputChar : inputBuffer)
    {
        if (targetedEnemyIndex == -1)
        {
            for (size_t i = 0; i < enemies.size(); ++i)
            {
                if (inputChar == enemies[i].getWord()[0])
                {
                    targetedEnemyIndex = i;
                    enemies[i].isTargeted = true;
                    if (!enemies[i].lockAnim)
                    {
                        enemies[i].lockAnim = new Animation(32, 32, 4, 100, true, 4); // frames: 1-4
                    }

                    player->lookAt(enemies[i].getX() + enemies[i].getSize() / 2, enemies[i].getY() + enemies[i].getSize() / 2);
                    player->shoot(bullets);
                    enemies[i].getHit();
                    break;
                }
            }
        }
        else
        {
            auto &it = enemies[targetedEnemyIndex];
            if (inputChar == it.getWord()[it.getWord().size() - it.health])
            {
                player->shoot(bullets);
                enemies[targetedEnemyIndex].getHit();
            }
            if (enemies[targetedEnemyIndex].health == 0)
            {
                targetedEnemyIndex = -1;
            }
        }
    }
    inputBuffer.clear(); // Don't forget to clear it after processing
}

void EnemyManager::handleEnemyBulletCollision(std::vector<Bullet> &bullets)
{
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();)
    {
        bool bulletRemoved = false;

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
        {
            float bx = bulletIt->getX();
            float by = bulletIt->getY();
            float bw = bulletIt->getWidth(); // or size
            float bh = bulletIt->getHeight();

            float ex = enemyIt->getX();
            float ey = enemyIt->getY();
            float ew = enemyIt->getSize();
            float eh = enemyIt->getSize();

            if (checkCollision(bx, by, bw, bh, ex, ey, ew, eh))
            {
                int enemyIndex = std::distance(enemies.begin(), enemyIt);

                // Remove bullet
                bulletIt = bullets.erase(bulletIt);
                bulletRemoved = true;

                // Reset lock-on if this was the target
                // if (enemyIndex == targetedEnemyIndex)
                // {
                //     targetedEnemyIndex = -1;
                //     currentTyped.clear();
                // }
                // else if (enemyIndex < targetedEnemyIndex)
                // {
                //     targetedEnemyIndex--; // adjust index
                // }

                // Remove enemy
                // enemyIt = enemies.erase(enemyIt);

                // enemies[enemyIndex].getHit();
                break; // one bullet can hit only one enemy
            }
            else
            {
                ++enemyIt;
            }
        }

        if (!bulletRemoved)
            ++bulletIt;
    }
}

void EnemyManager::startWave(int count, float speed, float delay)
{
    enemiesToSpawn = count;
    enemySpeed = speed;
    spawnDelay = delay;
    totalEnemiesSpawned = 0;
    spawning = true;
    lastSpawnTime = SDL_GetTicks();
}

bool EnemyManager::allEnemiesDefeated() const
{
    return spawning && totalEnemiesSpawned >= enemiesToSpawn && enemies.empty();
}