#include "enemy.h"
#include "textureManager.h"

Enemy::Enemy(int size, float x, float y, float speed,const std::string &word, const std::string &textureId ): x(x), y(y), speed(speed), word(word), textureId(textureId) {
  this->size = size;
}


void Enemy::update(float dt){
  y += speed * dt;
}

void Enemy::render(SDL_Renderer * renderer){
  SDL_FRect dst = {x, y, (float)size, (float)size};
  TextureManager::draw(textureId, dst, renderer);

}

bool Enemy::isOffScreen(int screenHeight) const {

  return y > screenHeight;
}
