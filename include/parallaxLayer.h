#pragma once
#include <SDL3/SDL.h>
#include <string>

#include "animation.h"


class ParallaxLayer{
  public:
   ParallaxLayer(const std::string & textureId, float scrollSpeed, int winWidth, int winHeight );
   ParallaxLayer(const std::string & textureId, float scrollSpeed, int winWidth, int winHeight ,float x, float y, float w, float h);
   ParallaxLayer(const std::string & textureId, float scrollSpeed, int winWidth, int winHeight , Animation * animation,float x, float y, float w, float h);

   void update(float dt);
   void render(SDL_Renderer * renderer);
   std::string getTexId() {return texId;}
  private:
   std::string texId;
   float scrollSpeed;
   float offsetY;
   int winWidth, winHeight;
   bool isSubPart = false; 
   bool isAnimated = false;
   Animation * anim = nullptr;
   float objX = 0, objY = 0, objW = 0, objH = 0;
};
