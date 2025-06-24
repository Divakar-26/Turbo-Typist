#pragma once

#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <iostream>
#include <memory>
#include "animation.h"

class Enemy
{
public:
  Enemy(int size, float x, float y, float speed, const std::string &word, const std::string &textureId);

  void update(float dt, float playerX, float playerY);
  void render(SDL_Renderer *renderer, float playerX, float playerY);

  float getX() const { return x; }
  float getY() const { return y; }
  bool isOffScreen(int screenHeight) const;
  const std::string &getWord() const { return word; }
  float getSize() { return size; }

  void getHit()
  {
    if (health <= 0 || dead)
      return;

    word[word.size() - health] = ' ';
    health--;

    knockbackVelocity = -20.0f;
    knockbackDuration = maxKnockbackTime;

    if (health <= 0 && !dead)
    {
      dead = true;
      deathAnim = std::make_unique<Animation>(48, 48, 7, 100, false, 7);
      deathAnim->reset();
      std::cout << "Death aniamtion started" << std::endl;
    }
  };

  bool isDead() const { return dead; }
  bool isDeathAnimFinished() const { return deathAnim && deathAnim->isFinished(); }
  int health = 0;

  bool isTargeted = false;
  float lockAnimTime = 0.0f;
  Animation *lockAnim = nullptr; // lock-on animation
private:
  float x, y;
  float speed;
  int size;

  std::string word;
  std::string textureId;

  // death
  bool dead = false;
  std::unique_ptr<Animation> deathAnim = nullptr;

  std::string deathTextureId = "explosion"; // ID you loaded with TextureManager

  float knockbackVelocity = 0.0f;
  float knockbackDuration = 0.0f;
  float maxKnockbackTime = 0.1f; // 0.1s = 100ms
};
