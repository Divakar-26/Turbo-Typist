#pragma once
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include <string>

class TextManager{


    public:
    TextManager(SDL_Renderer * renderer);
    ~TextManager();
    
    bool loadFont(const std::string &path, int fontSize);
    void renderText(const std::string &text, int x, int y, SDL_Color textColor = {255,255,255,255}, SDL_Color bgColor = {50,50,50,128}, bool center = false);
    
    private:
    SDL_Renderer * renderer = nullptr;
    TTF_Font * font = nullptr;

};