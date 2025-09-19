#include "Polygon.h"
#include "Point.h"

Polygon::Polygon(bool anti_aliasing, bool filled, Uint32 border_color, Uint32 fill_color, Point fill_point) : fill_point(fill_point) {
    points.clear();
    this->anti_aliasing = anti_aliasing;
    this->filled = filled;
    this->border_color = border_color;
    this->fill_color = fill_color;
    this->fill_color_set = true;
    this->fill_point = fill_point;
}


Polygon::Polygon(bool anti_aliasing, bool filled, Uint32 border_color) : fill_point(0,0) {
    points.clear();
    this->anti_aliasing = anti_aliasing;
    this->filled = filled;
    this->border_color = border_color;
    this->fill_color_set = false;
    this->fill_point = Point(0,0);
}


void Polygon::add_point(Point p) {
    points.push_back(p);
}

void Polygon::clear_points() {
    points.clear();
}

int Polygon::get_point_count() {
    return (int)points.size();
}

void Polygon::draw(SDL_Surface* surface) {
    if (!surface || points.size() < 2) return;

    for (size_t i = 0; i < points.size(); i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % points.size()];

        int x1 = (int)p1.get_x();
        int y1 = (int)p1.get_y();
        int x2 = (int)p2.get_x();
        int y2 = (int)p2.get_y();

        Primitives::draw_line(surface, x1, y1, x2, y2, this->border_color, this->anti_aliasing);
    }

    // Setting a default color if fill_color was not provided.
    if (this->filled == true && this->fill_color_set == false) {
        this->fill_color = SDL_MapRGB(surface->format, 0, 0, 0);
        // TODO: Set a well-positioned default paint point.
    }

    if (filled == true) {
        Primitives::flood_fill(surface, (int) this->fill_point.get_x(), (int) this->fill_point.get_y(), this->fill_color);
    }
}
