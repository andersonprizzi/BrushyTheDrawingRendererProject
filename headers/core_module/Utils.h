#ifndef UTILS_H
#define UTILS_H

#include "App.h"
#include "Point.h"

class Utils {
    public:
        static int verify_limits(SDL_Surface* surface, int x, int y);
        static bool check_window_size(App *app, int width, int height);

        static double to_radians(double degrees);

        // ========= MATRIZ 2D AFIM (3x3) =========
        typedef struct Matrix {
            double m[3][3];
        } Matrix;

        // Construção/composição
        static Matrix mat_identity();
        static Matrix mat_multiply(const Matrix A, const Matrix B);   // A * B
        static Point  mat_apply(const Matrix M, Point p);             // M * [x y 1]^T

        static Matrix mat_translate(double dx, double dy);            // Tr
        static Matrix mat_rotate_deg(double degrees);                 // R (graus)
        static Matrix mat_scale(double sx, double sy);                // S

        // Aplica pivô: T(-c) * M * T(c)
        static Matrix mat_with_pivot(const Matrix M, Point c);

        struct UniverseRect  { double x, y, w, h; };

        static Point screen_to_canvas(int mx, int my, const UniverseRect& dst_rect);
        static Point clamp_canvas(Point p, int canvas_w, int canvas_h);
        static Point canvas_to_universe(Point c,int canvas_w, int canvas_h,int universe_w, int universe_h);
        static Point universe_to_canvas(Point u,int canvas_w, int canvas_h,int universe_w, int universe_h);
        static UniverseRect canvas_drag_to_universe(Point a, Point b,int canvas_w, int canvas_h,int universe_w, int universe_h);
        static int clampi(int v, int lo, int hi);
};

#endif
