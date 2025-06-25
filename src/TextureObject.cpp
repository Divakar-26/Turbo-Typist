#include "TextureObject.h"
#include "textureManager.h"
#include <cmath>

TextureObject::TextureObject(const std::string& id, float x, float y, float w, float h)
    : id(id), dst{x, y, w, h}, targetX(x), targetY(y) {}

void TextureObject::moveTo(float x, float y) {
    targetX = x;
    targetY = y;
}

void TextureObject::fadeIn() {
    fadingIn = true;
    fadingOut = false;
}

void TextureObject::fadeOut() {
    fadingOut = true;
    fadingIn = false;
}

void TextureObject::update(float dt) {
    // Movement
    float dx = targetX - dst.x;
    float dy = targetY - dst.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance > 1.0f) {
        float dirX = dx / distance;
        float dirY = dy / distance;
        dst.x += dirX * moveSpeed * dt;
        dst.y += dirY * moveSpeed * dt;

        // Clamp if overshoot
        if (std::abs(dst.x - targetX) < 1.0f) dst.x = targetX;
        if (std::abs(dst.y - targetY) < 1.0f) dst.y = targetY;
    }

    // Fade
    if (fadingIn && opacity < 255.0f) {
        opacity += fadeSpeed * dt;
        if (opacity >= 255.0f) {
            opacity = 255.0f;
            fadingIn = false;
        }
    }

    if (fadingOut && opacity > 0.0f) {
        opacity -= fadeSpeed * dt;
        if (opacity <= 0.0f) {
            opacity = 0.0f;
            fadingOut = false;
        }
    }
}

void TextureObject::render(SDL_Renderer* renderer) {
    SDL_SetTextureAlphaMod(TextureManager::get(id), static_cast<Uint8>(opacity));
    TextureManager::draw(id, dst, renderer, src);
}
