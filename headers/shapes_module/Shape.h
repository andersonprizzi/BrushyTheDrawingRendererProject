#ifndef SHAPE_H
#define SHAPE_H

#include "Point.h"

class Shape {
    private:
    public:
        double width  = 0.0;
        double height = 0.0;
        double x_origin = 0.0;
        double y_origin = 0.0;
        float rotated_angle = 0;
        virtual ~Shape() {}
        virtual void draw(SDL_Surface* surface) = 0;
        virtual void generate_points() = 0;
        virtual void scale(double scale_x, double scale_y) = 0;
        virtual void translate(double translation_x, double translation_y) = 0;
        virtual void rotate_figure(double angle) = 0;

        void change_height(double new_height){
            this->height = new_height;
            generate_points();
        }

        void change_width(double new_width){
            this->width = new_width;
            generate_points();
        }

        void change_origin(Point new_origin){
            this->x_origin = new_origin.get_x();
            this->y_origin = new_origin.get_y();
            generate_points();
        }
};

#endif
