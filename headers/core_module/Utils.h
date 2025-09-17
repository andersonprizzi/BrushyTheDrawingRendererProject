#ifndef UTILS_H
#define UTILS_H

#include "App.h"

class Utils {
    public:
        static int verify_limits(SDL_Surface* surface, int x, int y);
        static bool check_window_size(App *app, int width, int height);
};

#endif
