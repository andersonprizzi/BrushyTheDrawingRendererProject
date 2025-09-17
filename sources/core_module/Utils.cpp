#include "Utils.h"


// Verifica se posição está dentro dos limites da tela
int Utils::verify_limits(SDL_Surface* surface, int x, int y) {
    if (surface->w < x || surface->h < y || x < 0 || y < 0){
        ErrorHandler::log_error("Point out of bounds.");
        return 0;
    }

    return 1;
}


bool Utils::check_window_size(App *app, int width, int height) {
    const int MIN_WIDTH = 300;
    const int MIN_HEIGHT = 300;
    const int MAX_WIDTH = app->get_screen_width();
    const int MAX_HEIGHT = app->get_screen_height();

    if (width < MIN_WIDTH || width > MAX_WIDTH) return false;
    if (height < MIN_HEIGHT || height > MAX_HEIGHT) return false;

    return true;
}
