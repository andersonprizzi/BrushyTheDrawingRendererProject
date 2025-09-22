#ifndef FENCE_H
#define FENCE_H

#include "Utils.h"
#include "Shape.h"   // <<---- ADICIONE ISTO
#include "Point.h"
#include <sdl.h>    // se preferir, pode manter por causa de uint32 (ou também forward)
//struct SDL_Surface; // forward-declare

class Fence : public Shape {
private:
    Uint32 plank_color = 0, top_color = 0;

    Point vert_plank1_bottom_left = Point(0,0);
    Point vert_plank1_bottom_right = Point(0,0);
    Point vert_plank1_top_right = Point(0,0);
    Point vert_plank1_top_left = Point(0,0);

    Point vert_plank2_bottom_left = Point(0,0);
    Point vert_plank2_bottom_right = Point(0,0);
    Point vert_plank2_top_right = Point(0,0);
    Point vert_plank2_top_left = Point(0,0);

    Point hor_plank1_bottom_left = Point(0,0);
    Point hor_plank1_bottom_right = Point(0,0);
    Point hor_plank1_top_right = Point(0,0);
    Point hor_plank1_top_left = Point(0,0);

    Point hor_plank2_bottom_left = Point(0,0);
    Point hor_plank2_bottom_right = Point(0,0);
    Point hor_plank2_top_right = Point(0,0);
    Point hor_plank2_top_left = Point(0,0);

    Point top1 = Point(0,0);
    Point top2 = Point(0,0);

    Point vertical1_fill = Point(0, 0);
    Point vertical2_fill = Point(0, 0);
    Point hor1_p1_fill = Point(0, 0);
    Point hor1_p2_fill = Point(0, 0);
    Point hor1_p3_fill = Point(0, 0);
    Point hor2_p1_fill = Point(0, 0);
    Point hor2_p2_fill = Point(0, 0);
    Point hor2_p3_fill = Point(0, 0);
    Point top1_fill = Point(0, 0);
    Point top2_fill = Point(0, 0);
    void generate_points() override;

    //std::vector<Point*> pts[28];
    std::array<Point*, 28> pts;

public:
    //int height = 0, width = 0, x_origin = 0, y_origin = 0;
    Uint32 colors[2] = {0,0};
    //Uint32 roof_color = 0, walls_color = 0, door_color = 0;

    Fence(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_plank, Uint32 color_top);


    void draw(SDL_Surface* surface) override;
    void translate(double dx, double dy) override;
    void rotate_figure(double angle) override;
    void scale(double sx, double sy) override;
    //void reset_transform() override;                                 // volta para identidade

/*
    void change_height(double new_height);
    void change_width(double new_width);
    void change_origin(Point new_origin);
*/
    //void scale(double sx, double sy, Point center);
};

#endif
