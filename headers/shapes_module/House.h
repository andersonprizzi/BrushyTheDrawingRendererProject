#ifndef HOUSE_H
#define HOUSE_H

#include "Utils.h"
#include "Shape.h"   // <<---- ADICIONE ISTO
#include "Point.h"
#include <sdl.h>    // se preferir, pode manter por causa de uint32 (ou tamb�m forward)
//struct SDL_Surface; // forward-declare

class House : public Shape {
private:
    /*typedef struct Matrix{
        double m[3][3];
    } Matrix;*/

    //Utils::Matrix T; //Trnasform

    Uint32 roof_color = 0, walls_color = 0, door_color = 0;

    Point wall_top_left = Point(0,0);
    Point wall_bottom_right = Point(0,0);
    Point wall_top_right = Point(0,0);
    Point wall_bottom_left = Point(0,0);

    Point door_top_left = Point(0,0);
    Point door_top_right = Point(0,0);
    Point door_bottom_left = Point(0,0);
    Point door_bottom_right = Point(0,0);

    Point roof_peak = Point(0,0);

    Point wall_fill = Point(0, 0);
    Point wall_fill2 = Point(0, 0);
    Point wall_fill3 = Point(0, 0);
    Point door_fill = Point(0, 0);
    Point roof_fill = Point(0, 0);
    Point roof_fill2 = Point(0, 0);
    Point roof_fill3 = Point(0, 0);
    Point roof_fill4 = Point(0, 0);

    std::array<Point*, 17> pts;

    void generate_points() override;


public:
    int height = 0, width = 0, x_origin = 0, y_origin = 0;
    Uint32 colors[3] = {0,0,0};
    //Uint32 roof_color = 0, walls_color = 0, door_color = 0;

    House(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_walls, Uint32 color_door, Uint32 color_roof);

    void draw(SDL_Surface* surface) override;

    /*
    void change_height(double new_height);
    void change_width(double new_width);
    void change_origin(Point new_origin);
    */

    //void reset_transform();                                 // volta para identidade
    void translate(double dx, double dy) override;
    void scale(double sx, double sy) override;                   // T = T * Tr
    //void rotate_figure(SDL_Surface* surface, double degrees, Point center); // T = T * R(piv�)
    void rotate_figure(double angle) override;
    //void scale(double sx, double sy, Point center);
};

#endif
