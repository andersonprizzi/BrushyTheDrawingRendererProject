#ifndef POINT_H
#define POINT_H

class Point {
    private:
        int x, y;

    public:
        Point(int new_x, int new_y);
        int get_x();
        int get_y();
        void set_x(int new_x);
        void set_y(int new_y);
        void print();
};

#endif


