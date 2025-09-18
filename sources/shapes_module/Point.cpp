#include <cstdio>
#include "Point.h"


// CONSTRUCTOR IMPLEMENTATION
Point::Point(int new_x = 0, int new_y = 0) {
    x = new_x;
    y = new_y;
}


// METHOD IMPLEMENTATION
int Point::get_x() {
    return x;
};


// METHOD IMPLEMENTATION
int Point::get_y() {
    return y;
}


// METHOD IMPLEMENTATION
void Point::set_x(int new_x){
    x = new_x;
}


// METHOD IMPLEMENTATION
void Point::set_y(int new_y) {
    y = new_y;
}


// METHOD IMPLEMENTATION
void Point::print() {
    fprintf(stdout, "Point: (%f, %f).\n", this->x, this->y);
}
