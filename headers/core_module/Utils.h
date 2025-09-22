#ifndef UTILS_H
#define UTILS_H

#include "App.h"
#include "Point.h"

class Utils {
    public:
        static int verify_limits(SDL_Surface* surface, int x, int y);
        static bool check_window_size(App *app, int width, int height);

        static double to_radians(double degrees);

        static double perp_dist(double px, double py,double ax, double ay,double bx, double by);
        struct UniverseRect  { double x, y, w, h; };

        static Point screen_to_canvas(int mx, int my, const UniverseRect& dst_rect);
        static Point clamp_canvas(Point p, int canvas_w, int canvas_h);
        static Point canvas_to_universe(Point c,int canvas_w, int canvas_h,int universe_w, int universe_h);
        static Point universe_to_canvas(Point u,int canvas_w, int canvas_h,int universe_w, int universe_h);
        static UniverseRect canvas_drag_to_universe(Point a, Point b,int canvas_w, int canvas_h,int universe_w, int universe_h);
        static int clampi(int v, int lo, int hi);
};

#endif
