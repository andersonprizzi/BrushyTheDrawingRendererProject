#ifndef UTILS_H
#define UTILS_H

#include "App.h"

class Utils {
    public:
        static int verify_limits(SDL_Surface* surface, int x, int y);
        static Uint32 rgb_to_uint32(SDL_Surface* surface, int r, int g, int b);
        static SDL_Color uint32_to_sdlcolor(SDL_Surface* surface, Uint32 color);
};

#endif
