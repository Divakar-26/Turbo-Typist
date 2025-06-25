#pragma once
#include <SDL3/SDL.h>

class UIElement {
protected:
    float x, y;
    float targetX, targetY;
    float opacity = 255;
    float speed = 300.0f;
    bool fadingOut = false;
    bool fadingIn = false;
    float fadeSpeed = 300.0f; 

public:
    UIElement(float x, float y);

    virtual void update(float dt);
    virtual void render(SDL_Renderer* renderer) = 0;

    void moveTo(float newX, float newY);
    void fadeIn();
    void fadeOut();

    float getOpacity() const { return opacity; }
    virtual ~UIElement() = default;
};
