#include "textureManager.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <iostream>
#include <iterator>
#include <ostream>

std::unordered_map<std::string, SDL_Texture *> TextureManager::textures;

bool TextureManager::load(const std::string &id, const std::string &filename, SDL_Renderer *renderer){
  SDL_Surface * surface = IMG_Load(filename.c_str());
  if(!surface){
    std::cerr<<"Failed to load surface : "<<std::endl;
    return false;
  }

  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  SDL_DestroySurface(surface);

  if(!texture){
    std::cerr<<"Failed to laod texture :"<<std::endl;
    return false;
  }

  textures[id] = texture;
  return true;
}

void TextureManager::draw(const std::string &id, SDL_FRect dst, SDL_Renderer *renderer, SDL_FRect * src){
  if(textures.find(id) != textures.end()){
    SDL_RenderTexture(renderer, textures[id], src, &dst);
  }else{
    std::cerr<<"Texture not found"<<std::endl;
  }
}

void TextureManager::Drop(const std::string & id){
  if(textures.find(id) != textures.end()){
    SDL_DestroyTexture(textures[id]);
    textures.erase(id);
  }
}

void TextureManager::clean(){
  for(auto & it: textures){
    SDL_DestroyTexture(it.second);
  }
  textures.clear();
}

void TextureManager::drawRotated(const std::string &id,const SDL_FRect &dst, SDL_Renderer *renderer, SDL_FRect *src, double angle, const SDL_FPoint * center, SDL_FlipMode flip){
  auto it = textures.find(id);
  if(it != textures.end()){
    SDL_RenderTextureRotated(renderer, it->second, src, &dst, angle,center, flip);
  }
  else{
    std::cerr<<"Texture "<<id<<" not found in Textures"<<std::endl;
  }
}

SDL_Texture* TextureManager::get(const std::string& id) {
    auto it = textures.find(id);
    return (it != textures.end()) ? it->second : nullptr;
}
