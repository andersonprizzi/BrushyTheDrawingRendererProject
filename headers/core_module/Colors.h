#ifndef COLORS_H
#define COLORS_H

#include "App.h"

class Colors {
    public:
        struct rgb_color {
            Uint8 r, g, b;
        };

        struct color_entry {
            const char* name;
            rgb_color color;
        };

        static Uint32 rgb_to_uint32(SDL_Surface* surface, int r, int g, int b);
        static SDL_Color uint32_to_sdlcolor(SDL_Surface* surface, Uint32 color);
        static Uint32 get_color_by_name(SDL_Surface* surface, const std::string& name);

    private:
        static const int number_of_colors;
        static const color_entry colors[];
};

#endif
