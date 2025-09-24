#ifndef TREE_H
#define TREE_H

#include "Utils.h"
#include "Shape.h"
#include "Point.h"
#include <sdl.h>

class Tree : public Shape {
    private:
        Uint32 trunk_color = 0, leaves_color = 0, apple_color = 0;
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

        std::array<Point*, 12> pts;

        /*Point apple_fill = Point(0, 0);
        Point leaves1_fill = Point(0, 0);
        Point leaves2_fill = Point(0, 0);
        Point leaves3_fill = Point(0, 0);*/

        void generate_points() override;

    public:
        Tree(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_trunk, Uint32 color_leaves, Uint32 color_apple);
        Uint32 colors[3] = {0,0,0};
        void draw(SDL_Surface* surface) override;
        void translate(double dx, double dy) override;
        void rotate_figure(double angle) override;
        void scale(double sx, double sy) override;
};

#endif
