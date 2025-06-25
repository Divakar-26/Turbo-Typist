#pragma once
#include "UIElement.h"
#include "textManager.h"
#include <string>

enum class LabelAnimState { NONE, MOVING_IN, HOLDING, MOVING_OUT };

class UILabel : public UIElement {
private:
    std::string text;
    SDL_Color color;
    TextManager *textManager;

    LabelAnimState animState = LabelAnimState::NONE;
    float animTime = 0.0f;
    float duration = 1.0f;
    float holdTime = 2.0f;

    float startY = 0.0f;
    float targetY = 0.0f;
    float endY = 0.0f;

public:
    UILabel(float x, float y, const std::string &text, SDL_Color color, TextManager *tm);

    void render(SDL_Renderer *renderer) override;
    void update(float dt) override;

    void setText(const std::string &newText);
    void moveTo(float newX, float newY) { x = newX; y = newY; }

    void animateInFromTop(float toY);
};
