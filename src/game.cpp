#include "game.h"
#include "enemy.h"
#include <string>
#include <iostream>
#include <unordered_set>

std::string currentTyped;
int targetedEnemyIndex = -1;
std::string inputBuffer;

bool checkCollision(float x1, float y1, float w1, float h1,
                    float x2, float y2, float w2, float h2)
{
    return !(x1 + w1 < x2 || x1 > x2 + w2 ||
             y1 + h1 < y2 || y1 > y2 + h2);
}

Game::Game(int width, int height)
    : WINDOW_W(width), WINDOW_H(height), isRunning(false),enemyManager(width, height) {}

Game::~Game()
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    clean();
}

bool Game::init(const char *title)
{
    if (!initSDL())
        return false;
    if (!initWindowAndRenderer(title))
    {
        return false;
    }
    if (!loadTextures())
        return false;
    SDL_StartTextInput(window);
    // loading word from words.txt
    wordManager.loadFromFile("words.txt");

    // parallax layers
    anim = new Animation(100, 100, 400, 80, true, 20);
    bgLayer.emplace_back("bg4", 20.0f, WINDOW_W, WINDOW_H);
    bgLayer.emplace_back("planet", 40.0f, WINDOW_W, WINDOW_H, anim, 200, 300, 300, 300);
    // bgLayer.emplace_back("bg3", 60.0f, WINDOW_W, WINDOW_H);

    // player Definition
    player = new Player(64, WINDOW_W / 2 - 32, WINDOW_H - 32 - 100, "player", "boost");

    // textManager and loading font
    textManager = new TextManager(renderer);
    textManager->loadFont("assets/sans.ttf", 20);

    isRunning = true;
    return true;
}

void Game::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            isRunning = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            Uint32 now = SDL_GetTicks();
            if (now - lastBulletTime > bulletDelay)
            {
                // player->shoot(bullets);
                lastBulletTime = now;
            }
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            float x, y;
            SDL_GetMouseState(&x, &y);
            for (auto &it : enemies)
            {
                if (x >= it.getX() && x <= it.getX() + it.getSize() && y >= it.getY() && y <= it.getY() + it.getSize())
                {
                    std::cout << it.getWord() << std::endl;
                    std::cout << it.health << std::endl;
                }
            }
        }

        if (event.type == SDL_EVENT_TEXT_INPUT)
        {
            inputBuffer += event.text.text;
            printf("Typed character: %c\n", inputBuffer[0]);

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

        // player->handleEvent(event);
    }
}

void Game::update(float dt)
{
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
                    currentTyped.clear();
                }
                else if (index < targetedEnemyIndex)
                {
                    targetedEnemyIndex--;
                }
                it = enemies.erase(it);
                continue;
            }
        }
        else if (it->isOffScreen(WINDOW_H))
        {
            int index = std::distance(enemies.begin(), it);

            if (index == targetedEnemyIndex)
            {
                targetedEnemyIndex = -1;
                enemies[index].isTargeted = false;
                currentTyped.clear();
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

    handleEnemeyBulletCollison();

    if (SDL_GetTicks() - lastSpawnTime > spawnDelay)
    {
        spawnEnemy();
        lastSpawnTime = SDL_GetTicks();
    }

    for (auto &it : enemies)
    {
        it.update(dt, player->getX(), player->getY());
    }

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        int index = std::distance(enemies.begin(), it);
        if (it->isOffScreen(WINDOW_H))
        {
            if (index == targetedEnemyIndex)
            {
                targetedEnemyIndex = -1;
                currentTyped.clear();
            }
            else if (index < targetedEnemyIndex)
            {
                targetedEnemyIndex--;
            }

            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto &it : bgLayer)
    {
        it.update(dt);
    }
    for (auto &bullet : bullets)
        bullet.update(dt);

    player->update(dt);

    // float x, y;
    // SDL_GetMouseState(&x, &y);
    if (targetedEnemyIndex >= 0 && targetedEnemyIndex < enemies.size())
    {
        auto &enemy = enemies[targetedEnemyIndex];
        player->lookAt(enemy.getX() + enemy.getSize() / 2, enemy.getY() + enemy.getSize() / 2);
    }
}
void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 160, 32, 240, 255);
    SDL_RenderClear(renderer);

    for (auto &it : bgLayer)
    {
        it.render(renderer);
    }

    player->render(renderer);

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

    for (auto &bullet : bullets)
        bullet.render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::spawnEnemy()
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
    std::string word = wordManager.getWordAvoiding(usedLetters, 8);
    if (word.empty())
    {
        SDL_Log("No valid word with unique starting letter available!");
        return;
    }

    SDL_Log("Spawning enemy with word: %s", word.c_str());

    std::string texId = textures[rand() % textures.size()];
    float randX = rand() % (WINDOW_W - 48);
    float speed = 20.f + rand() % 50;
    int s = rand() % 40 + 40;

    enemies.emplace_back(s, randX, -48.0f, speed, word, texId);
}

bool Game::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Log("SDL Init failed: %s", SDL_GetError());
        return false;
    }
    if (!TTF_Init())
    {
        SDL_Log("SDL_TTF Init failed : %s", SDL_GetError());
        return false;
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    return true;
}

bool Game::initWindowAndRenderer(const char *title)
{
    window = SDL_CreateWindow(title, WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool Game::loadTextures()
{
    std::vector<std::pair<std::string, std::string>> textures = {
        {"player", "assets/ship_1.png"},
        {"boost", "assets/turbo_blue.png"},
        {"enemy_1", "assets/ship_2.png"},
        {"enemy_2", "assets/ship_3.png"},
        {"enemy_3", "assets/ship_4.png"},
        {"enemy_4", "assets/ship_5.png"},
        {"enemy_5", "assets/ship_6.png"},
        {"bullet", "assets/shot.png"},
        {"bg3", "assets/layer_3.png"},
        {"bg4", "assets/layer_4.png"},
        {"textbox_left", "assets/left_cap.png"},
        {"textbox_middle", "assets/middle_cap.png"},
        {"textbox_right", "assets/right_cap.png"},
        {"planet", "assets/planets/planet_1.png"},
        {"explosion", "assets/explode.png"},
        {"lock", "assets/lock_1.png"}};

    for (const auto &[id, path] : textures)
    {
        if (!TextureManager::load(id, path, renderer))
        {
            SDL_Log("Failed to load texture: %s", path.c_str());
            return false;
        }
    }
    return true;
}

void Game::handleEnemeyBulletCollison()
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
