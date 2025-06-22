#include "parallaxLayer.h"
#include "textureManager.h"
#include <SDL3/SDL_rect.h>

ParallaxLayer::ParallaxLayer(const std::string &textureId, float scrollSpeed, int winW, int winH) : texId(textureId), scrollSpeed(scrollSpeed), winWidth(winW), winHeight(winH), isAnimated(false),anim(nullptr) {}
ParallaxLayer::ParallaxLayer(const std::string &textureId, float scrollSpeed, int winW, int winH, Animation *animation,float x, float y, float w, float h ) : texId(textureId), scrollSpeed(scrollSpeed), winWidth(winW), winHeight(winH), isAnimated(true), anim(animation) , objX(x), objY(y), objW(w), objH(h) {}

void ParallaxLayer::update(float dt)
{
    offsetY += scrollSpeed * dt;
    if (offsetY >= winHeight)
        offsetY = 0;

    if (isAnimated && anim)
        anim->update();
}

void ParallaxLayer::render(SDL_Renderer *renderer) {
    if (isAnimated && anim) {
        SDL_FRect dst = { objX, objY + offsetY, objW, objH };
        SDL_FRect src = anim->getSrcRect();
        TextureManager::draw(texId, dst, renderer, &src);
    } else {
        // Tiled scrolling background
        SDL_FRect dst1 = { 0, offsetY, (float)winWidth, (float)winHeight };
        SDL_FRect dst2 = { 0, offsetY - winHeight, (float)winWidth, (float)winHeight };

        TextureManager::draw(texId, dst1, renderer, nullptr);
        TextureManager::draw(texId, dst2, renderer, nullptr);  // Seamless loop
    }
}