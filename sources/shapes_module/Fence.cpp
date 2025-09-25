#include "Fence.h"
#include "Utils.h"
#include "Primitives.h"
#include <SDL.h>
#include <math.h>

Fence::Fence(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_plank, Uint32 color_top) {
    this->width       = width;
    this->height      = height;
    this->x_origin    = universe_x_origin;
    this->y_origin    = universe_y_origin;
    this->plank_color = color_plank;
    this->top_color  = color_top;

    this->colors[0] = color_plank;
    this->colors[1] = color_top;


    this->pts = {
        &vert_plank1_bottom_left,  &vert_plank1_bottom_right, &vert_plank1_top_right,  &vert_plank1_top_left,
        &vert_plank2_bottom_left,  &vert_plank2_bottom_right, &vert_plank2_top_right,  &vert_plank2_top_left,
        &hor_plank1_bottom_left,   &hor_plank1_bottom_right,  &hor_plank1_top_right,   &hor_plank1_top_left,
        &hor_plank2_bottom_left,   &hor_plank2_bottom_right,  &hor_plank2_top_right,   &hor_plank2_top_left,
        &top1, &top2,
        &top1_fill, &top2_fill,
        &vertical1_fill, &vertical1_fill2, &vertical1_fill3, &vertical2_fill,
        &hor1_p1_fill, &hor1_p2_fill, &hor1_p3_fill,
        &hor2_p1_fill, &hor2_p2_fill, &hor2_p3_fill, &vertical2_fill2, &vertical2_fill3
    };
    this->generate_points();
}

void Fence::generate_points() {
    this->vert_plank1_bottom_left = Point(this->x_origin + 0.15 * this->width, this->y_origin + 0.0 * this->height);
    this->vert_plank1_bottom_right = Point(this->x_origin + 0.45 * this->width, this->y_origin + 0.0 * this->height);
    this->vert_plank1_top_right = Point(this->x_origin + 0.45 * this->width, this->y_origin + 0.85 * this->height);
    this->vert_plank1_top_left = Point(this->x_origin + 0.15 * this->width, this->y_origin + 0.85 * this->height);

    this->vert_plank2_bottom_left = Point(this->x_origin + 0.55 * this->width, this->y_origin + 0.0 * this->height);
    this->vert_plank2_bottom_right = Point(this->x_origin + 0.85 * this->width, this->y_origin + 0.0 * this->height);
    this->vert_plank2_top_right = Point(this->x_origin + 0.85 * this->width, this->y_origin + 0.85 * this->height);
    this->vert_plank2_top_left = Point(this->x_origin + 0.55 * this->width, this->y_origin + 0.85 * this->height);

    this->top1 = Point(this->x_origin + 0.3 * this->width, this->y_origin + 1 * this->height);
    this->top2 = Point(this->x_origin + 0.7 * this->width, this->y_origin + 1 * this->height);

    this->hor_plank1_bottom_left = Point(this->x_origin + 0 * this->width, this->y_origin + 0.466 * this->height);
    this->hor_plank1_bottom_right = Point(this->x_origin + 1 * this->width, this->y_origin + 0.466 * this->height);
    this->hor_plank1_top_right = Point(this->x_origin + 1 * this->width, this->y_origin + 0.766 * this->height);
    this->hor_plank1_top_left = Point(this->x_origin + 0 * this->width, this->y_origin + 0.766 * this->height);

    this->hor_plank2_bottom_left = Point(this->x_origin + 0 * this->width, this->y_origin + 0.083 * this->height);
    this->hor_plank2_bottom_right = Point(this->x_origin + 1 * this->width, this->y_origin + 0.083 * this->height);
    this->hor_plank2_top_right = Point(this->x_origin + 1 * this->width, this->y_origin + 0.383 * this->height);
    this->hor_plank2_top_left = Point(this->x_origin + 0 * this->width, this->y_origin + 0.383 * this->height);

    this->vertical1_fill = Point(this->x_origin + 0.3 * this->width, this->y_origin + 0.616 * this->height);
    this->vertical1_fill2 = Point(this->x_origin + 0.3 * this->width, this->y_origin + 0.83 * this->height);
    this->vertical1_fill3 = Point(this->x_origin + 0.3 * this->width, this->y_origin + 0.1 * this->height);

    this->vertical2_fill = Point(this->x_origin + 0.7 * this->width, this->y_origin + 0.616 * this->height);
    this->vertical2_fill2 = Point(this->x_origin + 0.65 * this->width, this->y_origin + 0.83 * this->height);
    this->vertical2_fill3 = Point(this->x_origin + 0.75 * this->width, this->y_origin + 0.1 * this->height);

    this->hor1_p1_fill = Point(this->x_origin + 0.1 * this->width, this->y_origin + 0.616 * this->height);
    this->hor1_p2_fill = Point(this->x_origin + 0.5 * this->width, this->y_origin + 0.616 * this->height);
    this->hor1_p3_fill = Point(this->x_origin + 0.9 * this->width, this->y_origin + 0.616 * this->height);
    this->hor2_p1_fill = Point(this->x_origin + 0.1 * this->width, this->y_origin + 0.233 * this->height);
    this->hor2_p2_fill = Point(this->x_origin + 0.5 * this->width, this->y_origin + 0.233 * this->height);
    this->hor2_p3_fill = Point(this->x_origin + 0.9 * this->width, this->y_origin + 0.233 * this->height);
    this->top1_fill = Point(this->x_origin + 0.3 * this->width, this->y_origin + 0.925 * this->height);
    this->top2_fill = Point(this->x_origin + 0.7 * this->width, this->y_origin + 0.925 * this->height);
}

