#pragma once
#include <SDL3/SDL.h>
class Animation{
  
  public:
    Animation() = default;  
    Animation(int frameWidth, int frameHeight, int numFrames, int speed , bool looping = true, int columns = 1);

    void update();
    SDL_FRect getSrcRect() const;

    void reset();
    bool isFinished() const;

  private:
    int frameWidth = 0;
    int frameHeight = 0;
    int numFrames = 0;
    int frameSpeed = 100;
    bool looping = true;
    int columns = 0;
    int currentFrame = 0;
    Uint32 lastUpdate = 0;
    bool finished = false;
};
