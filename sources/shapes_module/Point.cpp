#include <cstdio>
#include "Point.h"


// CONSTRUCTOR IMPLEMENTATION
Point::Point(double new_x = 0, double new_y = 0) {
    x = new_x;
    y = new_y;
}


// METHOD IMPLEMENTATION
double Point::get_x() {
    return x;
};


// METHOD IMPLEMENTATION
double Point::get_y() {
    return y;
}


// METHOD IMPLEMENTATION
void Point::set_x(double new_x){
    x = new_x;
}


// METHOD IMPLEMENTATION
void Point::set_y(double new_y) {
    y = new_y;
}


// METHOD IMPLEMENTATION
void Point::print() {
    fprintf(stdout, "Point: (%f, %f).\n", this->x, this->y);
}
