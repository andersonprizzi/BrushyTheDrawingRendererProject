#include "Utils.h"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

// Verifica se posição está dentro dos limites da tela
int Utils::verify_limits(SDL_Surface* surface, int x, int y) {
    if (surface->w < x || surface->h < y || x < 0 || y < 0){
        printf("ERROR: POINT OUT OF BOUNDS.\n");
        return 0;
    }

    return 1;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//


/**
 * @brief Converts RGB values into a 32-bit unsigned integer color.
 *
 * This function maps the given red, green, and blue components
 * into a single Uint32 value according to the pixel format of
 * the provided SDL_Surface. The resulting color value can then
 * be used to draw on that surface.
 *
 * @param surface Pointer to the SDL_Surface whose pixel format
 *                will be used for the conversion.
 * @param r Red component (0–255).
 * @param g Green component (0–255).
 * @param b Blue component (0–255).
 *
 * @return Uint32 value representing the mapped RGB color.
 */
Uint32 Utils::rgb_to_uint32(SDL_Surface* surface, int r, int g, int b) {
    return SDL_MapRGB(surface->format, r, g, b);
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//


/*
Converte de Uint32 para SDL_COLOR
Essa função faz a conversão de uma cor representada como um valor inteiro de 32 bits (Uint32) para uma estrutura SDL_Color, que tem campos separados para r (vermelho), g (verde), b (azul) e a (alfa).
*/
SDL_Color Utils::uint32_to_sdlcolor(SDL_Surface* surface, Uint32 color) {
    SDL_Color sdlColor;
    SDL_GetRGBA(color, surface->format, &sdlColor.r, &sdlColor.g, &sdlColor.b, &sdlColor.a);
    return sdlColor;
}
