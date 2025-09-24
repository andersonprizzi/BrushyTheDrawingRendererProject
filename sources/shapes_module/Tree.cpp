#include "Tree.h"
#include "Utils.h"
#include "Primitives.h"
#include "App.h" // Necessário para App::universe_width/height
#include <SDL.h>
#include <cmath>
#include <limits> // Para std::numeric_limits

// O Construtor permanece o mesmo
Tree::Tree(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_trunk, Uint32 color_leaves, Uint32 color_apple) {
    this->width = width;
    this->height = height;
    this->x_origin = universe_x_origin;
    this->y_origin = universe_y_origin;
    this->trunk_color = color_trunk;
    this->leaves_color = color_leaves;
    this->apple_color = color_apple;

    this->colors[0] = color_trunk;
    this->colors[1] = color_leaves;
    this->colors[2] = color_apple;

    this->pts = {
        &this->trunk_bottom_left , &this->trunk_bottom_right, &this->trunk_top_right , &this->trunk_top_left,
        &this->trunk_left_bezier_point, &this->trunk_right_bezier_point,
        &this->leaves_first_elipsis_center, &this->leaves_second_elipsis_center, &this->leaves_third_elipsis_center ,
        &this->apple_center, &this->apple2_center, &this->trunk_fill
    };
    this->generate_points();
}

// generate_points() permanece a mesma
void Tree::generate_points() {
    this->trunk_bottom_left = Point(this->x_origin + 0.0 * this->width, this->y_origin + 0.0 * this->height);
    this->trunk_bottom_right = Point(this->x_origin + 1 * this->width, this->y_origin + 0.0 * this->height);
    this->trunk_top_right = Point(this->x_origin + 0.8 * this->width, this->y_origin + 0.75 * this->height);
    this->trunk_top_left = Point(this->x_origin + 0.2 * this->width, this->y_origin + 0.75 * this->height);
    this->trunk_left_bezier_point = Point(this->x_origin + 0.7 * this->width, this->y_origin + 0.25 * this->height);
    this->trunk_right_bezier_point = Point(this->x_origin + 0.3 * this->width, this->y_origin + 0.25 * this->height);
    this->leaves_first_elipsis_center = Point(this->x_origin + 0.5 * this->width, this->y_origin + 0.85 * this->height);
    this->leaves_second_elipsis_center = Point(this->x_origin + 0.25 * this->width, this->y_origin + 0.72 * this->height);
    this->leaves_third_elipsis_center = Point(this->x_origin + 0.75 * this->width, this->y_origin + 0.72 * this->height);
    this->apple_center = Point(this->x_origin + 0.64 * this->width, this->y_origin + 0.69 * this->height);
    this->apple2_center = Point(this->x_origin + 0.32 * this->width, this->y_origin + 0.86 * this->height);
    this->trunk_fill = Point(this->x_origin + 0.5 * this->width, this->y_origin + 0.375 * this->height);
}

// rotate_figure() permanece a mesma
void Tree::rotate_figure(double angle)
{
    angle *= -1;
    double x, y, radians, cosTheta, sinTheta, dx, dy;
    dx = (double)this->trunk_bottom_left.get_x();
    dy = (double)this->trunk_bottom_left.get_y();
    translate(-dx, -dy);
    radians = Utils::to_radians(angle);
    this->rotated_angle += radians;
    this->rotated_angle = fmod(this->rotated_angle, 2.0 * M_PI);
    if (this->rotated_angle < 0.0) this->rotated_angle += 2.0 * M_PI;
    cosTheta = cos(radians);
    sinTheta = sin(radians);
    for(Point* p : pts) {
        x = (double)p->get_x();
        y = (double)p->get_y();
        p->set_x(x * cosTheta - y * sinTheta);
        p->set_y(x * sinTheta + y * cosTheta);
    }
    translate(dx, dy);
}

// translate() permanece a mesma
void Tree::translate(double dx, double dy){
    double x, y;
    for(Point* p : pts) {
        x = (double)p->get_x();
        y = (double)p->get_y();
        p->set_x(x + dx);
        p->set_y(y + dy);
    }
}

