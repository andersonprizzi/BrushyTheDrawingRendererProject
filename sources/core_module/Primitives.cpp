#include <stack>
#include <utility>
#include "Primitives.h"


// METHOD IMPLEMENTATION
/**
 * @brief
 * Sets the color of a specific pixel on an SDL_Surface.
 * This function writes a pixel with the given Uint32 color to the specified
 * coordinates (x, y) on the provided surface. It performs bounds checking
 * using Utils::verify_limits to prevent invalid memory access.
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

    if (x >= surface->w || y >= surface->h || x < 0 || y < 0) {
        // ErrorHandler::log_error("Point out of bounds.");
        return;
    }


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
    //if (!Utils::verify_limits(surface, px, py)) return;

    if (px >= surface->w || py >= surface->h || px < 0 || py < 0) {
        // ErrorHandler::log_error("Point out of bounds.");
        return;
    }

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
void Primitives::draw_bezier_curve(SDL_Surface* surface, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color, bool anti_aliasing) {
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

void Primitives::draw_flat_curve(SDL_Surface* surface,
                            int x0, int y0, int x1, int y1,
                            int x2, int y2, int x3, int y3,
                            Uint32 color, bool anti_aliasing) {
    if (!surface) return;
    // Se preferir manter o guard original, ok; mas remover os 4 checks evita "sumir" a curva
    // caso algum controle fique fora, já que vamos desenhar com clipping do draw_line.
    // if (Utils::verify_limits(surface, x0, y0) == 0) return;
    // if (Utils::verify_limits(surface, x1, y1) == 0) return;
    // if (Utils::verify_limits(surface, x2, y2) == 0) return;
    // if (Utils::verify_limits(surface, x3, y3) == 0) return;

    struct V2 { double x, y; };
    auto line_len = [](V2 a, V2 b) {
        double dx = a.x - b.x, dy = a.y - b.y;
        return std::sqrt(dx*dx + dy*dy);
    };
    auto perp_dist = [](V2 p, V2 a, V2 b) {
        // distância perpendicular de p ao segmento (a,b) usando área do paralelogramo
        double dx = b.x - a.x, dy = b.y - a.y;
        double num = std::abs(dy*(p.x - a.x) - dx*(p.y - a.y));
        double den = std::sqrt(dx*dx + dy*dy);
        return (den > 0.0) ? (num / den) : 0.0;
    };

    const double TOL = 0.5;      // tolerância em pixels (0.5 px costuma ser ótimo)
    const int    MAX_DEPTH = 16; // trava p/ evitar recursão infinita

    V2 P0{(double)x0,(double)y0}, P1{(double)x1,(double)y1};
    V2 P2{(double)x2,(double)y2}, P3{(double)x3,(double)y3};

    // desenha um segmento (escolhe AA ou não)
    auto draw_seg = [&](V2 a, V2 b) {
        if (anti_aliasing) {
            Primitives::draw_xiaolin_wu_line(surface,
                (int)std::lround(a.x),(int)std::lround(a.y),
                (int)std::lround(b.x),(int)std::lround(b.y),
                color);
        } else {
            Primitives::draw_bresenham_line(surface,
                (int)std::lround(a.x),(int)std::lround(a.y),
                (int)std::lround(b.x),(int)std::lround(b.y),
                color);
        }
    };

    // subdivide até ficar "plano" o suficiente
    std::function<void(V2,V2,V2,V2,int)> recurse = [&](V2 A, V2 B, V2 C, V2 D, int depth) {
        // critério de planicidade: distâncias de B e C à reta AD
        double d1 = perp_dist(B, A, D);
        double d2 = perp_dist(C, A, D);
        // também evita over-subdividir segmentos já bem pequenos
        double chord = line_len(A, D);

        if ((std::max(d1, d2) <= TOL) || depth >= MAX_DEPTH || chord <= 1.0) {
            draw_seg(A, D);
            return;
        }

        // De Casteljau: subdivisão em duas cúbicas
        V2 AB{(A.x+B.x)*0.5, (A.y+B.y)*0.5};
        V2 BC{(B.x+C.x)*0.5, (B.y+C.y)*0.5};
        V2 CD{(C.x+D.x)*0.5, (C.y+D.y)*0.5};

        V2 ABC{(AB.x+BC.x)*0.5, (AB.y+BC.y)*0.5};
        V2 BCD{(BC.x+CD.x)*0.5, (BC.y+CD.y)*0.5};

        V2 ABCD{(ABC.x+BCD.x)*0.5, (ABC.y+BCD.y)*0.5}; // ponto médio na curva

        // recursão nas duas metades
        recurse(A,   AB,  ABC, ABCD, depth+1);
        recurse(ABCD, BCD, CD,  D,    depth+1);
    };

    recurse(P0, P1, P2, P3, 0);
}

void Primitives::draw_curve(SDL_Surface* surface, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color, bool anti_aliasing) {
    // "Flatness" da cúbica: maior distância dos pontos de controle internos
    // à reta que liga as extremidades (em pixels).
    double d1 = Utils::perp_dist((double)x1, (double)y1, (double)x0, (double)y0, (double)x3, (double)y3);
    double d2 = Utils::perp_dist((double)x2, (double)y2, (double)x0, (double)y0, (double)x3, (double)y3);
    double flatness = std::max(d1, d2);

    //Reference surface size
    const int REF_W = 895;
    const int REF_H = 503;
    //Reference thrheshold
    const double BASE_THR = 10.0;
    const double MIN_THR  = 3.0, MAX_THR = 24.0;

    //Gets the diagonal of the surface
    double diag     = std::hypot(double(surface->w), double(surface->h));
    double diag_ref = std::hypot(double(REF_W),       double(REF_H));

    int thr_px = Utils::clampi(BASE_THR * (diag / diag_ref), MIN_THR, MAX_THR);

    if (flatness <= thr_px) {
        // curva “plana o suficiente” → simples
        Primitives::draw_bezier_curve(surface, x0, y0, x1, y1, x2, y2, x3, y3, color, anti_aliasing);
    } else {
        // curva acentuada → robusta (subdivisão/adaptativa em linhas conectadas)
        Primitives::draw_flat_curve  (surface, x0, y0, x1, y1, x2, y2, x3, y3, color, anti_aliasing);
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


void Primitives::fill_polygon(SDL_Surface* s, const std::vector<SDL_Point>& pts, Uint32 color) {
    if (!s || pts.size() < 3) return;
    const int H = s->h, W = s->w;

    // encontra faixa de Y
    int ymin = pts[0].y, ymax = pts[0].y;
    for (auto &p : pts){ ymin = std::min(ymin, p.y); ymax = std::max(ymax, p.y); }
    ymin = std::max(0, ymin);
    ymax = std::min(H - 1, ymax);

    struct Edge { int y_min, y_max; double x_at_ymin, inv_slope; };
    std::vector<Edge> edges; edges.reserve(pts.size());

    // constrói tabela de arestas (ignora horizontais)
    for (size_t i = 0; i < pts.size(); ++i) {
        SDL_Point a = pts[i], b = pts[(i + 1) % pts.size()];
        if (a.y == b.y) continue;
        if (a.y > b.y) std::swap(a, b); // a.y < b.y
        Edge e;
        e.y_min = a.y;
        e.y_max = b.y;            // tratado como [y_min, y_max) (topo incluso, base exclusiva)
        e.x_at_ymin = a.x;
        e.inv_slope = double(b.x - a.x) / double(b.y - a.y);
        edges.push_back(e);
    }

    // para cada scanline
    for (int y = ymin; y <= ymax; ++y) {
        std::vector<double> xs; xs.reserve(pts.size());

        // coleta interseções com esta linha (y + 0.5 evita ambiguidade em vértices)
        double scan_y = y + 0.5;
        for (const auto& e : edges) {
            if (scan_y >= e.y_min && scan_y < e.y_max) {
                double x = e.x_at_ymin + (scan_y - e.y_min) * e.inv_slope;
                xs.push_back(x);
            }
        }

        if (xs.size() < 2) continue;
        std::sort(xs.begin(), xs.end());

        // pinta pares (regra par-ímpar)
        for (size_t i = 0; i + 1 < xs.size(); i += 2) {
            int xL = (int)std::ceil(xs[i]);
            int xR = (int)std::floor(xs[i + 1]);
            if (xL > xR) continue;
            xL = std::max(0, xL);
            xR = std::min(W - 1, xR);
            for (int x = xL; x <= xR; ++x) {
                Primitives::set_pixel(s, x, y, color);
            }
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
    if (!surface || x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        return;
    }

    Uint32 target_color = get_pixel(surface, x, y);
    if (target_color == fill_color) {
        return;
    }

    // A pilha agora usa pares (x, y) e cresce dinamicamente.
    std::stack<std::pair<int, int>> pixels;
    pixels.push({x, y});

    while (!pixels.empty()) {
        // Pega o pixel do topo da pilha e o remove (pop).
        std::pair<int, int> current = pixels.top();
        pixels.pop();

        int px = current.first;
        int py = current.second;

        // Verifica os limites e a cor do pixel atual.
        if (px < 0 || px >= surface->w || py < 0 || py >= surface->h || get_pixel(surface, px, py) != target_color) {
            continue;
        }

        // Pinta o pixel.
        set_pixel(surface, px, py, fill_color);

        // Adiciona (push) os vizinhos à pilha.
        pixels.push({px + 1, py});
        pixels.push({px - 1, py});
        pixels.push({px, py + 1});
        pixels.push({px, py - 1});
    }
}

// METHOD IMPLEMENTATION
void Primitives::draw_rotated_ellipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, float angle_rad, Uint32 color, bool filled)
{
    if (!surface) return;
    if (rx <= 0 || ry <= 0) return;

    SDL_Color col = Colors::uint32_to_sdlcolor(surface, color);

    float c = cosf(angle_rad);
    float s = sinf(angle_rad);
    int margin = (rx > ry ? rx : ry) + 2;
    int samples = 4;

    // [CORREÇÃO] Calcula os limites da iteração (bounding box na tela)
    int x_start = cx - margin;
    int x_end = cx + margin;
    int y_start = cy - margin;
    int y_end = cy + margin;

    // [CORREÇÃO] Restringe (clamping) os limites para ficarem DENTRO da tela.
    // Isso evita loops gigantes e garante que 'x' e 'y' sempre serão válidos.
    x_start = std::max(0, x_start);
    x_end = std::min(surface->w - 1, x_end);
    y_start = std::max(0, y_start);
    y_end = std::min(surface->h - 1, y_end);

    // O loop agora itera SOMENTE sobre os pixels potencialmente visíveis.
    for (int y = y_start; y <= y_end; ++y) {
        for (int x = x_start; x <= x_end; ++x) {
            int inside = 0;
            for (int sy = 0; sy < samples; ++sy) {
                for (int sx = 0; sx < samples; ++sx) {
                    float dx = (x + (sx + 0.5f) / samples) - cx;
                    float dy = (y + (sy + 0.5f) / samples) - cy;
                    float u = c * dx + s * dy;
                    float v = -s * dx + c * dy;
                    float val = (u * u) / (rx * rx) + (v * v) / (ry * ry);
                    if (filled ? (val <= 1.0f) : (fabsf(val - 1.0f) <= 0.07f)) {
                        inside++;
                    }
                }
            }
            float cov = (float)inside / (samples * samples);
            // A chamada a blend_pixel agora é inerentemente segura, pois x e y
            // estão garantidos dentro dos limites da tela pelo loop.
            if (cov > 0.0f) {
                Primitives::blend_pixel(surface, x, y, col, cov);
            }
        }
    }
}


void Primitives::draw_triangle(SDL_Surface* surface,
                                      int x0, int y0,
                                      int x1, int y1,
                                      int x2, int y2,
                                      Uint32 color)
{
    if (!surface) return;


auto edge_fn = [](int ax, int ay, int bx, int by, int px, int py) -> long long {
        // (By - Ay)*(Px - Ax) - (Bx - Ax)*(Py - Ay)
        return (long long)(by - ay) * (px - ax) - (long long)(bx - ax) * (py - ay);
    };

    long long area2 = edge_fn(x0,y0, x1,y1, x2,y2);
    if (area2 == 0) {
        // Triângulo degenerado: desenha as arestas
        Primitives::draw_bresenham_line(surface, x0, y0, x1, y1, color);
        Primitives::draw_bresenham_line(surface, x1, y1, x2, y2, color);
        Primitives::draw_bresenham_line(surface, x2, y2, x0, y0, color);
        return;
    }

    // BBox inteira (não clampada)
    int minx = std::min(x0, std::min(x1, x2));
    int maxx = std::max(x0, std::max(x1, x2));
    int miny = std::min(y0, std::min(y1, y2));
    int maxy = std::max(y0, std::max(y1, y2));

    // Top-left rule
    auto is_top_left = [](int ax, int ay, int bx, int by) -> bool {
        int dx = bx - ax, dy = by - ay;
        return (dy < 0) || (dy == 0 && dx < 0);
    };

    bool e0_top = is_top_left(x1,y1, x2,y2); // oposta a v0
    bool e1_top = is_top_left(x2,y2, x0,y0); // oposta a v1
    bool e2_top = is_top_left(x0,y0, x1,y1); // oposta a v2

    // Coeficientes integrais da edge function: E(P)=A*x + B*y + C
    struct EC { long long A,B,C; };
    auto coeffs = [](int ax,int ay,int bx,int by) -> EC {
        EC c;
        c.A = (long long)(by - ay);
        c.B = (long long)-(bx - ax);
        // C = bx*ay - by*ax  (forma compatível com A,B acima)
        c.C = (long long)bx * ay - (long long)by * ax;
        return c;
    };

    EC e0 = coeffs(x1,y1, x2,y2);
    EC e1 = coeffs(x2,y2, x0,y0);
    EC e2 = coeffs(x0,y0, x1,y1);

    // Bias para amostrar no centro do pixel: (A*0.5 + B*0.5)
    auto center_bias = [](const EC& e) -> long long {
        // como A e B são inteiros, A/2 + B/2 em aritmética inteira truncada funciona
        // (usar 64 bits evita overflow)
        return (e.A >> 1) + (e.B >> 1);
    };
    long long bias0 = center_bias(e0);
    long long bias1 = center_bias(e1);
    long long bias2 = center_bias(e2);

    for (int y = miny; y <= maxy; ++y) {
        // E(minx + 0.5, y + 0.5) = A*minx + B*y + C + (A/2 + B/2)
        long long E0 = e0.A * (long long)minx + e0.B * (long long)y + e0.C + bias0;
        long long E1 = e1.A * (long long)minx + e1.B * (long long)y + e1.C + bias1;
        long long E2 = e2.A * (long long)minx + e2.B * (long long)y + e2.C + bias2;

        for (int x = minx; x <= maxx; ++x) {
            if (area2 > 0) {
                bool c0 = (E0 > 0) || (E0 == 0 && e0_top);
                bool c1 = (E1 > 0) || (E1 == 0 && e1_top);
                bool c2 = (E2 > 0) || (E2 == 0 && e2_top);
                if (c0 && c1 && c2)
                    Primitives::set_pixel(surface, x, y, color);
            } else {
                bool c0 = (E0 < 0) || (E0 == 0 && e0_top);
                bool c1 = (E1 < 0) || (E1 == 0 && e1_top);
                bool c2 = (E2 < 0) || (E2 == 0 && e2_top);
                if (c0 && c1 && c2)
                    Primitives::set_pixel(surface, x, y, color);
            }

            // próximo pixel em X: E += A
            E0 += e0.A; E1 += e1.A; E2 += e2.A;
        }
    }
}

void Primitives::draw_rectangle(SDL_Surface* surface,
                                int x0, int y0,
                                int x1, int y1,
                                int x2, int y2,
                                int x3, int y3,
                                Uint32 color)
{
    if (!surface) return;

    // Coloca em arrays para facilitar o manuseio por índice.
    int xs[4] = { x0, x1, x2, x3 };
    int ys[4] = { y0, y1, y2, y3 };

    // Escolhe a diagonal: o par com maior distância ao quadrado.
    auto d2 = [&](int i, int j) -> long long {
        long long dx = (long long)xs[i] - (long long)xs[j];
        long long dy = (long long)ys[i] - (long long)ys[j];
        return dx*dx + dy*dy;
    };

    int di = 0, dj = 1;                  // índices da diagonal escolhida
    long long best = d2(0,1);

    const int pairs[6][2] = { {0,1},{0,2},{0,3},{1,2},{1,3},{2,3} };
    for (int p = 0; p < 6; ++p) {
        int i = pairs[p][0], j = pairs[p][1];
        long long cur = d2(i,j);
        if (cur > best) {
            best = cur; di = i; dj = j;
        }
    }

    // Os dois índices restantes (os outros vértices do retângulo).
    int others[2], k = 0;
    for (int i = 0; i < 4; ++i) if (i != di && i != dj) others[k++] = i;

    // Triangula sempre pela diagonal (di)-(dj).
    // Isso cobre o retângulo inteiro sem buracos, independentemente da ordem de entrada.
    Primitives::draw_triangle(surface,
                              xs[di], ys[di],
                              xs[others[0]], ys[others[0]],
                              xs[dj], ys[dj],
                              color);

    Primitives::draw_triangle(surface,
                              xs[di], ys[di],
                              xs[dj], ys[dj],
                              xs[others[1]], ys[others[1]],
                              color);
}
