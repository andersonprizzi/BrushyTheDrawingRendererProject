#include "Sun.h"
#include "Utils.h"
#include "Primitives.h"
#include <SDL.h>
#include <math.h>

Sun::Sun(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_sun, Uint32 color_rays) {
    this->width       = width;
    this->height      = height;
    this->x_origin    = universe_x_origin;
    this->y_origin    = universe_y_origin;
    this->sun_color = color_sun;
    this->sunrays_color  = color_rays;

    this->colors[0] = color_sun;
    this->colors[1] = color_rays;

    this->generate_points();
}

void Sun::generate_points() {
    this->sun_center = Point(this->x_origin + 0.5 * this->width, this->y_origin + 0.5 * this->height);
}

void Sun::rotate_figure(double angle)
{
    angle *= -1;
    double x, y, radians, cosTheta, sinTheta, dx, dy;

    dx = (double)this->sun_center.get_x();
    dy = (double)this->sun_center.get_y();

    translate(-dx, -dy);

    radians  = Utils::to_radians(angle);

    this->rotated_angle  += radians;
    this->rotated_angle  = fmod(this->rotated_angle , 6.28318530717958647692); //2 * pi
    if (this->rotated_angle < 0.0) this->rotated_angle  += 6.28318530717958647692; //2 * pi

    cosTheta = cos(radians);
    sinTheta = sin(radians);

    x = (double)this->sun_center.get_x();
    y = (double)this->sun_center.get_y();
    this->sun_center.set_x(x * cosTheta - y * sinTheta);
    this->sun_center.set_y(x * sinTheta + y * cosTheta);

    translate(dx, dy);
}

void Sun::translate(double dx, double dy){
    double x, y;

    x = (double)this->sun_center.get_x();
    y = (double)this->sun_center.get_y();
    this->sun_center.set_x(x + dx);
    this->sun_center.set_y(y + dy);

}

void Sun::scale(double sx, double sy){
    double dx, dy, x, y, radians, cosTheta, sinTheta;

    // ancora no canto inferior esquerdo (mesmo do rotate)
    dx = (double)this->sun_center.get_x();
    dy = (double)this->sun_center.get_y();
    this->translate(-dx, -dy);

    // usa a rotacao atual da arvore como referencial local
    radians  = this->rotated_angle;
    cosTheta = cos(radians);
    sinTheta = sin(radians);

    // leva o ponto para o referencial LOCAL (desrotaciona por -radians)
    x = (double)this->sun_center.get_x();
    y = (double)this->sun_center.get_y();

    double lx =  x *  cosTheta + y * sinTheta; // R(-theta)
    double ly = -x *  sinTheta + y * cosTheta;

    // escala nos eixos locais
    lx = lx * sx;
    ly = ly * sy;

    // volta para o mundo (rotaciona por +radians)
    double wx = lx * cosTheta - ly * sinTheta; // R(+theta)
    double wy = lx * sinTheta + ly * cosTheta;

    this->sun_center.set_x(wx);
    this->sun_center.set_y(wy);

    this->translate(dx, dy);

    // Atualiza dimensoes logicas (usadas p/ raios das folhas/macas na draw)
    this->width  = (int)lround(this->width  * sx);
    this->height = (int)lround(this->height * sy);
}

#include <cmath>
#include <vector>

