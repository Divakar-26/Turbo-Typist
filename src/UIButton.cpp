#include "UIButton.h"

UIButton::UIButton(float x, float y, const std::string& text, SDL_Color normal, SDL_Color hover, TextManager* tm)
    : UIElement(x, y), text(text), normalColor(normal), hoverColor(hover), textManager(tm) {
    bounds = {x, y, 0, 0}; // Will be estimated by text size if needed
}

void UIButton::render(SDL_Renderer* renderer) {
    SDL_Color textColor = isHovered ? hoverColor : normalColor;
    textColor.a = (Uint8)opacity;

    SDL_FRect bgRect = {x, y, width, height}; 
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Color bgColor = isHovered ? SDL_Color{0, 0, 0, 120} : SDL_Color{0, 0, 0, 60};
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &bgRect);

    // Draw text centered
    textManager->renderText(text, x + 100, y + 20, textColor, {0, 0, 0, 0}, true);
}


void UIButton::update(float dt) {
    UIElement::update(dt);
    float mx, my;
    SDL_GetMouseState(&mx, &my);
    isHovered = (mx >= x && mx <= x + width && my >= y && my <= y + height);
}

void UIButton::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && isHovered) {
        // Optional: click sound
    }
}

bool UIButton::isClicked(const SDL_Event& e) {
    return e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && isHovered;
}
