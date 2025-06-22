#include "game.h"
#include "enemy.h"
#include <string>

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
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Log("SDL Init failed: %s", SDL_GetError());
        return false;
    }
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

    player = new Player(64, WINDOW_W / 2 - 32, WINDOW_H - 32 - 100, "player", "boost");
    if(!TextureManager::load("player", "assets/ship_1.png", renderer)){
    }
    TextureManager::load("boost", "assets/turbo_blue.png", renderer);
    
    TextureManager::load("enemy_1", "assets/ship_2.png", renderer);
    TextureManager::load("enemy_2", "assets/ship_3.png", renderer);
    TextureManager::load("enemy_3", "assets/ship_4.png", renderer);
    TextureManager::load("enemy_4", "assets/ship_5.png", renderer);
    TextureManager::load("enemy_5", "assets/ship_6.png", renderer);


    // TextureManager::load("bg1", "assets/layer_1.png", renderer);
    // TextureManager::load("bg2", "assets/layer_2.png", renderer);
    TextureManager::load("bg3", "assets/layer_3.png", renderer);
    TextureManager::load("bg4", "assets/layer_4.png", renderer);

    bgLayer.emplace_back("bg4", 5.0f, WINDOW_W, WINDOW_H);
    bgLayer.emplace_back("bg3", 20.0f, WINDOW_W, WINDOW_H);
    // bgLayer.emplace_back("bg2", 70.0f, WINDOW_W, WINDOW_H);
    // bgLayer.emplace_back("bg1", 100.0f, WINDOW_W, WINDOW_H);
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
        player->handleEvent(event);
    }

    
}

void Game::update(float dt)
{
  if(SDL_GetTicks() - lastSpawnTime > spawnDelay){
    spawnEnemy();
    lastSpawnTime = SDL_GetTicks();

  }

  for(auto & it: enemies){
    it.update(dt);
  }
  // enemies.erase(std::remove_if(enemies.begin(), enemies.end() , [this](const Enemy& e) {return e.isOffScreen(WINDOW_H);}), enemies.end());
 
  for(auto & it: bgLayer){
    it.update(dt);
  }

  player->update(dt);
}

void Game::render()
{
    // Set background color (dark gray to make walls/tiles stand out)
    SDL_SetRenderDrawColor(renderer, 160, 32, 240, 255);
    SDL_RenderClear(renderer);

    for(auto & it: bgLayer){
      it.render(renderer);
    }

    player->render(renderer);
    for(auto & it: enemies){
      it.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::spawnEnemy() {
  std::vector<std::string> textures = {"enemy_1", "enemy_2", "enemy_3", "enemy_4", "enemy_5"};
  std::vector<std::string> words = {"dog", "cat", "ship"};

  int ranTex = rand() % textures.size();
  int randWord =rand() % words.size();
  float randX = rand() % (WINDOW_W - 48);
  float speed = 50.f + rand() % 50;
  int s = rand() % 48 + 32;

  enemies.emplace_back(s, randX, -48.0f, speed, words[randWord], textures[ranTex]);
}