void Fence::rotate_figure(double angle)
{
    angle *= -1;
    double x, y, radians, cosTheta, sinTheta, dx, dy;

    dx = (double)this->vert_plank1_bottom_left.get_x();
    dy = (double)this->vert_plank1_bottom_left.get_y();

    translate(-dx, -dy);

    radians  = Utils::to_radians(angle);

    this->rotated_angle  += radians;
    this->rotated_angle  = fmod(this->rotated_angle , 6.28318530717958647692); //2 * pi
    if (this->rotated_angle < 0.0) this->rotated_angle  += 6.28318530717958647692; //2 * pi

    cosTheta = cos(radians);
    sinTheta = sin(radians);

    for (Point* p : this->pts) {
        if (!p) break;
        x = (double)p->get_x();
        y = (double)p->get_y();
        p->set_x(x * cosTheta - y * sinTheta);
        p->set_y(x * sinTheta + y * cosTheta);
    }

    translate(dx, dy);
}


void Fence::translate(double translation_x, double translation_y){
    double x, y;

    for (Point* p : this->pts) {
        if (!p) break;
        x = (double)p->get_x();
        y = (double)p->get_y();
        p->set_x(x + translation_x);
        p->set_y(y + translation_y);
    }
}

void Fence::scale(double scale_x, double scale_y){
    double dx, dy, x, y, radians, cosTheta, sinTheta;

    // ancora no canto inferior esquerdo (mesmo crit�rio)
    dx = (double)this->vert_plank1_bottom_left.get_x();
    dy = (double)this->vert_plank1_bottom_left.get_y();
    this->translate(-dx, -dy);

    // usa a rota��o atual como referencial local
    radians  = this->rotated_angle;
    cosTheta = cos(radians);
    sinTheta = sin(radians);

    for (Point* p : this->pts) {
        if (!p) break;
        // leva o ponto para o referencial LOCAL (desrotaciona por -radians)
        x = (double)p->get_x();
        y = (double)p->get_y();

        double lx =  x *  cosTheta + y * sinTheta; // R(-theta)
        double ly = -x *  sinTheta + y * cosTheta;

        // escala nos eixos locais
        lx = lx * scale_x;
        ly = ly * scale_y;

        // volta para o mundo (rotaciona por +radians)
        double wx = lx * cosTheta - ly * sinTheta; // R(+theta)
        double wy = lx * sinTheta + ly * cosTheta;

        p->set_x(wx);
        p->set_y(wy);
    }

    this->translate(dx, dy);

    // Atualiza dimens�es l�gicas
    this->width  = (int)lround(this->width  * scale_x);
    this->height = (int)lround(this->height * scale_y);
}




