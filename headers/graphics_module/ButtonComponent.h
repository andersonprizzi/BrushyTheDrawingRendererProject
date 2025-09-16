#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

#include "App.h"

class ButtonComponent {
    public:
        int x, y;               // Position
        int w, h;               // Width and height
        Uint32 color;           // Background color
        std::string text;       // Button text
        TTF_Font* font;         // Font used to render text
        SDL_Color text_color;    // Text color

        ButtonComponent(int x, int y, int w, int h, Uint32 color, const std::string& text, TTF_Font* font, SDL_Color text_color);
        void draw(SDL_Surface* surface);
        bool is_clicked(int mouse_x, int mouse_y) const;
        void set_position(int new_x, int new_y);
};

#endif
