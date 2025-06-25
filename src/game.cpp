#include "game.h"

std::string inputBuffer;


Game::Game(int width, int height)
    : WINDOW_W(width), WINDOW_H(height), isRunning(false) {}

float logoW, logoH;
Game::~Game()
{
    delete startButton;
    startButton = nullptr;

    delete enemyManager;
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
    planetAnim = new Animation(100, 100, 400, 80, true, 20);
    bgLayer.emplace_back("bg4", 20.0f, WINDOW_W, WINDOW_H);
    // bgLayer.emplace_back("planet", 40.0f, WINDOW_W, WINDOW_H, planetAnim, 200, 300, 300, 300);

    // player Definition
    // player = new Player(64, WINDOW_W / 2 - 32, WINDOW_H - 32 - 100, "player", "boost");
    player = new Player(64, WINDOW_W / 2 - 32, WINDOW_H / 2, "player", "boost");

    // textManager and loading font
    textManager = new TextManager(renderer);
    textManager->loadFont("assets/sans.ttf", 20);

    uiTextManager = new TextManager(renderer);
    uiTextManager->loadFont("assets/mago1.ttf", 40);

    enemyManager = new EnemyManager(WINDOW_W, WINDOW_H, &wordManager, textManager);

    startButton = new UIButton(400, WINDOW_H - 150, "Start Game", {255, 255, 255, 255}, {255, 200, 0, 255}, uiTextManager);
    exitButton = new UIButton(400, WINDOW_H - 100, "Exit", {255, 255, 255, 255}, {255, 200, 0, 255}, uiTextManager);
    settingsButton = new UIButton(400, WINDOW_H - 50, "Settings", {255, 255, 255, 255}, {255, 200, 0, 255}, uiTextManager);

    startButton->moveTo(400 - startButton->getWidth() / 2, WINDOW_H - 150);
    exitButton->moveTo(400 - exitButton->getWidth() / 2, WINDOW_H - 100);
    settingsButton->moveTo(400 - settingsButton->getWidth() / 2, WINDOW_H - 50);
    startButton->fadeIn();

    resumeButton = new UIButton(400, 200, "Resume", {255, 255, 255, 255}, {200, 255, 200, 255}, uiTextManager);
    quitToMainMenu = new UIButton(400, 260, "Main Menu", {255, 255, 255, 255}, {255, 200, 200, 255}, uiTextManager);
    quitButton = new UIButton(400, 320, "Quit", {255, 255, 255, 255}, {255, 100, 100, 255}, uiTextManager);

    resumeButton->moveTo(400 - resumeButton->getWidth() / 2, 200);
    quitToMainMenu->moveTo(400 - quitToMainMenu->getWidth() / 2, 260);
    quitButton->moveTo(400 - quitButton->getWidth() / 2, 320);

    logoW = 400.0f;
    logoH = 400.0f;
    logoTex = new TextureObject("logo", WINDOW_W / 2 - logoW / 2, -16 * 4, logoW, logoH);

    float initialSpeed = 40.0f;
    float initialDelay = 2000.0f;
    int initialEnemies = 5;

    for (int i = 1; i <= 9999; ++i) // Endless (or a large number)
    {
        float speed = initialSpeed + (i - 1) * 2.0f;                     // Increase slowly
        float delay = std::max(500.0f, initialDelay - (i - 1) * 100.0f); // Never below 500ms
        int numEnemies = initialEnemies + (i - 1) * 2;

        int minLength = std::min(5, 3 + i / 2);
        int maxLength = std::min(15, 5 + i / 2);

        levels.push_back({i, numEnemies, speed, delay, minLength, maxLength});
    }
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

        if (event.type == SDL_EVENT_TEXT_INPUT && gameState == GameState::PLAYING)
        {
            inputBuffer += event.text.text;
            SDL_Log("Typed character: %c", inputBuffer[0]);

            enemyManager->handleTyping(inputBuffer, player, bullets);
        }

        if (gameState == GameState::MAIN_MENU)
        {

            startButton->handleEvent(event);
            exitButton->handleEvent(event);
            settingsButton->handleEvent(event);
            if (startButton->isClicked(event))
            {
                player->moveTo(WINDOW_W / 2 - 32, WINDOW_H - 32 - 100);
                logoTex->moveTo(200, -500);
                logoTex->fadeOut();
                exitButton->moveTo(400 - exitButton->getWidth() / 2, WINDOW_H + 150);
                startButton->moveTo(400 - startButton->getWidth() / 2, WINDOW_H + 200);
                settingsButton->moveTo(400 - settingsButton->getWidth() / 2, WINDOW_H + 250);

                const Level &level = levels[currentLevelIndex];
                enemyManager->clear(); // ensure no previous garbage
                bullets.clear();       // clear bullets
                enemyManager->startWave(level.numEnemies, level.enemySpeed, level.spawnDelay);


                gameState = GameState::PLAYING;
                // Trigger your logic here, like changing game state
            }
            else if (exitButton->isClicked(event))
            {
                isRunning = false;
            }
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
        {
            if (gameState == GameState::PLAYING)
                gameState = GameState::PAUSED;
            else if (gameState == GameState::PAUSED)
                gameState = GameState::PLAYING;
        }
        if (gameState == GameState::PAUSED)
        {
            resumeButton->handleEvent(event);
            quitToMainMenu->handleEvent(event);
            quitButton->handleEvent(event);

            if (resumeButton->isClicked(event))
            {
                gameState = GameState::PLAYING;
            }

            if (quitToMainMenu->isClicked(event))
            {
                gameState = GameState::MAIN_MENU;
                // Reset positions or reinit as needed
                startButton->moveTo(400 - startButton->getWidth() / 2, WINDOW_H - 150);
                exitButton->moveTo(400 - exitButton->getWidth() / 2, WINDOW_H - 100);
                settingsButton->moveTo(400 - settingsButton->getWidth() / 2, WINDOW_H - 50);
                startButton->fadeIn();
                logoTex->moveTo(WINDOW_W / 2 - logoW / 2, -16 * 4);
                logoTex->fadeIn();
                player->moveTo(WINDOW_W / 2 - 32, WINDOW_H / 2);
                player->lookAt(WINDOW_W / 2, 0);
            }

            if (quitButton->isClicked(event))
            {
                isRunning = false;
            }
        }

        // player->handleEvent(event);
    }
}

