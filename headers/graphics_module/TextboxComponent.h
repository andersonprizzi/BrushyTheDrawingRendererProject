#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "App.h"

class TextboxComponent {
    private:
        SDL_Color bgColor;
        SDL_Color borderColor;
        SDL_Color textColor;
        TTF_Font* font;
        std::string text;
        bool onlyNumbers;
        bool active;

    public:
        SDL_Rect rect; // TODO: Colocar como privado e adicionar um getter.
        TextboxComponent(int x, int y, int w, int h, TTF_Font* font, bool onlyNumbers = false);

        void handleEvent(SDL_Event* e);
        void render(SDL_Surface* targetSurface);

        void setText(const std::string& t) { text = t; }
        const std::string& getText() const { return text; }
        void setActive(bool a) { active = a; }
};

#endif
