// INCLUDES
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
    if (surface->w < x || surface->h < y || x < 0 || y < 0){
        ErrorHandler::log_error("Point out of bounds.");
        return 0;
    }

    return 1;
}


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
