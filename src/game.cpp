#include "game.h"
#include "enemy.h"
#include <string>
#include <iostream>

Game::Game(int width, int height)
    : WINDOW_W(width), WINDOW_H(height), isRunning(false) {}

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
    if(!loadTextures()) return false;

    //loading word from words.txt
    wordManager.loadFromFile("words.txt");

    
    //parallax layers
    anim = new Animation(100, 100, 400, 80, true, 20);
    bgLayer.emplace_back("bg4", 5.0f, WINDOW_W, WINDOW_H);
    bgLayer.emplace_back("planet", 1.0f, WINDOW_W, WINDOW_H, anim, 200, 300, 400, 400);
    bgLayer.emplace_back("bg3", 15.0f, WINDOW_W, WINDOW_H);
    
    //player Definition
    player = new Player(64, WINDOW_W / 2 - 32, WINDOW_H - 32 - 100, "player", "boost");

    //textManager and loading font
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
                player->shoot(bullets);
                lastBulletTime = now;
            }
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            float x, y;
            SDL_GetMouseState(&x, &y);
            for (auto it : enemies)
            {
                if (x >= it.getX() && x <= it.getX() + it.getSize() && y >= it.getY() && y <= it.getY() + it.getSize())
                {
                    std::cout << it.getWord() << std::endl;
                }
            }
        }

        // player->handleEvent(event);
    }
}

void Game::update(float dt)
{
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
        if (it->isOffScreen(WINDOW_H))
        {
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
    float x, y;
    SDL_GetMouseState(&x, &y);
    player->lookAt(x, y);
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

    for (auto &it : enemies)
    {
        it.render(renderer, player->getX(), player->getY());
        textManager->renderText(it.getWord(), it.getX() + it.getSize() / 2.0f, it.getY() - 20, {255, 255, 255, 255}, {0, 0, 0, 128}, true);
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


    std::string word = wordManager.getUniqueWord(5);
    if (word.empty())
    {
        SDL_Log("No more unique words available!");
        return;
    }

    SDL_Log("Spawning enemy with word: %s", word.c_str());

    std::string texId = textures[rand() % textures.size()];
        float randX = rand() % (WINDOW_W - 48);
    float speed = 20.f + rand() % 50;
    int s = rand() % 48 + 32;

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

bool Game::loadTextures() {
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
    };

    for (const auto& [id, path] : textures) {
        if (!TextureManager::load(id, path, renderer)) {
            SDL_Log("Failed to load texture: %s", path.c_str());
            return false;
        }
    }
    return true;
}
