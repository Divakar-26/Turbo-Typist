#pragma once
#include <SDL3/SDL.h>
#include <string>

class TextureObject {
public:
    std::string id;
    SDL_FRect dst;
    SDL_FRect* src = nullptr;
    float opacity = 255.0f;

    float targetX, targetY;
    bool fadingIn = false;
    bool fadingOut = false;
    float fadeSpeed = 300.0f; // per second
    float moveSpeed = 300.0f; // per second

    TextureObject(const std::string& id, float x, float y, float w, float h);

    void moveTo(float x, float y);
    void fadeIn();
    void fadeOut();

    void update(float dt);
    void render(SDL_Renderer* renderer);
};
