#include "UIElement.h"
#include <cmath>

UIElement::UIElement(float x, float y) : x(x), y(y), targetX(x), targetY(y) {}

void UIElement::update(float dt) {
    float dx = targetX - x;
    float dy = targetY - y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 1.0f) {
        float dirX = dx / dist;
        float dirY = dy / dist;
        x += dirX * speed * dt;
        y += dirY * speed * dt;
    }

    // Fade
    if (fadingOut) {
        opacity -= fadeSpeed * dt;
        if (opacity <= 0) {
            opacity = 0;
            fadingOut = false;
        }
    }
    if (fadingIn) {
        opacity += fadeSpeed * dt;
        if (opacity >= 255) {
            opacity = 255;
            fadingIn = false;
        }
    }
}

void UIElement::moveTo(float newX, float newY) {
    targetX = newX;
    targetY = newY;
}

void UIElement::fadeIn() {
    fadingIn = true;
    fadingOut = false;
}

void UIElement::fadeOut() {
    fadingOut = true;
    fadingIn = false;
}
