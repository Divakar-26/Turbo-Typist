#pragma once

#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>

class Enemy{  
  public:

    Enemy(int size, float x, float y, float speed,const std::string & word, const std::string&textureId);
    
    void update(float dt);
    void render(SDL_Renderer * renderer, float playerX, float playerY);

    float getY() const {return y;}
    bool isOffScreen(int screenHeight) const;
    const std::string & getWord() const {return word;}


  private:
    float x, y;
    float speed;
    int size;

    bool isDead  = false;

    std::string word; 
    std::string textureId;
};  
