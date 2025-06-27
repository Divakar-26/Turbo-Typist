#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <cwchar>
#include <string>
#include <vector>
#include "textureManager.h"
#include "animation.h"
#include "bullet.h"

class Player
{
public:
  Player(int size, float x, float y, const std::string &texId, const std::string &thId);
  ~Player();

  void render(SDL_Renderer *renderer);
  void handleEvent(SDL_Event &event);
  void update(float dt);
  float getX() { return x; }
  float getY() { return y; }
  void shoot(std::vector<Bullet> &bullets);
  void lookAt(float x, float y);

  void moveTo(float x, float y);
  SDL_FRect getBounds() const
  {
    return SDL_FRect{x, y, (float)size, (float)size};
  }

  void getHit(int n){
    if(health - n < 0){
      return ;
    }
    health -= n;
  }

private:

  float x, y;
  int size;
  std::string textureId;
  std::string thrusterTexId;
  Animation *anim;

  int health = 5;

  float hoverTime = 0.0f;
  float hoverOffset = 0.0f;

  float targetX, targetY;
  float moveSpeed = 100.0f;

  std::string healtbarTexture = "healthbar";
  SDL_FRect healthbarDest = {800 / 2 - 16*3*3 /2  , 600 - 16*3, 16*3*3, 16*3};
};

#endif // !DEBUG