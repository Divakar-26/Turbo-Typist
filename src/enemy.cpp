#include "enemy.h"
#include "textureManager.h"
#include <cmath>

Enemy::Enemy(int size, float x, float y, float speed,const std::string &word, const std::string &textureId ): x(x), y(y), speed(speed), word(word), textureId(textureId) {
  this->size = size;
}


void Enemy::update(float dt, float playerX, float playerY){
  y += speed * dt;
  
  float centerX = x + size/ 2.0f;
  float centerY = y + size/2.0f;
  float dx = playerX - centerX;
  float dy = playerY - centerY;

  float length = std::sqrt(dx*dx + dy*dy);
  if(length != 0){
    dx /= length;
    dy /= length;
  }

  float driftSpeed = 20.0f;
  x += dx * driftSpeed * dt;
  y += dy * driftSpeed * dt * 0.5f;
}

void Enemy::render(SDL_Renderer * renderer, float playerX, float playerY){
  
  SDL_FPoint center = {size / 2.0f, size / 2.0f};
  SDL_FRect dst = {x, y, (float)size, (float)size};
  double dx = playerX - (x + center.x);
  double dy = playerY - (y + center.y);
  double angle = std::atan2(dy,dx) * (180.0f / M_PI);


  TextureManager::drawRotated(textureId, dst, renderer,nullptr,angle + 90.0f,&center);

}

bool Enemy::isOffScreen(int screenHeight) const {

  return y > screenHeight;
}