void Fence::draw(SDL_Surface* surface) {
    if (!surface) return;

    const int device_width  = surface->w;
    const int device_height = surface->h;

    const int universe_width  = App::universe_width;
    const int universe_height = App::universe_height;

    // --- Converte todos os pontos para canvas ---
    Point v1_bl = Utils::universe_to_canvas(this->vert_plank1_bottom_left,  device_width, device_height, universe_width, universe_height);
    Point v1_br = Utils::universe_to_canvas(this->vert_plank1_bottom_right, device_width, device_height, universe_width, universe_height);
    Point v1_tr = Utils::universe_to_canvas(this->vert_plank1_top_right,    device_width, device_height, universe_width, universe_height);
    Point v1_tl = Utils::universe_to_canvas(this->vert_plank1_top_left,     device_width, device_height, universe_width, universe_height);

    Point v2_bl = Utils::universe_to_canvas(this->vert_plank2_bottom_left,  device_width, device_height, universe_width, universe_height);
    Point v2_br = Utils::universe_to_canvas(this->vert_plank2_bottom_right, device_width, device_height, universe_width, universe_height);
    Point v2_tr = Utils::universe_to_canvas(this->vert_plank2_top_right,    device_width, device_height, universe_width, universe_height);
    Point v2_tl = Utils::universe_to_canvas(this->vert_plank2_top_left,     device_width, device_height, universe_width, universe_height);

    Point h1_bl = Utils::universe_to_canvas(this->hor_plank1_bottom_left,   device_width, device_height, universe_width, universe_height);
    Point h1_br = Utils::universe_to_canvas(this->hor_plank1_bottom_right,  device_width, device_height, universe_width, universe_height);
    Point h1_tr = Utils::universe_to_canvas(this->hor_plank1_top_right,     device_width, device_height, universe_width, universe_height);
    Point h1_tl = Utils::universe_to_canvas(this->hor_plank1_top_left,      device_width, device_height, universe_width, universe_height);

    Point h2_bl = Utils::universe_to_canvas(this->hor_plank2_bottom_left,   device_width, device_height, universe_width, universe_height);
    Point h2_br = Utils::universe_to_canvas(this->hor_plank2_bottom_right,  device_width, device_height, universe_width, universe_height);
    Point h2_tr = Utils::universe_to_canvas(this->hor_plank2_top_right,     device_width, device_height, universe_width, universe_height);
    Point h2_tl = Utils::universe_to_canvas(this->hor_plank2_top_left,      device_width, device_height, universe_width, universe_height);

    Point t1    = Utils::universe_to_canvas(this->top1, device_width, device_height, universe_width, universe_height);
    Point t2    = Utils::universe_to_canvas(this->top2, device_width, device_height, universe_width, universe_height);

    // --- DESENHO APENAS COM TRIÂNGULOS E RETÂNGULOS PREENCHIDOS ---

    // Verticais (tábuas)
    Primitives::draw_rectangle(surface,
        (int)v1_bl.get_x(), (int)v1_bl.get_y(),
        (int)v1_br.get_x(), (int)v1_br.get_y(),
        (int)v1_tr.get_x(), (int)v1_tr.get_y(),
        (int)v1_tl.get_x(), (int)v1_tl.get_y(),
        this->plank_color);

    Primitives::draw_rectangle(surface,
        (int)v2_bl.get_x(), (int)v2_bl.get_y(),
        (int)v2_br.get_x(), (int)v2_br.get_y(),
        (int)v2_tr.get_x(), (int)v2_tr.get_y(),
        (int)v2_tl.get_x(), (int)v2_tl.get_y(),
        this->plank_color);

    // Horizontais (travessas)
    Primitives::draw_rectangle(surface,
        (int)h1_bl.get_x(), (int)h1_bl.get_y(),
        (int)h1_br.get_x(), (int)h1_br.get_y(),
        (int)h1_tr.get_x(), (int)h1_tr.get_y(),
        (int)h1_tl.get_x(), (int)h1_tl.get_y(),
        this->plank_color);

    Primitives::draw_rectangle(surface,
        (int)h2_bl.get_x(), (int)h2_bl.get_y(),
        (int)h2_br.get_x(), (int)h2_br.get_y(),
        (int)h2_tr.get_x(), (int)h2_tr.get_y(),
        (int)h2_tl.get_x(), (int)h2_tl.get_y(),
        this->plank_color);

    // Topos (triângulos)
    // Cada topo é um triângulo preenchido ligando as duas quinas superiores da tábua vertical ao vértice top.
    Primitives::draw_triangle(surface,
        (int)v1_tl.get_x(), (int)v1_tl.get_y(),
        (int)v1_tr.get_x(), (int)v1_tr.get_y(),
        (int)t1.get_x(),    (int)t1.get_y(),
        this->top_color);

    Primitives::draw_triangle(surface,
        (int)v2_tl.get_x(), (int)v2_tl.get_y(),
        (int)v2_tr.get_x(), (int)v2_tr.get_y(),
        (int)t2.get_x(),    (int)t2.get_y(),
        this->top_color);
}



