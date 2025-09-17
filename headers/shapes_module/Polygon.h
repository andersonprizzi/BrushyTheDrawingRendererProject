#ifndef POLYGON_H
#define POLYGON_H

#include "App.h"

class Polygon : public Shape {
    private:
        std::vector<Point> points;
        bool anti_aliasing;
        bool filled;
        Uint32 border_color;
        Uint32 fill_color;
        Point fill_point;
        bool fill_color_set;

    public:
        Polygon(bool anti_aliasing, bool filled, Uint32 border_color, Uint32 fill_color, Point fill_point);
        Polygon(bool anti_aliasing, bool filled, Uint32 border_color);
        void add_point(Point p);
        void clear_points();
        int get_point_count();
        virtual void draw(SDL_Surface* surface);
};

#endif
