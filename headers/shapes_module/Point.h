#ifndef POINT_H
#define POINT_H

class Point {
    private:
        double x, y;

    public:
        Point(double new_x, double new_y);
        double get_x();
        double get_y();
        void set_x(double new_x);
        void set_y(double new_y);
        void print();
};

#endif


