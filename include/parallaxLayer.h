#pragma once
#include <SDL3/SDL.h>
#include <string>

class ParallaxLayer{
  public:
   ParallaxLayer(const std::string & textureId, float scrollSpeed, int winWidth, int winHeight );

   void update(float dt);
   void render(SDL_Renderer * renderer);

  private:
   std::string texId;
   float scrollSpeed;
   float offsetY;
   int winWidth, winHeight;
};
