#include <SDL3/SDL_rect.h>
#define TEXTUREMANAGER_H

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>

#include <SDL3_image/SDL_image.h>
class TextureManager{
  public:
  static bool load(const std::string & id, const std::string & filename, SDL_Renderer * renderer);
  static void draw(const std::string & id,SDL_FRect dst, SDL_Renderer * renderer, SDL_FRect * src = nullptr);
  static void drawRotated(const std::string &id,const SDL_FRect &dst, SDL_Renderer * renderer, SDL_FRect * src , double angle, const SDL_FPoint * center = nullptr, SDL_FlipMode flip = SDL_FLIP_NONE);
  static void Drop(const std::string & id);
  static void clean();

  private:
   static std::unordered_map<std::string, SDL_Texture *> textures;

};
