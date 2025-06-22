#include "parallaxLayer.h"
#include "textureManager.h"
#include <SDL3/SDL_rect.h>

ParallaxLayer::ParallaxLayer(const std::string & textureId, float scrollSpeed, int winW, int winH): texId(textureId), scrollSpeed(scrollSpeed), winWidth(winW), winHeight(winH) {}

void ParallaxLayer::update(float dt) {
    offsetY += scrollSpeed * dt;
    if (offsetY >= winHeight)
        offsetY = 0;
}


void ParallaxLayer::render(SDL_Renderer * renderer) {
    SDL_FRect dest1 = {0, offsetY, (float)winWidth, (float)winHeight};
    SDL_FRect dest2 = {0, offsetY - winHeight, (float)winWidth, (float)winHeight};

    TextureManager::draw(texId, dest1, renderer);
    TextureManager::draw(texId, dest2, renderer);
}
