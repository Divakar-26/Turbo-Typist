#include "textManager.h"
#include <SDL3/SDL_log.h>

TextManager::TextManager(SDL_Renderer *renderer)
{
    this->renderer = renderer;
}

TextManager::~TextManager()
{
    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

bool TextManager::loadFont(const std::string &path, int fontSize)
{
    font = TTF_OpenFont(path.c_str(), fontSize);
    if (!font)
    {
        SDL_Log("Failed to load font : %s", SDL_GetError());
        return false;
    }
    return true;
}

void TextManager::renderText(const std::string &text, int x, int y, SDL_Color textColor, SDL_Color bgColor, bool center)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!font || !renderer)
        return;

    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), text.size(), textColor);
    if (!surface)
    {
        SDL_Log("Failed to render text: %s", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        SDL_Log("Failed to create texture from surface");
        return;
    }

        SDL_FRect destRect = {(float)x, (float)y, (float)surface->w, (float)surface->h};

    if (center) {
        destRect.x -= surface->w / 2;
        destRect.y -= surface->h / 2;
    }

    SDL_FRect bgRect = {
        (float)destRect.x - 4,
        (float)destRect.y - 2,
        (float)destRect.w + 8,
        (float)destRect.h + 4
    };

    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &bgRect);

    SDL_RenderTexture(renderer, texture, nullptr, &destRect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);

}
