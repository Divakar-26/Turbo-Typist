#include "animation.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_thread.h>
#include <SDL3/SDL_timer.h>

Animation::Animation(int fw, int fh, int nf, int speed, bool loop, int c):
  frameWidth(fw) , frameHeight(fh) , numFrames(nf) , frameSpeed(speed), looping(loop) ,columns(c), lastUpdate(SDL_GetTicks()), finished(false){}


void Animation::update() {
    if (finished) return;

    Uint32 now = SDL_GetTicks();
    if (now - lastUpdate >= (Uint32)frameSpeed) {
        lastUpdate = now;
        currentFrame++;

        if (currentFrame >= numFrames) {
            if (looping) {
                currentFrame = 0;
            } else {
                currentFrame = numFrames - 1;
                finished = true;
            }
        }
    }
}


SDL_FRect Animation::getSrcRect() const {
    int col = currentFrame % columns;
    int row = currentFrame / columns;

    return SDL_FRect{
        static_cast<float>(col * frameWidth),
        static_cast<float>(row * frameHeight),
        static_cast<float>(frameWidth),
        static_cast<float>(frameHeight)
    };
}


void Animation::reset() {

  currentFrame = 0;
  finished = false;
  lastUpdate = SDL_GetTicks();

}

bool Animation::isFinished() const{
  return finished;
}
