#include "Primitives.h"

// METHOD IMPLEMENTATION
/**
 * @brief
 *      Sets the color of a specific pixel on an SDL_Surface.
 *      This function writes a pixel with the given Uint32 color to the specified
 *      coordinates (x, y) on the provided surface. It performs bounds checking
 *      using Utils::verify_limits to prevent invalid memory access.
 *
 * @param surface
 *      Pointer to the SDL_Surface where the pixel will be set.
 * @param x
 *      X-coordinate of the pixel.
 * @param y
 *      Y-coordinate of the pixel.
 * @param color
 *      Uint32 value representing the color to set (in the surface's pixel format).
 **/
void Primitives::set_pixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    if (!surface) return;
    if (Utils::verify_limits(surface, x, y) == 0) return;

    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / 4;
    pixels[y * pitch + x] = color;
}


// METHOD IMPLEMENTATION
/**
 * @brief
 * Retrieves the color of a pixel from an SDL_Surface.
 * This function reads the pixel at coordinates (x, y) from the given surface
 * and returns it as a Uint32 value. The function handles different bytes-per-pixel
 * formats (1, 2, 3, or 4 bytes) and considers the system's endianness.
 *
 * @param surface
 * Pointer to the SDL_Surface to read from.
 *
 * @param x
 * X-coordinate of the pixel.
 *
 * @param y
 * Y-coordinate of the pixel.
 *
 * @return
 * Uint32 value representing the pixel color. Returns 0 if the coordinates
 * are out of bounds or if an unexpected pixel format is encountered.
 */
