#ifndef PLAYER_H
#define PLAYER_H

#include<SDL3/SDL.h>
#include <cwchar>
#include <string>
#include <vector>
#include "textureManager.h"
#include "animation.h"
#include "bullet.h"

class Player{
  public:
    Player(int size, float x, float y, const std::string &texId, const std::string & thId);
    ~Player();

    void render(SDL_Renderer * renderer);
    void handleEvent(SDL_Event & event);
    void update(float dt);
    float getX() {return x;}
    float getY() {return y;}
    void shoot(std::vector<Bullet> & bullets);
    void lookAt(float x, float y);

  private:
    float x,y;
    int size;
    std::string textureId;
    std::string thrusterTexId;
    Animation * anim;
};

#endif // !DEBUG
