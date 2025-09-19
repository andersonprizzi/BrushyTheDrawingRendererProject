#include "House.h"
#include "Utils.h"
#include "Primitives.h"
#include <SDL.h>
#include <math.h>

House::House(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_walls, Uint32 color_door, Uint32 color_roof) {
    this->width       = width;
    this->height      = height;
    this->x_origin    = universe_x_origin;
    this->y_origin    = universe_y_origin;
    this->walls_color = color_walls;
    this->door_color  = color_door;
    this->roof_color  = color_roof;

    this->colors[0] = color_walls;
    this->colors[1] = color_door;
    this->colors[2] = color_roof;

    this->generate_points();
    //reset_transform();
}

void House::generate_points() {
    this->wall_top_left     = Point(this->x_origin + 0.0 * this->width, this->y_origin + 0.5 * this->height);
    this->wall_bottom_right = Point(this->x_origin + 1.0 * this->width, this->y_origin + 0.0 * this->height);
    this->wall_top_right    = Point(this->wall_bottom_right.get_x(),this->wall_top_left.get_y());
    this->wall_bottom_left  = Point(this->wall_top_left.get_x(), this->wall_bottom_right.get_y());

    this->door_top_left     = Point(this->x_origin + 0.4 * this->width, this->y_origin + 0.25 * this->height);
    this->door_top_right    = Point(this->x_origin + 0.6 * this->width, this->y_origin + 0.25 * this->height);
    this->door_bottom_left  = Point(this->door_top_left.get_x(), this->y_origin + 0.0 * this->height);
    this->door_bottom_right = Point(this->door_top_right.get_x(), this->y_origin + 0.0 * this->height);

    this->roof_peak         = Point(this->x_origin + 0.5 * this->width, this->y_origin + 1.0 * this->height);

    this->wall_fill = Point(this->x_origin + 0.1 * this->width, this->y_origin + 0.4 * this->height);
    this->door_fill = Point(this->x_origin + 0.5 * this->width, this->y_origin + 0.22 * this->height);
    this->roof_fill = Point(this->x_origin + 0.5 * this->width, this->y_origin + 0.65  * this->height);
}