void Sun::draw(SDL_Surface* surface) {
    if (!surface) return;

    const int device_width   = surface->w;
    const int device_height  = surface->h;
    const int universe_width = App::universe_width;
    const int universe_height= App::universe_height;

    // Centro no canvas (px)
    Point C = Utils::universe_to_canvas(
        this->sun_center, device_width, device_height, universe_width, universe_height
    );

    // --- raios base no UNIVERSO, agora anisotrópicos ---
    const double ruX = 0.28 * this->width;
    const double ruY = 0.28 * this->height;

    // rx/ry em PX (conversão por eixo)
    Point tmpX = Utils::universe_to_canvas(
        Point(this->sun_center.get_x() + ruX, this->sun_center.get_y()),
        device_width, device_height, universe_width, universe_height
    );
    Point tmpY = Utils::universe_to_canvas(
        Point(this->sun_center.get_x(), this->sun_center.get_y() + ruY),
        device_width, device_height, universe_width, universe_height
    );
    const int rx_px = (int)std::lround(std::fabs(tmpX.get_x() - C.get_x()));
    const int ry_px = (int)std::lround(std::fabs(tmpY.get_y() - C.get_y()));

    // ---------- Raios triangulares (agora usando ruX/ruY) + rotação ----------
    const double PI = 3.14159265358979323846;
    const int    RAY_COUNT       = 16;

    // this->angle em GRAUS (ajuste se já estiver em rad):
    const double base_angle_rad = this->rotated_angle;
    // rotated_angle já está em RAD
    const double rot0            = -PI/2.0 + base_angle_rad + this->rotated_angle;
    const double base_half_angle = (12.0 * PI) / 180.0;   // ~12

    // comprimentos em UNIVERSO, por eixo
    const double base_radius_x = ruX * 0.96;
    const double base_radius_y = ruY * 0.96;
    const double ray_len_x     = ruX * 0.85;
    const double ray_len_y     = ruY * 0.85;

    std::vector<std::pair<int,int>> seeds;
    seeds.reserve(RAY_COUNT);

    for (int i = 0; i < RAY_COUNT; ++i) {
        const double th = rot0 + (2.0 * PI * i) / RAY_COUNT;

        // --- vértices no UNIVERSO, com escala por eixo ---
        Point tip_u(
            this->sun_center.get_x() + (base_radius_x + ray_len_x) * std::cos(th),
            this->sun_center.get_y() + (base_radius_y + ray_len_y) * std::sin(th)
        );
        Point b1_u(
            this->sun_center.get_x() + base_radius_x * std::cos(th - base_half_angle),
            this->sun_center.get_y() + base_radius_y * std::sin(th - base_half_angle)
        );
        Point b2_u(
            this->sun_center.get_x() + base_radius_x * std::cos(th + base_half_angle),
            this->sun_center.get_y() + base_radius_y * std::sin(th + base_half_angle)
        );

        // canvas (px)
        Point tip = Utils::universe_to_canvas(tip_u, device_width, device_height, universe_width, universe_height);
        Point b1  = Utils::universe_to_canvas(b1_u,  device_width, device_height, universe_width, universe_height);
        Point b2  = Utils::universe_to_canvas(b2_u,  device_width, device_height, universe_width, universe_height);

        // arestas (a base será coberta pela elipse)
        Primitives::draw_line(surface, (int)b1.get_x(), (int)b1.get_y(),
                              (int)tip.get_x(), (int)tip.get_y(),
                              this->sunrays_color, /*aa=*/false);
        Primitives::draw_line(surface, (int)b2.get_x(), (int)b2.get_y(),
                              (int)tip.get_x(), (int)tip.get_y(),
                              this->sunrays_color, /*aa=*/false);

        // seed para flood fill (puxado ao ápice)
        const double sx = 0.40*tip.get_x() + 0.30*b1.get_x() + 0.30*b2.get_x();
        const double sy = 0.40*tip.get_y() + 0.30*b1.get_y() + 0.30*b2.get_y();
        seeds.emplace_back((int)std::lround(sx), (int)std::lround(sy));
    }

    // ---------- Elipse central com rx/ry em PX ----------
    Primitives::draw_ellipse(surface, (int)std::lround(C.get_x()), (int)std::lround(C.get_y()),
                             rx_px, ry_px, this->sun_color, false, true);

    // ---------- Flood fill em cada raio ----------
    for (const auto& p : seeds)
        Primitives::flood_fill(surface, p.first, p.second, this->sunrays_color);
}