void Game::update(float dt)
{
    for (auto &it : bgLayer)
        it.update(dt);

    if (gameState == GameState::PLAYING)
    {
        enemyManager->update(dt, player);
        enemyManager->handleEnemyBulletCollision(bullets);
        for (auto &bullet : bullets)
            bullet.update(dt);
        player->update(dt);

        if (gameState == GameState::PLAYING)
        {
            enemyManager->update(dt, player);
            enemyManager->handleEnemyBulletCollision(bullets);

            for (auto &bullet : bullets)
                bullet.update(dt);

            player->update(dt);

            if (enemyManager->allEnemiesDefeated())
            {
                gameState = GameState::LEVEL_CLEARED;
                levelTimer = 0.0f;
            }
        }
    }

    if (gameState == GameState::LEVEL_CLEARED)
    {
        levelTimer += dt;

        if (levelTimer > 3.0f) // wait 3 seconds before next level
        {
            currentLevelIndex++;

            if (currentLevelIndex >= levels.size())
            {
                levelLabel->setText("Game Complete!");
                levelLabel->fadeIn();
                gameState = GameState::MAIN_MENU;
            }
            else
            {
                const Level &level = levels[currentLevelIndex];

                enemyManager->clear();
                bullets.clear();

                enemyManager->startWave(level.numEnemies, level.enemySpeed, level.spawnDelay);

                levelTimer = 0.0f;
                gameState = GameState::PLAYING;
            }
        }
    }
    // Only update player animation on menu (if needed)
    if (gameState == GameState::MAIN_MENU)
    {
        player->update(dt);
    }

    if (gameState == GameState::PAUSED)
    {
        resumeButton->update(dt);
        quitToMainMenu->update(dt);
        quitButton->update(dt);
        // player->update(dt);  <- Avoid this if it’s gameplay logic
    }

    // UI elements update regardless of state
    startButton->update(dt);
    exitButton->update(dt);
    settingsButton->update(dt);

    logoTex->update(dt);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 160, 32, 240, 255);
    SDL_RenderClear(renderer);

    for (auto &it : bgLayer)
    {
        it.render(renderer);
    }

    startButton->render(renderer);
    exitButton->render(renderer);
    settingsButton->render(renderer);
    logoTex->render(renderer);
    player->render(renderer);

    if (gameState == GameState::PAUSED)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160); // RGBA (black with 160 alpha)
        SDL_FRect pauseBox = {
            WINDOW_W / 2 - 250.0f,
            WINDOW_H / 2 - 150.0f,
            500.0f,
            300.0f};
        SDL_RenderFillRect(renderer, &pauseBox);
        enemyManager->render(renderer, player);
        for (auto &bullet : bullets)
            bullet.render(renderer);
        resumeButton->render(renderer);
        quitToMainMenu->render(renderer);
        quitButton->render(renderer);
    }


    if (gameState == GameState::PLAYING)
    {
        enemyManager->render(renderer, player);
        for (auto &bullet : bullets)
            bullet.render(renderer);

    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
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
        {"lock", "assets/lock_1.png"},
        {"logo", "assets/turboTypist.png"}};

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
