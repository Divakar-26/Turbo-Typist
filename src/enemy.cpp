#include "enemy.h"
#include "textureManager.h"
#include <cmath>

Enemy::Enemy(int size, float x, float y, float speed,const std::string &word, const std::string &textureId ): x(x), y(y), speed(speed), word(word), textureId(textureId) {
  this->size = size;
}


void Enemy::update(float dt){
  y += speed * dt;
}

void Enemy::render(SDL_Renderer * renderer, float playerX, float playerY){
  
  SDL_FPoint center = {size / 2.0f, size / 2.0f};
  SDL_FRect dst = {x, y, (float)size, (float)size};
  double angle = std::atan((playerY - dst.y) / (playerX - dst.x));

  angle = angle * (180.0 / M_PI); 

  TextureManager::drawRotated(textureId, dst, renderer,nullptr,angle + 90.0f,&center);

}

bool Enemy::isOffScreen(int screenHeight) const {

  return y > screenHeight;
}
