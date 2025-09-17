#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(int x, int y, int w, int h, Uint32 color, const std::string& text, TTF_Font* font, SDL_Color text_color) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->color = color;
    this->text = text;
    this->font = font;
    this->text_color = text_color;
}

void ButtonComponent::draw(SDL_Surface* surface) {
    if (!surface) return;

    int radius = 6; // Rounded corner radius

    // Draw central areas
    SDL_Rect rect = { x + radius, y, w - 2*radius, h };
    SDL_FillRect(surface, &rect, color);
    rect = { x, y + radius, w, h - 2*radius };
    SDL_FillRect(surface, &rect, color);

    // Draw rounded corners
    Primitives::draw_circle(surface, x + radius, y + radius, radius, color, true, true);                 // top-left
    Primitives::draw_circle(surface, x + w - radius - 1, y + radius, radius, color, true, true);       // top-right
    Primitives::draw_circle(surface, x + radius, y + h - radius - 1, radius, color, true, true);       // bottom-left
    Primitives::draw_circle(surface, x + w - radius - 1, y + h - radius - 1, radius, color, true, true); // bottom-right

    // Draw text
    if (font && !text.empty()) {
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), text_color);
        if (textSurface) {
            SDL_Rect textRect;
            textRect.w = textSurface->w;
            textRect.h = textSurface->h;
            textRect.x = x + (w - textRect.w) / 2;
            textRect.y = y + (h - textRect.h) / 2;
            SDL_BlitSurface(textSurface, NULL, surface, &textRect);
            SDL_FreeSurface(textSurface);
        }
    }
}

bool ButtonComponent::is_clicked(int mouse_x, int mouse_y) const {
    return (mouse_x >= x && mouse_x <= x + w && mouse_y >= y && mouse_y <= y + h);
}

void ButtonComponent::set_position(int new_x, int new_y) {
    x = new_x;
    y = new_y;
}
