#ifndef COLORS_H
#define COLORS_H

#include "App.h"

class Colors {
    public:
        struct rgb_color {
            Uint8 r, g, b;
        };

        struct ColorItem {
            const char* name;
            rgb_color color;
        };

        static Uint32 rgb_to_uint32(SDL_Surface* surface, int r, int g, int b);
        static SDL_Color uint32_to_sdlcolor(SDL_Surface* surface, Uint32 color);
        static Uint32 get_color(SDL_Surface* surface, const Colors::ColorItem* color_table, const int table_size, const std::string& color_name);

        static const int number_of_drawing_colors;
        static const ColorItem drawing_colors_table[];
        static const int number_of_interface_colors;
        static const ColorItem interface_colors_table[];
};

#endif
