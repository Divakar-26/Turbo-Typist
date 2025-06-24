#pragma once
#include <SDL3/SDL.h>
#include <sstream>
#include <string>
#include"animation.h"

class Bullet{
public:
  Bullet(float x, float y, float vx, float vy, float speed, int size, const std::string &id);

  void update(float dt);
  void render(SDL_Renderer * renderer);
  bool isOffScreen(int windowW, int windowH);

  float getWidth() const {return size;}
  float getHeight() const {return size;}
  float getX() const {return x;}
  float getY() const {return y;}

private:
  float x, y;
  float vx, vy;
  float speed;
  int size;
  float angle = 0.0f;
  std::string textureId;
  Animation * anim;

};
