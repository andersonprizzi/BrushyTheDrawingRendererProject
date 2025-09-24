#include "Colors.h"


// STATIC ATTRIBUTE INITIALIZATION
const Colors::ColorItem Colors::drawing_colors_table[] = {
    {"white",   {255, 255, 255}},
    {"silver",  {192, 192, 192}},
    {"gray",    {128, 128, 128}},
    {"black",   {0, 0, 0}},
    {"red",     {255, 0, 0}},
    {"maroon",  {128, 0, 0}},
    {"yellow",  {255, 255, 0}},
    {"olive",   {128, 128, 0}},
    {"lime",    {0, 255, 0}},
    {"green",   {0, 128, 0}},
    {"aqua",    {0, 255, 255}},
    {"teal",    {0, 128, 128}},
    {"blue",    {0, 0, 255}},
    {"navy",    {0, 0, 128}},
    {"fuchsia", {255, 0, 255}},
    {"purple",  {128, 0, 128}},
    {"brown",   {165, 42,  42}},
    {"orange",  {255, 165, 0}}
};


// STATIC ATTRIBUTE INITIALIZATION
const Colors::ColorItem Colors::interface_colors_table[] = {
    {"primary_background_button",     {95, 95, 95}},
    {"secondary_background_button",   {0, 89, 173}},
    {"button_text_color",             {255, 255, 255}},
    {"main_background_window",        {255, 255, 255}},
    {"primary_background_window",     {240, 240, 240}},
};


// STATIC ATTRIBUTES INITIALIZATION
const int Colors::number_of_drawing_colors = sizeof(Colors::drawing_colors_table) / sizeof(Colors::drawing_colors_table[0]);
const int Colors::number_of_interface_colors = sizeof(Colors::interface_colors_table) / sizeof(Colors::interface_colors_table[0]);


// METHOD IMPLEMENTATION
/**
 * @brief
 * This method maps the given red, green, and blue components
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
Uint32 Colors::rgb_to_uint32(SDL_Surface* surface, int r, int g, int b) {
    return SDL_MapRGB(surface->format, r, g, b);
}


// METHOD IMPLEMENTATION
/*
Converte de Uint32 para SDL_COLOR
Essa função faz a conversão de uma cor representada como um valor inteiro de 32 bits (Uint32) para uma estrutura SDL_Color, que tem campos separados para r (vermelho), g (verde), b (azul) e a (alfa).
*/
SDL_Color Colors::uint32_to_sdlcolor(SDL_Surface* surface, Uint32 color) {
    SDL_Color sdlColor;
    SDL_GetRGBA(color, surface->format, &sdlColor.r, &sdlColor.g, &sdlColor.b, &sdlColor.a);
    return sdlColor;
}


// METHOD IMPLEMENTATION
Uint32 Colors::get_color(SDL_Surface* surface, const Colors::ColorItem* color_table, const int table_size, const std::string& color_name) {
    for (int i = 0; i < table_size; i++) {
        if (strcasecmp(color_table[i].name, color_name.c_str()) == 0) {
            return Colors::rgb_to_uint32(
                surface,
                color_table[i].color.r,
                color_table[i].color.g,
                color_table[i].color.b
            );
        }
    }

    // If not found, returns black by default.
    return Colors::rgb_to_uint32(surface, 0, 0, 0);
}

