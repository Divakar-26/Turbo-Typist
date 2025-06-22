#include "bullet.h"
#include <SDL3/SDL_rect.h>
#include "textureManager.h"
#include<cmath>

Bullet::Bullet(float x, float y, float vx, float vy, float speed, int size, const std::string &id): x(x), y(y), vx(vx), vy(vy), speed(speed), size(size), textureId(id){
  anim = new Animation(48,48,2,100,true,2);
  angle = std::atan2(vy, vx) * (180.0f / M_PI) + 180.0f;  // Calculate direction
}

void Bullet::update(float dt){
  x  += vx * speed *dt;
  y += vy * speed *dt;
  anim->update();

}

void Bullet::render(SDL_Renderer *renderer) {
    SDL_FRect dst = { x, y, (float)size, (float)size };
    SDL_FRect src = anim->getSrcRect();
    SDL_FPoint center = { size / 2.0f, size / 2.0f };

    // Rotate the bullet sprite based on movement angle
    TextureManager::drawRotated(textureId, dst, renderer, &src, angle - 90.0f, &center);
}


bool Bullet::isOffScreen(int windowW, int windowH){
  return x < -size || x > windowW || y < -size || y > windowH;
}
