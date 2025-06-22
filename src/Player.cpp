#include "Player.h"
#include "animation.h"
#include <SDL3/SDL_rect.h>

Player::Player(int size,float x, float y, const std::string& texId, const std::string& thId){
  this->size = size;
  this->x = x;
  this->y = y;
  this->textureId = texId;
  this->thrusterTexId = thId;
  anim = new Animation(48,48,2,100,true); 
}

Player::~Player(){}

void Player::handleEvent(SDL_Event & event){
  if(event.type == SDL_EVENT_KEY_DOWN){
    switch (event.key.key){
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

void Player::update(float dt){
 anim->update(); 

}
  

void Player::render(SDL_Renderer * renderer){

  SDL_FRect th = anim->getSrcRect();

  SDL_FRect thDest = {
    x + (size / 48.0f) * -3.0f,
    y + (size / 48.0f) * 28.0f,
    size,size
  };
  TextureManager::draw(thrusterTexId, thDest, renderer, &th);
  thDest.x = (size / 48.0f) * 7.0f + x;

  TextureManager::draw(thrusterTexId, thDest, renderer, &th);
  // SDL_SetRenderDrawColor(renderer, 255,0,0,0);
  SDL_FRect r = {x, y, (float)size, (float)size};
  TextureManager::draw(textureId, r, renderer);




}
