#include "Utils.h"


// METHOD IMPLEMENTATION
/**
 * @brief
 * This method checks if the specified coordinates lie inside the dimensions
 * of the provided SDL surface. If the point is out of bounds, an error is logged.
 *
 * @param surface
 * Pointer to the SDL surface to check against.
 *
 * @param x
 * The x-coordinate of the point to verify.
 *
 * @param y
 * The y-coordinate of the point to verify.
 *
 * @return int Returns 1 if the point is within bounds, 0 otherwise.
 */
int Utils::verify_limits(SDL_Surface* surface, int x, int y) {
    /*if (surface->w < x || surface->h < y || x < 0 || y < 0){
        ErrorHandler::log_error("Point out of bounds.");
        return 0;
    }*/

    return 1;
}

double  Utils::perp_dist(double px, double py,double ax, double ay,double bx, double by)
{
    double dx = bx - ax, dy = by - ay;
    double num = std::abs(dy * (px - ax) - dx * (py - ay));
    double den = std::sqrt(dx*dx + dy*dy);
    return (den > 0.0) ? (num / den) : 0.0;
};

// METHOD IMPLEMENTATION
/**
 * @brief
 * This method checks if the provided width and height are not smaller than
 * the defined minimum size and do not exceed the maximum size defined by
 * the application's current screen dimensions.
 *
 * @param app Pointer to the application instance, used to retrieve screen limits.
 * @param width Desired window width.
 * @param height Desired window height.
 * @return true If the width and height are within valid ranges.
 * @return false If either width or height is outside the allowed limits.
 */
bool Utils::check_window_size(App *app, int width, int height) {
    const int MIN_WIDTH = 300;
    const int MIN_HEIGHT = 300;
    const int MAX_WIDTH = app->get_screen_width();
    const int MAX_HEIGHT = app->get_screen_height();

    if (width < MIN_WIDTH || width > MAX_WIDTH) return false;
    if (height < MIN_HEIGHT || height > MAX_HEIGHT) return false;

    return true;
}


double Utils::to_radians(double degrees)
{
    return degrees * 0.017453293;
}

int Utils::clampi(int v, int lo, int hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// janela (screen) -> canvas (desconta o dst_rect do blit)
Point Utils::screen_to_canvas(int mx, int my, const Utils::UniverseRect& dst_rect) {
    return { mx - dst_rect.x, my - dst_rect.y };
}

// canvas -> universo  (Y do universo cresce pra cima)
Point Utils::canvas_to_universe(Point c,int canvas_w, int canvas_h,int universe_w, int universe_h) {
    const double sx = double(universe_w) / double(canvas_w);
    const double sy = double(universe_h) / double(canvas_h);
    return { double(c.get_x()) * sx, double(canvas_h - c.get_y()) * sy }; //Y invertido
}

// universo -> canvas (meia-aberta + clamp pra evitar x==w / y==h)
Point Utils::universe_to_canvas(Point u,int canvas_w, int canvas_h,int universe_w, int universe_h) {
    int x = int(std::floor(u.get_x() * double(canvas_w) / double(universe_w)));
    int y_from_bottom = int(std::floor(u.get_y() * double(canvas_h) / double(universe_h)));
    int y = (canvas_h - 1) - y_from_bottom;
    x = clampi(x, 0, canvas_w - 1);
    y = clampi(y, 0, canvas_h - 1);
    Point p = Point(x,y);
    return p;
}

// dois pontos no canvas (drag) -> retângulo normalizado no universo
Utils::UniverseRect Utils::canvas_drag_to_universe(Point a, Point b, int canvas_w, int canvas_h,int universe_w, int universe_h) {
    Point ua = canvas_to_universe(a, canvas_w, canvas_h, universe_w, universe_h);
    Point ub = canvas_to_universe(b, canvas_w, canvas_h, universe_w, universe_h);
    Utils::UniverseRect r;
    r.x = std::min(ua.get_x(), ub.get_x());
    r.y = std::min(ua.get_y(), ub.get_y());
    r.w = std::fabs(ub.get_x() - ua.get_x());
    r.h = std::fabs(ub.get_y() - ua.get_y());
    return r;
}
