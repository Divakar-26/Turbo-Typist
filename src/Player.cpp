#include "Player.h"
#include "animation.h"
#include <SDL3/SDL_rect.h>
#include <cmath>

double angle = 0.0f;

Player::Player(int size, float x, float y, const std::string &texId, const std::string &thId)
{
  this->size = size;
  this->x = x;
  this->y = y;
  this->targetX = x; 
  this->targetY = y; 
  this->textureId = texId;
  this->thrusterTexId = thId;
  anim = new Animation(48, 48, 2, 100, true, 2);
}

Player::~Player() {}

void Player::handleEvent(SDL_Event &event)
{
  if (event.type == SDL_EVENT_KEY_DOWN)
  {
    switch (event.key.key)
    {
    case SDLK_W:
      y -= 5.0f;
      break;
    case SDLK_S:
      y += 5.0f;
      break;
    case SDLK_A:
      x -= 5.0f;
      break;
    case SDLK_D:
      x += 5.0f;
      break;
    }
  }
}

void Player::update(float dt)
{
  anim->update();

  float dx = targetX - x;
  float dy = targetY - y;
  float dist = sqrt(dx * dx + dy * dy);
  if (dist > 1.0f)
  {
    float angle = atan2(dy, dx);
    float stepX = cos(angle) * moveSpeed * dt;
    float stepY = sin(angle) * moveSpeed * dt;
    if (abs(stepX) > abs(dx))
      stepX = dx;
    if (abs(stepY) > abs(dy))
      stepY = dy;
    x += stepX;
    y += stepY;
  }

  hoverTime += dt;
  hoverOffset = std::sin(hoverTime * 2.0f) * 2.0f;
}

void Player::render(SDL_Renderer *renderer)
{
  SDL_FRect src = anim->getSrcRect();
  SDL_FPoint center = {size / 2.0f, size / 2.0f}; // Player center

  SDL_FRect playerDst = {x, y + hoverOffset, (float)size, (float)size};

  float cx = x + size / 2.0f;
  float cy = y + hoverOffset + size / 2.0f;

  float offsetX1 = -5;
  float offsetY1 = 20 + 18;

  float offsetX2 = 10;
  float offsetY2 = 20 + 18;

  float rad = angle * (M_PI / 180.0f);

  // Rotate offset
  float tx1 = cx + offsetX1 * cos(rad) - offsetY1 * sin(rad);
  float ty1 = cy + offsetX1 * sin(rad) + offsetY1 * cos(rad);

  float tx2 = cx + offsetX2 * cos(rad) - offsetY2 * sin(rad);
  float ty2 = cy + offsetX2 * sin(rad) + offsetY2 * cos(rad);

  SDL_FRect thrusterDst1 = {tx1 - size / 2.0f, ty1 - size / 2.0f, (float)size, (float)size};
  SDL_FRect thrusterDst2 = {tx2 - size / 2.0f, ty2 - size / 2.0f, (float)size, (float)size};

  TextureManager::drawRotated(thrusterTexId, thrusterDst1, renderer, &src, angle, &center);
  TextureManager::drawRotated(thrusterTexId, thrusterDst2, renderer, &src, angle, &center);
  TextureManager::drawRotated(textureId, playerDst, renderer, nullptr, angle, &center);

  SDL_FRect healthSrc = {0,0,16*3, 16};

  TextureManager::draw(healtbarTexture, healthbarDest, renderer, &healthSrc);
}

void Player::shoot(std::vector<Bullet> &bullets)
{
  float bulletX = x + size / 2.0f;
  float bulletY = y + size / 2.0f;

  float angleRad = (angle - 90.0f) * (M_PI / 180.0f);

  float vx = std::cos(angleRad);
  float vy = std::sin(angleRad);

  float bulletSpeed = 1500.0f;
  int bulletSize = (size / 48.0f) * 16;

  bullets.emplace_back(
      bulletX - bulletSize / 2,
      bulletY - bulletSize / 2,
      vx,
      vy,
      bulletSpeed,
      bulletSize,
      "bullet");
}

void Player::lookAt(float x, float y)
{
  float dx = x - (this->x + size / 2.0f);
  float dy = y - (this->y + size / 2.0f);
  angle = std::atan2(dy, dx) * (180.0f / M_PI) + 90.0f;
}

void Player::moveTo(float x, float y)
{
  targetX = x;
  targetY = y;
}
