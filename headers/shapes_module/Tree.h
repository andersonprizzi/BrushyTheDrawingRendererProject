#ifndef TREE_H
#define TREE_H

#include "Utils.h"
#include "Shape.h"   // <<---- ADICIONE ISTO
#include "Point.h"
#include <sdl.h>    // se preferir, pode manter por causa de uint32 (ou também forward)
//struct SDL_Surface; // forward-declare

class Tree : public Shape {
private:
    Uint32 trunk_color = 0, leaves_color = 0, apple_color = 0;
    float rotated_angle = 0;

    Point trunk_bottom_left = Point(0,0);
    Point trunk_bottom_right = Point(0,0);
    Point trunk_top_right = Point(0,0);
    Point trunk_top_left = Point(0,0);
    Point trunk_left_bezier_point = Point(0,0);
    Point trunk_right_bezier_point = Point(0,0);

    Point leaves_first_elipsis_center = Point(0,0);
    Point leaves_second_elipsis_center = Point(0,0);
    Point leaves_third_elipsis_center = Point(0,0);

    Point apple_center = Point(0,0);
    Point apple2_center = Point(0,0);

    Point trunk_fill = Point(0, 0);
    /*Point apple_fill = Point(0, 0);
    Point leaves1_fill = Point(0, 0);
    Point leaves2_fill = Point(0, 0);
    Point leaves3_fill = Point(0, 0);*/

    void generate_points();

public:
    int height = 0, width = 0, x_origin = 0, y_origin = 0;
    Uint32 colors[3] = {0,0,0};
    //Uint32 roof_color = 0, walls_color = 0, door_color = 0;

    Tree(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_trunk, Uint32 color_leaves, Uint32 color_apple);

    void draw(SDL_Surface* surface);
    void translate(double dx, double dy);
    void rotate_figure(double angle);


    void change_height(double new_height);
    void change_width(double new_width);
    void change_origin(Point new_origin);

    void reset_transform();                                 // volta para identidade
    void scale(double sx, double sy);                   // T = T * Tr
    //void scale(double sx, double sy, Point center);
};

#endif
