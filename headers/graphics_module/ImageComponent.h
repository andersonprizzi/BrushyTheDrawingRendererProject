#ifndef IMAGE_COMPONENT_H
#define IMAGE_COMPONENT_H

#include "App.h"

class ImageComponent {
    private:
        SDL_Surface* image;
        int x, y;

    public:
        ImageComponent(const std::string& filepath, int x, int y)
            : x(x), y(y)
        {
            image = SDL_LoadBMP(filepath.c_str());
            if (!image) {
                fprintf(stderr, "Erro ao carregar imagem %s: %s\n", filepath.c_str(), SDL_GetError());
            }
        }

        ~ImageComponent() {
            if (image) {
                SDL_FreeSurface(image);
            }
        }

        void draw(SDL_Surface* targetSurface) {
            if (!image) return;

            SDL_Rect dstRect = { x, y, 0, 0 }; // posição da imagem
            SDL_BlitSurface(image, NULL, targetSurface, &dstRect);
        }

        void setPosition(int newX, int newY) {
            x = newX;
            y = newY;
        }

        int getX() const { return x; }
        int getY() const { return y; }
};

#endif
