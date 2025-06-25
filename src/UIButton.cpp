#include "UIButton.h"

UIButton::UIButton(float x, float y, const std::string &text, SDL_Color normal, SDL_Color hover, TextManager *tm)
    : UIElement(x, y), text(text), normalColor(normal), hoverColor(hover), textManager(tm)
{
    bounds = {x, y, 0, 0};
}

void UIButton::render(SDL_Renderer *renderer)
{
    SDL_Color textColor = isHovered ? hoverColor : normalColor;
    textColor.a = static_cast<Uint8>(opacity); // ✅ correct now

    SDL_FRect bgRect = {x, y, width, height};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    Uint8 bgAlpha = static_cast<Uint8>(opacity * (isHovered ? 0.5f : 0.3f));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, bgAlpha);
    SDL_RenderFillRect(renderer, &bgRect);

    textManager->renderText(text, x + width / 2, y + height / 2, textColor, {0, 0, 0, 0}, true);
}


void UIButton::update(float dt)
{
    UIElement::update(dt);
    float mx, my;
    SDL_GetMouseState(&mx, &my);
    isHovered = (mx >= x && mx <= x + width && my >= y && my <= y + height);
}

void UIButton::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && isHovered)
    {
    }
}

bool UIButton::isClicked(const SDL_Event &e)
{
    return e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && isHovered;
}