// scale() permanece a mesma
void Tree::scale(double sx, double sy){
    double dx, dy, x, y, radians, cosTheta, sinTheta;
    dx = (double)this->trunk_bottom_left.get_x();
    dy = (double)this->trunk_bottom_left.get_y();
    this->translate(-dx, -dy);
    radians = this->rotated_angle;
    cosTheta = cos(radians);
    sinTheta = sin(radians);
    for (Point* p : pts) {
        x = (double)p->get_x();
        y = (double)p->get_y();
        double lx =  x * cosTheta + y * sinTheta;
        double ly = -x * sinTheta + y * cosTheta;
        lx = lx * sx;
        ly = ly * sy;
        double wx = lx * cosTheta - ly * sinTheta;
        double wy = lx * sinTheta + ly * cosTheta;
        p->set_x(wx);
        p->set_y(wy);
    }
    this->translate(dx, dy);
    this->width = (int)lround(this->width * sx);
    this->height = (int)lround(this->height * sy);
}


void Tree::draw(SDL_Surface* surface) {
    if (!surface) return;

    const int device_width = surface->w;
    const int device_height = surface->h;
    const int universe_width = App::universe_width;
    const int universe_height = App::universe_height;

    // --- CONVERSÕES PARA CANVAS ---
    Point c_trunk_bl = Utils::universe_to_canvas(this->trunk_bottom_left, device_width, device_height, universe_width, universe_height);
    Point c_trunk_br = Utils::universe_to_canvas(this->trunk_bottom_right, device_width, device_height, universe_width, universe_height);
    Point c_trunk_tr = Utils::universe_to_canvas(this->trunk_top_right, device_width, device_height, universe_width, universe_height);
    Point c_trunk_tl = Utils::universe_to_canvas(this->trunk_top_left, device_width, device_height, universe_width, universe_height);
    Point c_bezier_l = Utils::universe_to_canvas(this->trunk_left_bezier_point, device_width, device_height, universe_width, universe_height);
    Point c_bezier_r = Utils::universe_to_canvas(this->trunk_right_bezier_point, device_width, device_height, universe_width, universe_height);
    Point c_trunk_fill = Utils::universe_to_canvas(this->trunk_fill, device_width, device_height, universe_width, universe_height);
    Point c_leaves1 = Utils::universe_to_canvas(this->leaves_first_elipsis_center, device_width, device_height, universe_width, universe_height);
    Point c_leaves2 = Utils::universe_to_canvas(this->leaves_second_elipsis_center, device_width, device_height, universe_width, universe_height);
    Point c_leaves3 = Utils::universe_to_canvas(this->leaves_third_elipsis_center, device_width, device_height, universe_width, universe_height);
    Point c_apple1 = Utils::universe_to_canvas(this->apple_center, device_width, device_height, universe_width, universe_height);
    Point c_apple2 = Utils::universe_to_canvas(this->apple2_center, device_width, device_height, universe_width, universe_height);

    // --- TRONCO ---
    Primitives::draw_line(surface, c_trunk_bl.get_x(), c_trunk_bl.get_y(), c_trunk_br.get_x(), c_trunk_br.get_y(), this->trunk_color, false);
    Primitives::draw_line(surface, c_trunk_tl.get_x(), c_trunk_tl.get_y(), c_trunk_tr.get_x(), c_trunk_tr.get_y(), this->trunk_color, false);
    Primitives::draw_curve(surface, c_trunk_bl.get_x(), c_trunk_bl.get_y(), c_bezier_r.get_x(), c_bezier_r.get_y(), c_bezier_r.get_x(), c_bezier_r.get_y(), c_trunk_tl.get_x(), c_trunk_tl.get_y(), this->trunk_color, false);
    Primitives::draw_curve(surface, c_trunk_br.get_x(), c_trunk_br.get_y(), c_bezier_l.get_x(), c_bezier_l.get_y(), c_bezier_l.get_x(), c_bezier_l.get_y(), c_trunk_tr.get_x(), c_trunk_tr.get_y(), this->trunk_color, false);
    Primitives::flood_fill(surface, c_trunk_fill.get_x(), c_trunk_fill.get_y(), this->trunk_color);

    // --- FOLHAS ---
    double rx1_u = 0.38 * this->width, ry1_u = 0.22 * this->height;
    double rx2_u = 0.235 * this->width, ry2_u = 0.18 * this->height;
    double cA = cos(this->rotated_angle);
    double sA = sin(this->rotated_angle);
    int rx_px, ry_px;
    double ang;

    // Elipse 1 (central)
    // [CORREÇÃO] Usar o ponto original do universo (this->...) para o cálculo.
    Point tmp = Utils::universe_to_canvas(Point(this->leaves_first_elipsis_center.get_x() + rx1_u * cA, this->leaves_first_elipsis_center.get_y() + rx1_u * sA), device_width, device_height, universe_width, universe_height);
    Point tmp2 = Utils::universe_to_canvas(Point(this->leaves_first_elipsis_center.get_x() - ry1_u * sA, this->leaves_first_elipsis_center.get_y() + ry1_u * cA), device_width, device_height, universe_width, universe_height);
    rx_px = (int)lround(hypot(tmp.get_x() - c_leaves1.get_x(), tmp.get_y() - c_leaves1.get_y()));
    ry_px = (int)lround(hypot(tmp2.get_x() - c_leaves1.get_x(), tmp2.get_y() - c_leaves1.get_y()));
    ang = atan2(tmp.get_y() - c_leaves1.get_y(), tmp.get_x() - c_leaves1.get_x());
    Primitives::draw_rotated_ellipse(surface, c_leaves1.get_x(), c_leaves1.get_y(), rx_px, ry_px, ang, this->leaves_color, true);

    // Elipse 2 (esquerda)
    // [CORREÇÃO] Usar o ponto original do universo (this->...) para o cálculo.
    tmp = Utils::universe_to_canvas(Point(this->leaves_second_elipsis_center.get_x() + rx2_u * cA, this->leaves_second_elipsis_center.get_y() + rx2_u * sA), device_width, device_height, universe_width, universe_height);
    tmp2 = Utils::universe_to_canvas(Point(this->leaves_second_elipsis_center.get_x() - ry2_u * sA, this->leaves_second_elipsis_center.get_y() + ry2_u * cA), device_width, device_height, universe_width, universe_height);
    rx_px = (int)lround(hypot(tmp.get_x() - c_leaves2.get_x(), tmp.get_y() - c_leaves2.get_y()));
    ry_px = (int)lround(hypot(tmp2.get_x() - c_leaves2.get_x(), tmp2.get_y() - c_leaves2.get_y()));
    ang = atan2(tmp.get_y() - c_leaves2.get_y(), tmp.get_x() - c_leaves2.get_x());
    Primitives::draw_rotated_ellipse(surface, c_leaves2.get_x(), c_leaves2.get_y(), rx_px, ry_px, ang, this->leaves_color, true);

    // Elipse 3 (direita)
    // [CORREÇÃO] Usar o ponto original do universo (this->...) para o cálculo.
    tmp = Utils::universe_to_canvas(Point(this->leaves_third_elipsis_center.get_x() + rx2_u * cA, this->leaves_third_elipsis_center.get_y() + rx2_u * sA), device_width, device_height, universe_width, universe_height);
    tmp2 = Utils::universe_to_canvas(Point(this->leaves_third_elipsis_center.get_x() - ry2_u * sA, this->leaves_third_elipsis_center.get_y() + ry2_u * cA), device_width, device_height, universe_width, universe_height);
    rx_px = (int)lround(hypot(tmp.get_x() - c_leaves3.get_x(), tmp.get_y() - c_leaves3.get_y()));
    ry_px = (int)lround(hypot(tmp2.get_x() - c_leaves3.get_x(), tmp2.get_y() - c_leaves3.get_y()));
    ang = atan2(tmp.get_y() - c_leaves3.get_y(), tmp.get_x() - c_leaves3.get_x());
    Primitives::draw_rotated_ellipse(surface, c_leaves3.get_x(), c_leaves3.get_y(), rx_px, ry_px, ang, this->leaves_color, true);

    /*
    int r_apple_px = lround(0.06 * this->width * scale_x);
    Point c_apple1 = Utils::universe_to_canvas(this->apple_center, device_width, device_height, universe_width, universe_height);
    Point c_apple2 = Utils::universe_to_canvas(this->apple2_center, device_width, device_height, universe_width, universe_height);

    Primitives::draw_circle(surface, c_apple1.get_x(), c_apple1.get_y(), r_apple_px, this->apple_color, true, true);
    Primitives::draw_circle(surface, c_apple2.get_x(), c_apple2.get_y(), r_apple_px, this->apple_color, true, true);
    */
}

