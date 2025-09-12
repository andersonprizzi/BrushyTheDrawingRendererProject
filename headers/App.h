#ifndef APP_H
#define APP_H

///////////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINITIONS AND IMPORTS                                            //
///////////////////////////////////////////////////////////////////////////////

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <stack>
#include <unordered_map>
#include <strings.h>

#include "Utils.h"
#include "Colors.h"
#include "Primitives.h"
#include "Shape.h"
#include "Circle.h"



///////////////////////////////////////////////////////////////////////////////
// ...                                                                       //
///////////////////////////////////////////////////////////////////////////////

class App {
    private:
        SDL_Window* window;
        SDL_Surface* surface;
        bool running;
        int window_width;
        int window_height;
        std::string window_title;

    public:
        App(const std::string& title, int width, int height);
        ~App();
        void run();
        void handle_events();
        void clear_screen(Uint8 r, Uint8 g, Uint8 b);
        void update_screen();
        int getWidth() const;
        int getHeight() const;
        SDL_Surface* getSurface();
};

#endif
