#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "App.h"

class Rectangle : public Shape {
private:
    int x1, y1, x2, y2;
    Uint32 color;

public:
    Rectangle(int x1, int y1, int x2, int y2, Uint32 color)
        : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {}

    void draw(SDL_Surface* surface) override {
        if (!surface) return;

        bool filled = false;
        bool anti_aliasing = false;

        /*// Desenhar retângulo preenchido usando Primitives
        for (int y = y1; y < y2; y++) {
            for (int x = x1; x < x2; x++) {
                Primitives::setPixel(surface, x, y, color);
            }
        }*/

        // Alternativamente, poderia usar apenas as arestas:
        Primitives::draw_line(surface, x1, y1, x2, y1, color, anti_aliasing); // topo
        Primitives::draw_line(surface, x1, y2, x2, y2, color, anti_aliasing); // base
        Primitives::draw_line(surface, x1, y1, x1, y2, color, anti_aliasing); // esquerda
        Primitives::draw_line(surface, x2, y1, x2, y2, color, anti_aliasing); // direita

        if (filled == false) return;

        for (int i = y1; i < y2; i++)
            Primitives::draw_line(surface, x1, i, x2, i, color, anti_aliasing);

    }

    int getX1() {
        return x1;
    }

    int getY1() {
        return y1;
    }

    int getX2() {
        return x2;
    }

    int getY2() {
        return y2;
    }

};

#endif
