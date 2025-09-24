#include "Colors.h"


// STATIC ATTRIBUTE INITIALIZATION
const Colors::ColorItem Colors::drawing_colors_table[] = {
    {"aliceblue",   {240, 248, 255}},
    {"antiquewhite",   {250, 235, 215}},
    {"aqua",   {0, 255, 255}},
    {"aquamarine",   {127, 255, 212}},
    {"azure",   {240, 255, 255}},
    {"beige",   {245, 245, 220}},
    {"bisque",   {255, 228, 196}},
    {"black",   {0, 0, 0}},
    {"blanchedalmond",   {255, 235, 205}},
    {"blue",   {0, 0, 255}},
    {"blueviolet",   {138, 43, 226}},
    {"brown",   {165, 42, 42}},
    {"burlywood",   {222, 184, 135}},
    {"cadetblue",   {95, 158, 160}},
    {"chartreuse",   {127, 255, 0}},
    {"chocolate",   {210, 105, 30}},
    {"coral",   {255, 127, 80}},
    {"cornflowerblue",   {100, 149, 237}},
    {"cornsilk",   {255, 248, 220}},
    {"crimson",   {220, 20, 60}},
    {"cyan",   {0, 255, 255}},
    {"darkblue",   {0, 0, 139}},
    {"darkcyan",   {0, 139, 139}},
    {"darkgoldenrod",   {184, 134, 11}},
    {"darkgray",   {169, 169, 169}},
    {"darkgreen",   {0, 100, 0}},
    {"darkkhaki",   {189, 183, 107}},
    {"darkmagenta",   {139, 0, 139}},
    {"darkolivegreen",   {85, 107, 47}},
    {"darkorange",   {255, 140, 0}},
    {"darkorchid",   {153, 50, 204}},
    {"darkred",   {139, 0, 0}},
    {"darksalmon",   {233, 150, 122}},
    {"darkseagreen",   {143, 188, 143}},
    {"darkslateblue",   {72, 61, 139}},
    {"darkslategray",   {47, 79, 79}},
    {"darkturquoise",   {0, 206, 209}},
    {"darkviolet",   {148, 0, 211}},
    {"deeppink",   {255, 20, 147}},
    {"deepskyblue",   {0, 191, 255}},
    {"dimgray",   {105, 105, 105}},
    {"dodgerblue",   {30, 144, 255}},
    {"firebrick",   {178, 34, 34}},
    {"floralwhite",   {255, 250, 240}},
    {"forestgreen",   {34, 139, 34}},
    {"fuchsia",   {255, 0, 255}},
    {"gainsboro",   {220, 220, 220}},
    {"ghostwhite",   {248, 248, 255}},
    {"gold",   {255, 215, 0}},
    {"goldenrod",   {218, 165, 32}},
    {"gray",   {128, 128, 128}},
    {"green",   {0, 128, 0}},
    {"greenyellow",   {173, 255, 47}},
    {"honeydew",   {240, 255, 240}},
    {"hotpink",   {255, 105, 180}},
    {"indianred",   {205, 92, 92}},
    {"indigo",   {75, 0, 130}},
    {"ivory",   {255, 255, 240}},
    {"khaki",   {240, 230, 140}},
    {"lavender",   {230, 230, 250}},
    {"lavenderblush",   {255, 240, 245}},
    {"lawngreen",   {124, 252, 0}},
    {"lemonchiffon",   {255, 250, 205}},
    {"lightblue",   {173, 216, 230}},
    {"lightcoral",   {240, 128, 128}},
    {"lightcyan",   {224, 255, 255}},
    {"lightgoldenrodyellow",   {250, 250, 210}},
    {"lightgray",   {211, 211, 211}},
    {"lightgreen",   {144, 238, 144}},
    {"lightpink",   {255, 182, 193}},
    {"lightsalmon",   {255, 160, 122}},
    {"lightseagreen",   {32, 178, 170}},
    {"lightskyblue",   {135, 206, 250}},
    {"lightslategray",   {119, 136, 153}},
    {"lightsteelblue",   {176, 196, 222}},
    {"lightyellow",   {255, 255, 224}},
    {"lime",   {0, 255, 0}},
    {"limegreen",   {50, 205, 50}},
    {"linen",   {250, 240, 230}},
    {"magenta",   {255, 0, 255}},
    {"maroon",   {128, 0, 0}},
    {"mediumaquamarine",   {102, 205, 170}},
    {"mediumblue",   {0, 0, 205}},
    {"mediumorchid",   {186, 85, 211}},
    {"mediumpurple",   {147, 112, 219}},
    {"mediumseagreen",   {60, 179, 113}},
    {"mediumslateblue",   {123, 104, 238}},
    {"mediumspringgreen",   {0, 250, 154}},
    {"mediumturquoise",   {72, 209, 204}},
    {"mediumvioletred",   {199, 21, 133}},
    {"midnightblue",   {25, 25, 112}},
    {"mintcream",   {245, 255, 250}},
    {"mistyrose",   {255, 228, 225}},
    {"moccasin",   {255, 228, 181}},
    {"navajowhite",   {255, 222, 173}},
    {"navy",   {0, 0, 128}},
    {"oldlace",   {253, 245, 230}},
    {"olive",   {128, 128, 0}},
    {"olivedrab",   {107, 142, 35}},
    {"orange",   {255, 165, 0}},
    {"orangered",   {255, 69, 0}},
    {"orchid",   {218, 112, 214}},
    {"palegoldenrod",   {238, 232, 170}},
    {"palegreen",   {152, 251, 152}},
    {"paleturquoise",   {175, 238, 238}},
    {"palevioletred",   {219, 112, 147}},
    {"papayawhip",   {255, 239, 213}},
    {"peachpuff",   {255, 218, 185}},
    {"peru",   {205, 133, 63}},
    {"pink",   {255, 192, 203}},
    {"plum",   {221, 160, 221}},
    {"powderblue",   {176, 224, 230}},
    {"purple",   {128, 0, 128}},
    {"rebeccapurple",   {102, 51, 153}},
    {"red",   {255, 0, 0}},
    {"rosybrown",   {188, 143, 143}},
    {"royalblue",   {65, 105, 225}},
    {"saddlebrown",   {139, 69, 19}},
    {"salmon",   {250, 128, 114}},
    {"sandybrown",   {244, 164, 96}},
    {"seagreen",   {46, 139, 87}},
    {"seashell",   {255, 245, 238}},
    {"sienna",   {160, 82, 45}},
    {"silver",   {192, 192, 192}},
    {"skyblue",   {135, 206, 235}},
    {"slateblue",   {106, 90, 205}},
    {"slategray",   {112, 128, 144}},
    {"snow",   {255, 250, 250}},
    {"springgreen",   {0, 255, 127}},
    {"steelblue",   {70, 130, 180}},
    {"tan",   {210, 180, 140}},
    {"teal",   {0, 128, 128}},
    {"thistle",   {216, 191, 216}},
    {"tomato",   {255, 99, 71}},
    {"turquoise",   {64, 224, 208}},
    {"violet",   {238, 130, 238}},
    {"wheat",   {245, 222, 179}},
    {"white",   {255, 255, 255}},
    {"whitesmoke",   {245, 245, 245}},
    {"yellow",   {255, 255, 0}},
    {"yellowgreen",   {154, 205, 50}}
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

