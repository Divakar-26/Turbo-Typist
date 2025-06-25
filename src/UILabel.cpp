#include "UILabel.h"
#include <algorithm>
#include <cmath>

UILabel::UILabel(float x, float y, const std::string &text, SDL_Color color, TextManager *tm)
    : UIElement(x, y), text(text), color(color), textManager(tm) {}

void UILabel::render(SDL_Renderer *renderer) {
    SDL_Color bg = {0, 0, 0, 0};
    SDL_Color fadeColor = {color.r, color.g, color.b, static_cast<Uint8>(opacity)};
    textManager->renderText(text, x, y, fadeColor, bg, true);
}

void UILabel::setText(const std::string &newText) {
    text = newText;
}

void UILabel::animateInFromTop(float toY) {
    animState = LabelAnimState::MOVING_IN;
    animTime = 0.0f;
    startY = -100.0f;
    targetY = toY;
    endY = toY + 100.0f;
    opacity = 0.0f;

    x = x; 
    y = startY;
}

void UILabel::update(float dt) {
    UIElement::update(dt);

    if (animState == LabelAnimState::MOVING_IN) {
        animTime += dt;
        float t = std::min(animTime / duration, 1.0f);
        t = 1 - std::pow(1 - t, 3); 

        y = startY + (targetY - startY) * t;
        opacity = 255 * t;

        if (t >= 1.0f) {
            animState = LabelAnimState::HOLDING;
            animTime = 0.0f;
        }
    } else if (animState == LabelAnimState::HOLDING) {
        animTime += dt;
        if (animTime > holdTime) {
            animState = LabelAnimState::MOVING_OUT;
            animTime = 0.0f;
        }
    } else if (animState == LabelAnimState::MOVING_OUT) {
        animTime += dt;
        float t = std::min(animTime / duration, 1.0f);
        t = t * t; 

        y = targetY + (endY - targetY) * t;
        opacity = 255 * (1.0f - t);

        if (t >= 1.0f) {
            animState = LabelAnimState::NONE;
            opacity = 0.0f;
        }
    }
}
