#ifndef APPBAR_COMPONENT_H
#define APPBAR_COMPONENT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class AppBar {
    public:
        AppBar(int width, int height, const std::string& title, TTF_Font* font);
        void setBackgroundColor(SDL_Color color);
        void setTextColor(SDL_Color color);
        void setMarginLeft(int margin);
        void setTitle(const std::string& title);
        void setSize(int width, int height);
        void draw(SDL_Surface* targetSurface);

    private:
        SDL_Rect rect;
        SDL_Color bgColor;
        SDL_Color textColor;
        std::string title;
        int marginLeft;
        TTF_Font* font;
};

#endif

