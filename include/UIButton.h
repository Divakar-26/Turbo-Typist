#pragma once
#include "UIElement.h"
#include "textManager.h"

class UIButton : public UIElement
{
private:
    std::string text;
    SDL_Color normalColor;
    SDL_Color hoverColor;
    TextManager *textManager;
    bool isHovered = false;
    SDL_FRect bounds;
    float width = 200;
    float height = 40;

public:
    UIButton(float x, float y, const std::string &text, SDL_Color normal, SDL_Color hover, TextManager *tm);

    void render(SDL_Renderer *renderer) override;
    void update(float dt) override;
    void handleEvent(const SDL_Event &e);

    bool isClicked(const SDL_Event &e);
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    void setSize(float w, float h)
    {
        width = w;
        height = h;
    }

    void setText(const std::string &text)
    {
        this->text = text;
    }

    using UIElement::fadeIn;
    using UIElement::fadeOut;
};
