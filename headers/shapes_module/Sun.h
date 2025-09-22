#ifndef SUN_H
#define SUN_H

#include "Utils.h"
#include "Shape.h"   // <<---- ADICIONE ISTO
#include "Point.h"
#include <sdl.h>    // se preferir, pode manter por causa de uint32 (ou também forward)

class Sun : public Shape {
private:
    Uint32 sun_color = 0, sunrays_color = 0;
    float rotated_angle = 0;

    Point sun_center = Point(0,0);

    void generate_points() override;

public:
    Uint32 colors[2] = {0,0};

    Sun(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_sun, Uint32 color_rays);

    void draw(SDL_Surface* surface) override;
    void translate(double dx, double dy) override;
    void rotate_figure(double angle) override;

    void scale(double sx, double sy) override;                   // T = T * Tr
};

#endif