Uint32 Primitives::get_pixel(SDL_Surface* surface, int x, int y) {
    if (!Utils::verify_limits(surface, x, y)) {
        return 0;
    }

    int bytes_per_pixel = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bytes_per_pixel;

    if (bytes_per_pixel == 1) {
        return *p;

    } else if (bytes_per_pixel == 2) {
        return *(Uint16*)p;

    } else if (bytes_per_pixel == 3) {
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            return (p[0] << 16) | (p[1] << 8) | p[2];
        } else {
            return p[0] | (p[1] << 8) | (p[2] << 16);
        }

    } else if (bytes_per_pixel == 4) {
        return *(Uint32*)p;

    } else {
        // Unexpected pixel format, return 0. // TODO: Implementar função de erro.
        return 0;
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Blends a color with the background pixel on the surface.
 *
 * This function blends the given line color with the background pixel
 * at (px, py) using the provided intensity for Xiaolin Wu's antialiasing.
 *
 * @param surface Pointer to the SDL_Surface.
 * @param px X-coordinate of the pixel.
 * @param py Y-coordinate of the pixel.
 * @param line_color SDL_Color of the line.
 * @param intensity Float intensity (0.0–1.0) of coverage.
 */
void Primitives::blend_pixel(SDL_Surface* surface, int px, int py, SDL_Color line_color, float intensity) {
    if (!Utils::verify_limits(surface, px, py)) return;

    Uint32 bgPixel = Primitives::get_pixel(surface, px, py);
    SDL_Color bgColor;
    SDL_GetRGBA(bgPixel, surface->format, &bgColor.r, &bgColor.g, &bgColor.b, &bgColor.a);

    float alpha = intensity * (line_color.a / 255.0f);

    Uint8 r = Uint8(bgColor.r * (1 - alpha) + line_color.r * alpha);
    Uint8 g = Uint8(bgColor.g * (1 - alpha) + line_color.g * alpha);
    Uint8 b = Uint8(bgColor.b * (1 - alpha) + line_color.b * alpha);
    Uint8 a = 255;

    Primitives::set_pixel(surface, px, py, SDL_MapRGBA(surface->format, r, g, b, a));
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws a line between two points on an SDL surface.
 *
 * The function automatically selects the most appropriate algorithm:
 * - If the line is a single point, only one pixel is drawn.
 * - If the line is vertical or horizontal, optimized functions are used.
 * - If anti_aliasing is enabled, Xiaolin Wu's algorithm is applied for smooth edges.
 * - Otherwise, Bresenham's algorithm is used for fast rasterization.
 *
 * @param surface       SDL surface where the line will be drawn.
 * @param x1            X coordinate of the first point.
 * @param y1            Y coordinate of the first point.
 * @param x2            X coordinate of the second point.
 * @param y2            Y coordinate of the second point.
 * @param color         Pixel color (mapped to the surface format).
 * @param anti_aliasing If true, draw the line with anti-aliasing; otherwise, use a sharp edge.
 */
void Primitives::draw_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color, bool anti_aliasing){
    if (Utils::verify_limits(surface, x1, y1) == 0) return;
    if (Utils::verify_limits(surface, x2, y2) == 0) return;

    if (x1 == x2){
        if (y1==y2){
            Primitives::set_pixel(surface, x1,y1,color);
            return;
        }

        Primitives::draw_vertical_line(surface, x1, y1, y2, color);

    }else if (y1==y2){
        draw_horizontal_line(surface, x1, x2, y1, color);

    }else if (anti_aliasing) {
        draw_xiaolin_wu_line(surface, x1, y1, x2, y2, color);

    } else {
        draw_bresenham_line(surface, x1, y1, x2, y2, color);
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws a horizontal line on the given surface.
 *
 * This function draws a horizontal line from (x1, y) to (x2, y)
 * on the specified SDL_Surface with the given color.
 * If x1 is greater than x2, the values are swapped automatically.
 *
 * @param surface Pointer to the SDL_Surface where the line will be drawn.
 * @param x1 The starting x-coordinate of the line.
 * @param x2 The ending x-coordinate of the line.
 * @param y The y-coordinate of the line.
 * @param color The color of the line, in Uint32 format.
 */
void Primitives::draw_horizontal_line(SDL_Surface* surface, int x1, int x2, int y, Uint32 color) {
    if (x1 > x2){
        int aux = x2;
        x2 = x1;
        x1 = aux;
    }

    for (int x = x1; x < x2; x++)
        Primitives::set_pixel(surface, x, y, color);
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws a vertical line on the given surface.
 *
 * This function draws a vertical line from (x, y1) to (x, y2)
 * on the specified SDL_Surface with the given color.
 * If y1 is greater than y2, the values are swapped automatically.
 *
 * @param surface Pointer to the SDL_Surface where the line will be drawn.
 * @param x The x-coordinate of the line.
 * @param y1 The starting y-coordinate of the line.
 * @param y2 The ending y-coordinate of the line.
 * @param color The color of the line, in Uint32 format.
 */
void Primitives::draw_vertical_line(SDL_Surface* surface,int x, int y1, int y2, Uint32 color) {
    if (y1 > y2){
        int aux = y2;
        y2 = y1;
        y1 = aux;
    }

    for (int y = y1; y < y2; y++) {
        Primitives::set_pixel(surface, x, y, color);
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws a line between two points using Bresenham's algorithm.
 *
 * This function draws a line from (x1, y1) to (x2, y2) on the specified
 * SDL_Surface using Bresenham's line drawing algorithm, which ensures
 * a straight and efficient line regardless of slope. The line is drawn
 * pixel by pixel using the given color.
 *
 * @param surface Pointer to the SDL_Surface where the line will be drawn.
 * @param x1 The x-coordinate of the starting point.
 * @param y1 The y-coordinate of the starting point.
 * @param x2 The x-coordinate of the ending point.
 * @param y2 The y-coordinate of the ending point.
 * @param color The color of the line, in Uint32 format.
 */
void Primitives::draw_bresenham_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        Primitives::set_pixel(surface, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws an anti-aliased colored line using Xiaolin Wu's algorithm.
 *
 * This version blends the line color with the background using
 * the Xiaolin Wu antialiasing technique.
 *
 * @param surface Pointer to the SDL_Surface to draw on.
 * @param x1 Starting x-coordinate.
 * @param y1 Starting y-coordinate.
 * @param x2 Ending x-coordinate.
 * @param y2 Ending y-coordinate.
 * @param color Uint32 color (mapped to surface format) of the line.
 */
void Primitives::draw_xiaolin_wu_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color) {
    if (!surface) return;

    SDL_Color lineColor;
    SDL_GetRGBA(color, surface->format, &lineColor.r, &lineColor.g, &lineColor.b, &lineColor.a);

    bool steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        int aux;

        aux = y1;
        y1 = x1;
        x1 = aux;

        aux = y2;
        y2 = x2;
        x2 = aux;
    }

    if (x1 > x2) {
        int aux;

        aux = x2;
        x2 = x1;
        x1 = aux;

        aux = y2;
        y2 = y1;
        y1 = aux;
    }

    float dx = float(x2 - x1);
    float dy = float(y2 - y1);
    float gradient = (dx == 0.0f) ? 1.0f : dy / dx;

    float intersectY = float(y1);

    for (int x = x1; x <= x2; x++) {
        int y = int(intersectY);
        float f = intersectY - y;

        if (steep) {
            Primitives::blend_pixel(surface, y, x, lineColor, 1 - f);
            Primitives::blend_pixel(surface, y + 1, x, lineColor, f);
        } else {
            Primitives::blend_pixel(surface, x, y, lineColor, 1 - f);
            Primitives::blend_pixel(surface, x, y + 1, lineColor, f);
        }

        intersectY += gradient;
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws a cubic Bézier curve on an SDL_Surface with optional anti-aliasing.
 *
 * This function interpolates a cubic Bézier curve defined by four control points
 * (x0, y0), (x1, y1), (x2, y2), (x3, y3) and draws it on the provided SDL_Surface.
 * The curve can be drawn with or without anti-aliasing, which smooths the edges
 * by blending pixel colors based on their distance from the ideal curve line.
 *
 * @param surface Pointer to the SDL_Surface where the curve will be drawn.
 * @param x0 X-coordinate of the first control point.
 * @param y0 Y-coordinate of the first control point.
 * @param x1 X-coordinate of the second control point.
 * @param y1 Y-coordinate of the second control point.
 * @param x2 X-coordinate of the third control point.
 * @param y2 Y-coordinate of the third control point.
 * @param x3 X-coordinate of the fourth control point.
 * @param y3 Y-coordinate of the fourth control point.
 * @param color Uint32 color of the curve in the surface's pixel format.
 * @param anti_aliasing Boolean flag to enable or disable anti-aliasing.
 *
 * @note The function automatically adjusts the number of interpolation steps
 *       based on the maximum delta between the first and last control points
 *       to ensure smooth rendering even for steep curves.
 */
void Primitives::draw_curve(SDL_Surface* surface, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color, bool anti_aliasing) {
    if (Utils::verify_limits(surface, x0, y0) == 0) return;
    if (Utils::verify_limits(surface, x1, y1) == 0) return;
    if (Utils::verify_limits(surface, x2, y2) == 0) return;
    if (Utils::verify_limits(surface, x3, y3) == 0) return;

    double xu = 0.0, yu = 0.0;

    const int steps = std::max(abs(x3-x0), abs(y3-y0)) * 3;

    for(int i = 0; i <= steps; i++) {
        double u = i / (double)steps;
        xu = pow(1-u,3)*x0+3*u*pow(1-u,2)*x1+3*pow(u,2)*(1-u)*x2+pow(u,3)*x3;
        yu = pow(1-u,3)*y0+3*u*pow(1-u,2)*y1+3*pow(u,2)*(1-u)*y2+pow(u,3)*y3;

        int x_int = int(xu);
        int y_int = int(yu);

        if (anti_aliasing) {
            float fx = float(xu - x_int);
            float fy = float(yu - y_int);

            // Blend pixels based on distance to the exact curve point.
            Primitives::blend_pixel(surface, x_int,     y_int,     Colors::uint32_to_sdlcolor(surface, color), (1.0f - fx) * (1.0f - fy));
            Primitives::blend_pixel(surface, x_int + 1, y_int,     Colors::uint32_to_sdlcolor(surface, color), fx * (1.0f - fy));
            Primitives::blend_pixel(surface, x_int,     y_int + 1, Colors::uint32_to_sdlcolor(surface, color), (1.0f - fx) * fy);
            Primitives::blend_pixel(surface, x_int + 1, y_int + 1, Colors::uint32_to_sdlcolor(surface, color), fx * fy);
        } else {
            Primitives::set_pixel(surface, x_int, y_int, color);
        }
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws a circle on an SDL_Surface with optional anti-aliasing and fill.
 *
 * Without anti-aliasing, the circle uses Bresenham's algorithm for fast integer pixel drawing.
 * With anti-aliasing, the circle smooths the edges based on the distance of each pixel from the exact radius.
 *
 * @param surface Pointer to the SDL_Surface where the circle will be drawn.
 * @param cx X-coordinate of the circle center.
 * @param cy Y-coordinate of the circle center.
 * @param radius Radius of the circle.
 * @param color Uint32 color of the circle in the surface's pixel format.
 * @param anti_aliasing If true, the circle border will be smoothed using anti-aliasing.
 * @param filled If true, the circle will be filled.
 */
void Primitives::draw_circle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 color, bool anti_aliasing, bool filled) {
    SDL_Color lineColor;
    SDL_GetRGBA(color, surface->format, &lineColor.r, &lineColor.g, &lineColor.b, &lineColor.a);

    if (anti_aliasing) {
        SDL_Color lineColor = Colors::uint32_to_sdlcolor(surface, color);

        int x_min = cx - radius - 1;
        int x_max = cx + radius + 1;
        int y_min = cy - radius - 1;
        int y_max = cy + radius + 1;

        for (int py = y_min; py <= y_max; py++) {
            for (int px = x_min; px <= x_max; px++) {
                float dx = px - cx;
                float dy = py - cy;
                float dist = sqrtf(dx * dx + dy * dy);

                if (filled || (dist <= radius + 1.0f && dist >= radius - 1.0f)) {
                    float coverage = radius + 0.5f - dist;
                    if (coverage > 1.0f) coverage = 1.0f;
                    if (coverage < 0.0f) coverage = 0.0f;

                    if (coverage > 0.0f) {
                        Uint32 bgPixel = Primitives::get_pixel(surface, px, py);
                        SDL_Color bgColor = Colors::uint32_to_sdlcolor(surface, bgPixel);

                        Uint8 r = Uint8(bgColor.r * (1.0f - coverage) + lineColor.r * coverage);
                        Uint8 g = Uint8(bgColor.g * (1.0f - coverage) + lineColor.g * coverage);
                        Uint8 b = Uint8(bgColor.b * (1.0f - coverage) + lineColor.b * coverage);

                        Uint32 newColor = Colors::rgb_to_uint32(surface, r, g, b);
                        Primitives::set_pixel(surface, px, py, newColor);
                    }
                }
            }
        }
    } else {
        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y) {
            Primitives::set_pixel(surface, cx + x, cy + y, color);
            Primitives::set_pixel(surface, cx + y, cy + x, color);
            Primitives::set_pixel(surface, cx - y, cy + x, color);
            Primitives::set_pixel(surface, cx - x, cy + y, color);
            Primitives::set_pixel(surface, cx - x, cy - y, color);
            Primitives::set_pixel(surface, cx - y, cy - x, color);
            Primitives::set_pixel(surface, cx + y, cy - x, color);
            Primitives::set_pixel(surface, cx + x, cy - y, color);

            if (filled) {
                for (int y = -radius; y <= radius; y++) {
                    int dx = (int)std::sqrt(radius * radius - y * y);
                    Primitives::draw_line(surface, cx - dx, cy + y, cx + dx, cy + y, color, anti_aliasing);
                }
            }

            y++;
            if (err <= 0) err += 2 * y + 1;
            if (err > 0) { x--; err -= 2 * x + 1; }
        }
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws an ellipse on an SDL surface, optionally with anti-aliasing.
 *
 * This function selects the appropriate drawing method based on the anti_aliasing flag:
 * - If anti_aliasing is true, the ellipse is drawn using supersampling (SSAA) for smooth edges.
 * - If anti_aliasing is false, the ellipse is drawn using Bresenham's algorithm for fast rasterization.
 *
 * The ellipse is centered at (cx, cy) with horizontal radius rx and vertical radius ry.
 * It can be filled or drawn as an outline based on the filled parameter.
 *
 * @param surface       SDL surface where the ellipse will be drawn.
 * @param cx            X coordinate of the ellipse center.
 * @param cy            Y coordinate of the ellipse center.
 * @param rx            Horizontal radius of the ellipse.
 * @param ry            Vertical radius of the ellipse.
 * @param color         Pixel color (mapped to the surface format).
 * @param anti_aliasing If true, draw the ellipse with anti-aliasing; otherwise, use Bresenham's algorithm.
 * @param filled        If true, fill the ellipse; if false, draw only the outline.
 */
void Primitives::draw_ellipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, Uint32 color, bool anti_aliasing, bool filled) {
    if (!Utils::verify_limits(surface, cx + rx, cy + ry)) return;
    if (!Utils::verify_limits(surface, cx - rx, cy - ry)) return;

    if (anti_aliasing) {
        Primitives::draw_supersampled_ellipse(surface, cx, cy, rx, ry, color, filled);
    } else {
        Primitives::draw_bresenham_ellipse(surface, cx, cy, rx, ry, color, filled);
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws an ellipse on an SDL surface using the Bresenham midpoint algorithm.
 *
 * This function plots an ellipse centered at (cx, cy) with with horizontal (rx)
 * and vertical (ry) radii. It supports both outlined and filled ellipses by plotting
 * symmetric points in all four quadrants. The algorithm is integer-based and
 * efficient for raster graphics.
 *
 * @param surface SDL surface where the ellipse will be drawn.
 * @param cx      X coordinate of the ellipse center.
 * @param cy      Y coordinate of the ellipse center.
 * @param rx      Horizontal radius of the ellipse.
 * @param ry      Vertical radius of the ellipse.
 * @param color   Pixel color (mapped to the surface format).
 * @param filled  If true, the ellipse is filled; otherwise, only the outline is drawn.
 */
void Primitives::draw_bresenham_ellipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, Uint32 color, bool filled) {
    int x = 0;
    int y = ry;

    int rx2 = rx * rx;
    int ry2 = ry * ry;
    int twoRx2 = 2 * rx2;
    int twoRy2 = 2 * ry2;

    int px = 0;
    int py = twoRx2 * y;

    int p = (int)(ry2 - (rx2 * ry) + 0.25f * rx2);

    while (px < py) {
        Primitives::set_pixel(surface, cx + x, cy + y, color);
        Primitives::set_pixel(surface, cx - x, cy + y, color);
        Primitives::set_pixel(surface, cx - x, cy - y, color);
        Primitives::set_pixel(surface, cx + x, cy - y, color);

        if (filled) {
            int fx;
            for (fx = cx - x + 1; fx < cx + x; fx++) {
                Primitives::set_pixel(surface, fx, cy + y, color);
                Primitives::set_pixel(surface, fx, cy - y, color);
            }
        }

        x++;
        px += twoRy2;

        if (p < 0) {
            p += ry2 + px;
        } else {
            y--;
            py -= twoRx2;
            p += ry2 + px - py;
        }
    }

    p = (int)(ry2 * (x + 0.5f) * (x + 0.5f) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);

    while (y >= 0) {
        Primitives::set_pixel(surface, cx + x, cy + y, color);
        Primitives::set_pixel(surface, cx - x, cy + y, color);
        Primitives::set_pixel(surface, cx - x, cy - y, color);
        Primitives::set_pixel(surface, cx + x, cy - y, color);

        if (filled) {
            int fx;
            for (fx = cx - x + 1; fx < cx + x; fx++) {
                Primitives::set_pixel(surface, fx, cy + y, color);
                Primitives::set_pixel(surface, fx, cy - y, color);
            }
        }

        y--;
        py -= twoRx2;

        if (p > 0) {
            p += rx2 - py;
        } else {
            x++;
            px += twoRy2;
            p += rx2 - py + px;
        }
    }
}


// METHOD IMPLEMENTATION
/**
 * @brief Draws an ellipse on an SDL surface using supersampling anti-aliasing (SSAA).
 *
 * This function rasterizes an ellipse centered at (cx, cy) with radii rx (horizontal)
 * and ry (vertical). The edges are anti-aliased by subdividing each pixel into a
 * grid of subpixels (supersampling) and computing the coverage fraction of the ellipse.
 *
 * - If filled is true, the interior of the ellipse is filled.
 * - If filled is false, only the outline of the ellipse is drawn with smooth edges.
 *
 * @param surface SDL surface where the ellipse will be drawn.
 * @param cx      X coordinate of the ellipse center.
 * @param cy      Y coordinate of the ellipse center.
 * @param rx      Horizontal radius of the ellipse.
 * @param ry      Vertical radius of the ellipse.
 * @param color   Pixel color (mapped to the surface format).
 * @param filled  If true, fill the ellipse; if false, draw only the anti-aliased border.
 */
void Primitives::draw_supersampled_ellipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, Uint32 color, bool filled) {
    SDL_Color lineColor = Colors::uint32_to_sdlcolor(surface, color);
    int samples = 4; // Subpixels per axis (4x4 = 16 subpixels per pixel).

    int marginX = rx / samples + 2;
    int marginY = ry / samples + 2;

    int xStart = cx - rx - marginX;
    int xEnd   = cx + rx + marginX;
    int yStart = cy - ry - marginY;
    int yEnd   = cy + ry + marginY;

    for (int y = yStart; y <= yEnd; y++) {
        for (int x = xStart; x <= xEnd; x++) {
            int count = 0;
            for (int sy = 0; sy < samples; sy++) {
                for (int sx = 0; sx < samples; sx++) {
                    float px = x + (sx + 0.5f) / samples - cx;
                    float py = y + (sy + 0.5f) / samples - cy;
                    float value = (px*px)/(rx*rx) + (py*py)/(ry*ry);

                    if (filled) {
                        // Any subpixel within the ellipse.
                        if (value <= 1.0f) count++;
                    } else {
                        // Only subpixels near the edge.
                        float tolerance = 0.07f / samples; // Controls the thickness.
                        if (fabs(value - 1.0f) <= tolerance) count++;
                    }
                }
            }

            float coverage = (float)count / (samples * samples);
            if (coverage > 0.0f) {
                Primitives::blend_pixel(surface, x, y, lineColor, coverage);
            }
        }
    }
}


// METHOD IMPLEMENTATION
// Essa função pega uma string, renderiza-a como imagem (com a fonte e cor especificadas) e desenha esse texto na superfície alvo na posição (x, y)
void Primitives::draw_text(SDL_Surface* target, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
    if (!target || !font) return;

    SDL_Surface* text_surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!text_surf) return;

    SDL_Rect dest = { x, y, text_surf->w, text_surf->h };
    SDL_BlitSurface(text_surf, NULL, target, &dest);
    SDL_FreeSurface(text_surf);
}


// METHOD IMPLEMENTATION
// Flood fill com  (algoritmo de preenchimento)
// Pinta shape com base na cor, usando o pixel de referência (x,y) como cor dos pixels que será substituída pela fillColor
// Se a pilha enche, o algoritmo apenas deixa de pintar pixels que precisariam ser processados, não dá erro.
void Primitives::flood_fill(SDL_Surface* surface, int x, int y, Uint32 fill_color) {
    if (!surface) return;

    Uint32 targetColor = get_pixel(surface, x, y);
    if (targetColor == fill_color) return;

    // Buffer para a pilha (simples, tamanho fixo)
    const int MAX_STACK = 100000;
    int stack_x[MAX_STACK];
    int stack_y[MAX_STACK];
    int stack_top = 0;

    // Push inicial
    stack_x[stack_top] = x;
    stack_y[stack_top] = y;
    stack_top++;

    while (stack_top > 0) {
        // Pop
        stack_top--;
        int px = stack_x[stack_top];
        int py = stack_y[stack_top];

        // Limites da surface
        if (px < 0 || px >= surface->w || py < 0 || py >= surface->h) continue;
        if (get_pixel(surface, px, py) != targetColor) continue;

        // Pinta pixel
        set_pixel(surface, px, py, fill_color);

        // Push vizinhos
        if (stack_top + 4 < MAX_STACK) {
            stack_x[stack_top] = px + 1;
            stack_y[stack_top] = py;
            stack_top++;
            stack_x[stack_top] = px - 1;
            stack_y[stack_top] = py;
            stack_top++;
            stack_x[stack_top] = px;
            stack_y[stack_top] = py + 1;
            stack_top++;
            stack_x[stack_top] = px;
            stack_y[stack_top] = py - 1;
            stack_top++;
        }
    }
}
