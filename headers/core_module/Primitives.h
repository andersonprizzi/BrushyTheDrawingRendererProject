#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "App.h"

class Primitives {
    private:
        static void draw_horizontal_line(SDL_Surface* surface, int x1, int x2, int y, Uint32 color);
        static void draw_vertical_line(SDL_Surface* surface,int x, int y1, int y2, Uint32 color);
        static void draw_bresenham_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color);
        static void draw_xiaolin_wu_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color);
        static void draw_bresenham_ellipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, Uint32 color, bool filled);
        static void draw_supersampled_ellipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, Uint32 color, bool filled);
        static void draw_bezier_curve(SDL_Surface* surface, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color, bool anti_aliasing);
        static void draw_flat_curve(SDL_Surface* surface, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color, bool anti_aliasing);
    public:
        static void set_pixel(SDL_Surface* surface, int x, int y, Uint32 color);
        static Uint32 get_pixel(SDL_Surface* surface, int x, int y);
        static void blend_pixel(SDL_Surface* surface, int px, int py, SDL_Color line_color, float intensity);
        static void draw_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color, bool anti_aliasing);
        static void draw_curve(SDL_Surface* surface, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color, bool anti_aliasing);
        static void draw_circle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 color, bool anti_aliasing, bool filled);
        static void draw_ellipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, Uint32 color, bool anti_aliasing, bool filled);
        static void draw_rotated_ellipse(SDL_Surface* surface,int cx, int cy, int rx, int ry,float angle_rad, Uint32 color, bool filled);
        static void draw_text(SDL_Surface* target, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color);
        static void fill_polygon(SDL_Surface* s, const std::vector<SDL_Point>& pts, Uint32 color);
        static void flood_fill(SDL_Surface* surface, int x, int y, Uint32 fillColor);
        static void draw_triangle(SDL_Surface* surface,int x0, int y0,int x1, int y1,int x2, int y2,Uint32 color);
        static void draw_rectangle(SDL_Surface* surface,
                                  int x0, int y0,
                                  int x1, int y1,
                                  int x2, int y2,
                                  int x3, int y3,
                                  Uint32 color);
};

#endif
