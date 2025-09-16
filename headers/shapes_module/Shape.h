#ifndef SHAPE_H
#define SHAPE_H

#include "App.h"

class Shape {
    public:
        virtual ~Shape() {}
        virtual void draw(SDL_Surface* surface) = 0;
};

#endif
