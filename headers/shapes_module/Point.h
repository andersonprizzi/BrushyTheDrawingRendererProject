#ifndef POINT_H
#define POINT_H
#include <sdl.h>

class Point {
    private:
        double x, y;

    public:
        Point(double new_x, double new_y);
        Point(double new_x, double new_y, Uint32 color);
        double get_x() const;
        double get_y() const;
        Uint32 color = 0;
        void set_x(double new_x);
        void set_y(double new_y);
        void print();
};

#endif