static inline int clampi(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

static inline int map_x(double universe_x, int device_width, int universe_width) {
    // [0, UW] -> [0, W-1], usando floor
    int x = (int)std::floor(universe_x * (double)device_width / (double)universe_width);
    return clampi(x, 0, device_width - 1);
}

static inline int map_y(double universe_y, int device_height, int universe_height) {
    // eixo Y do "universo" cresce para cima; pixel (0,0) é topo
    // [0, UH] (bottom->top) -> [H-1, 0]
    int y_from_bottom = (int)std::floor(universe_y * (double)device_height / (double)universe_height);
    int y = (device_height - 1) - y_from_bottom;
    return clampi(y, 0, device_height - 1);
}

void House::rotate_figure(double angle)
{
    double x, y, radians, cosTheta, sinTheta, dx, dy;

    dx = (double)this->wall_bottom_left.get_x();
    dy = (double)this->wall_bottom_left.get_y();

    translate(-dx, -dy);

    radians  = Utils::to_radians(angle);
    cosTheta = cos(radians);
    sinTheta = sin(radians);

    Point* pts[] = {
        &this->wall_top_left, &this->wall_top_right, &this->wall_bottom_right, &this->wall_bottom_left,
        &this->door_top_left, &this->door_top_right, &this->door_bottom_right, &this->door_bottom_left,
        &this->roof_peak, &this->wall_fill, &this->door_fill, &this->roof_fill
        // se tiver pontos de fill, pode adicioná-los aqui também
    };

    for(Point* p : pts) {
        x = (double)p->get_x();
        y = (double)p->get_y();
        p->set_x(x * cosTheta - y * sinTheta);
        p->set_y(x * sinTheta + y * cosTheta);
    }

    translate(dx, dy);
}

void House::translate(double dx, double dy){
    double x, y;
    Point* pts[] = {
        &this->wall_top_left, &this->wall_top_right, &this->wall_bottom_right, &this->wall_bottom_left,
        &this->door_top_left, &this->door_top_right, &this->door_bottom_right, &this->door_bottom_left,
        &this->roof_peak, &this->wall_fill, &this->door_fill, &this->roof_fill
        // se tiver pontos de fill, pode adicioná-los aqui também
    };

    for(Point* p : pts) {
        x = (double)p->get_x();
        y = (double)p->get_y();
        p->set_x(x + dx);
        p->set_y(y + dy);
    }
}

void House::scale(double sx, double sy){
    double dx, dy, x, y;
    dx = (double)this->wall_bottom_left.get_x();
    dy = (double)this->wall_bottom_left.get_y();
    this->translate(-dx,-dy);
    Point* pts[] = {
        &this->wall_top_left, &this->wall_top_right, &this->wall_bottom_right, &this->wall_bottom_left,
        &this->door_top_left, &this->door_top_right, &this->door_bottom_right, &this->door_bottom_left,
        &this->roof_peak, &this->wall_fill, &this->door_fill, &this->roof_fill
        // se tiver pontos de fill, pode adicioná-los aqui também
    };

    for(Point* p : pts) {
        x = (double)p->get_x();
        y = (double)p->get_y();
        p->set_x(x * sx);
        p->set_y(y * sy);
    }

    this->translate(dx,dy);
}

void House::change_height(double new_height){
    this->height = new_height;
    House::generate_points();
}

void House::change_width(double new_width){
    this->width = new_width;
    House::generate_points();
}

void House::change_origin(Point new_origin){
    this->x_origin = new_origin.get_x();
    this->y_origin = new_origin.get_y();
    House::generate_points();
}

void House::draw(SDL_Surface* surface) {
    const int device_width  = surface->w;
    const int device_height = surface->h;

    const int universe_width  = App::universe_width;
    const int universe_height = App::universe_height;

    Point wall_top_left      = Utils::universe_to_canvas(this->wall_top_left,      device_width, device_height, universe_width, universe_height);
    Point wall_top_right     = Utils::universe_to_canvas(this->wall_top_right,     device_width, device_height, universe_width, universe_height);
    Point wall_bottom_right  = Utils::universe_to_canvas(this->wall_bottom_right,  device_width, device_height, universe_width, universe_height);
    Point wall_bottom_left   = Utils::universe_to_canvas(this->wall_bottom_left,   device_width, device_height, universe_width, universe_height);

    Point door_top_left      = Utils::universe_to_canvas(this->door_top_left,      device_width, device_height, universe_width, universe_height);
    Point door_top_right     = Utils::universe_to_canvas(this->door_top_right,     device_width, device_height, universe_width, universe_height);
    Point door_bottom_left   = Utils::universe_to_canvas(this->door_bottom_left,   device_width, device_height, universe_width, universe_height);
    Point door_bottom_right  = Utils::universe_to_canvas(this->door_bottom_right,  device_width, device_height, universe_width, universe_height);

    Point roof_peak          = Utils::universe_to_canvas(this->roof_peak,          device_width, device_height, universe_width, universe_height);

    Point wall_fill          = Utils::universe_to_canvas(this->wall_fill,          device_width, device_height, universe_width, universe_height);
    Point door_fill          = Utils::universe_to_canvas(this->door_fill,          device_width, device_height, universe_width, universe_height);
    Point roof_fill          = Utils::universe_to_canvas(this->roof_fill,          device_width, device_height, universe_width, universe_height);

    Primitives::draw_line(surface, wall_bottom_left.get_x(),  wall_bottom_left.get_y(),  wall_top_left.get_x(),    wall_top_left.get_y(),    walls_color, false);
    Primitives::draw_line(surface, wall_top_left.get_x(), wall_top_left.get_y(), wall_top_right.get_x(),   wall_top_right.get_y(),   walls_color, false);
    Primitives::draw_line(surface, wall_top_right.get_x(), wall_top_right.get_y(), wall_bottom_right.get_x(),wall_bottom_right.get_y(),walls_color, false);
    Primitives::draw_line(surface, wall_bottom_right.get_x(), wall_bottom_right.get_y(), wall_bottom_left.get_x(), wall_bottom_left.get_y(), walls_color, false);

    // telhado (arestas até o pico)
    Primitives::draw_line(surface, wall_top_left.get_x(),  wall_top_left.get_y(),  roof_peak.get_x(), roof_peak.get_y(), roof_color, true);
    Primitives::draw_line(surface, wall_top_right.get_x(), wall_top_right.get_y(), roof_peak.get_x(), roof_peak.get_y(), roof_color, true);

    // desenho da porta (polígono)
    Primitives::draw_line(surface, door_bottom_left.get_x(),  door_bottom_left.get_y(), door_top_left.get_x(),   door_top_left.get_y(), door_color, false);
    Primitives::draw_line(surface, door_top_left.get_x(),     door_top_left.get_y(), door_top_right.get_x(),  door_top_right.get_y(), door_color, false);
    Primitives::draw_line(surface, door_top_right.get_x(),    door_top_right.get_y(), door_bottom_right.get_x(),door_bottom_right.get_y(), door_color,false);
    Primitives::draw_line(surface, door_bottom_right.get_x(), door_bottom_right.get_y(), door_bottom_left.get_x(), door_bottom_left.get_y(), door_color, false);

    // flood fills usando os pontos já convertidos
    Primitives::flood_fill(surface, (int)wall_fill.get_x(), (int)wall_fill.get_y(), walls_color);
    Primitives::flood_fill(surface, (int)door_fill.get_x(), (int)door_fill.get_y(), door_color);
    Primitives::flood_fill(surface, (int)roof_fill.get_x(), (int)roof_fill.get_y(), roof_color);
}

