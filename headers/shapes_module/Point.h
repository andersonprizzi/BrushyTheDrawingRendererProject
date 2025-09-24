#ifndef POINT_H
#define POINT_H

class Point {
    private:
        double x, y;

    public:
        Point(double new_x, double new_y);
        double get_x() const;
        double get_y() const;
        void set_x(double new_x);
        void set_y(double new_y);
        void print();
};

#endif


