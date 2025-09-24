#include "Tree.h"
#include "Utils.h"
#include "Primitives.h"
#include <SDL.h>
#include <math.h>

Tree::Tree(int width, int height, int universe_x_origin, int universe_y_origin, Uint32 color_trunk, Uint32 color_leaves, Uint32 color_apple) {
    this->width       = width;
    this->height      = height;
    this->x_origin    = universe_x_origin;
    this->y_origin    = universe_y_origin;
    this->trunk_color = color_trunk;
    this->leaves_color  = color_leaves;
    this->apple_color  = color_apple;

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
    //Não precisa para elipses e círculos, só pinta como filled
    /*
    this->apple_fill = Point(0, 0);
    this->leaves1_fill = Point(0, 0);
    this->leaves2_fill = Point(0, 0);
    this->leaves3_fill = Point(0, 0);*/
}

void Tree::rotate_figure(double angle)
{
    double x, y, radians, cosTheta, sinTheta, dx, dy;

    dx = (double)this->trunk_bottom_left.get_x();
    dy = (double)this->trunk_bottom_left.get_y();

    translate(-dx, -dy);

    radians  = Utils::to_radians(angle);

    this->rotated_angle  += radians;
    this->rotated_angle  = fmod(this->rotated_angle , 6.28318530717958647692); //2 * pi
    if (this->rotated_angle < 0.0) this->rotated_angle  += 6.28318530717958647692; //2 * pi

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

void Tree::translate(double dx, double dy){
    double x, y;

    for(Point* p : pts) {
        x = (double)p->get_x();
        y = (double)p->get_y();
        p->set_x(x + dx);
        p->set_y(y + dy);
    }
}

void Tree::scale(double sx, double sy){
    double dx, dy, x, y, radians, cosTheta, sinTheta;

    // ancora no canto inferior esquerdo (mesmo do rotate)
    dx = (double)this->trunk_bottom_left.get_x();
    dy = (double)this->trunk_bottom_left.get_y();
    this->translate(-dx, -dy);

    // usa a rotacao atual da arvore como referencial local
    radians  = this->rotated_angle;
    cosTheta = cos(radians);
    sinTheta = sin(radians);

    for (Point* p : pts) {
        // leva o ponto para o referencial LOCAL (desrotaciona por -radians)
        x = (double)p->get_x();
        y = (double)p->get_y();

        double lx =  x *  cosTheta + y * sinTheta; // R(-theta)
        double ly = -x *  sinTheta + y * cosTheta;

        // escala nos eixos locais
        lx = lx * sx;
        ly = ly * sy;

        // volta para o mundo (rotaciona por +radians)
        double wx = lx * cosTheta - ly * sinTheta; // R(+theta)
        double wy = lx * sinTheta + ly * cosTheta;

        p->set_x(wx);
        p->set_y(wy);
    }

    this->translate(dx, dy);

    // Atualiza dimensoes logicas (usadas p/ raios das folhas/macas na draw)
    this->width  = (int)lround(this->width  * sx);
    this->height = (int)lround(this->height * sy);
}

void Tree::draw(SDL_Surface* surface) {
    if (!surface) return;

    const int device_width  = surface->w;
    const int device_height = surface->h;

    const int universe_width  = App::universe_width;
    const int universe_height = App::universe_height;

    // --- conversões para canvas (mesmos nomes das members) ---
    Point trunk_bottom_left        = Utils::universe_to_canvas(this->trunk_bottom_left,        device_width, device_height, universe_width, universe_height);
    Point trunk_bottom_right       = Utils::universe_to_canvas(this->trunk_bottom_right,       device_width, device_height, universe_width, universe_height);
    Point trunk_top_right          = Utils::universe_to_canvas(this->trunk_top_right,          device_width, device_height, universe_width, universe_height);
    Point trunk_top_left           = Utils::universe_to_canvas(this->trunk_top_left,           device_width, device_height, universe_width, universe_height);
    Point trunk_left_bezier_point  = Utils::universe_to_canvas(this->trunk_left_bezier_point,  device_width, device_height, universe_width, universe_height);
    Point trunk_right_bezier_point = Utils::universe_to_canvas(this->trunk_right_bezier_point, device_width, device_height, universe_width, universe_height);

    Point leaves_first_elipsis_center  = Utils::universe_to_canvas(this->leaves_first_elipsis_center,  device_width, device_height, universe_width, universe_height);
    Point leaves_second_elipsis_center = Utils::universe_to_canvas(this->leaves_second_elipsis_center, device_width, device_height, universe_width, universe_height);
    Point leaves_third_elipsis_center  = Utils::universe_to_canvas(this->leaves_third_elipsis_center,  device_width, device_height, universe_width, universe_height);

    Point apple_center = Utils::universe_to_canvas(this->apple_center, device_width, device_height, universe_width, universe_height);
    Point apple2_center = Utils::universe_to_canvas(this->apple2_center, device_width, device_height, universe_width, universe_height);

    // --- TRONCO: base/topo + laterais com Bézier cúbica (p1=p2 = ponto de controle) ---
    Primitives::draw_line(surface, trunk_bottom_left.get_x(),  trunk_bottom_left.get_y(),  trunk_bottom_right.get_x(), trunk_bottom_right.get_y(), this->trunk_color, false);
    Primitives::draw_line(surface, trunk_top_left.get_x(),     trunk_top_left.get_y(),     trunk_top_right.get_x(),    trunk_top_right.get_y(),   this->trunk_color, false);

    //printf("PASSOU 4.1\n");

    Primitives::draw_curve(surface,
        (int)trunk_bottom_left.get_x(),  (int)trunk_bottom_left.get_y(),
        (int)trunk_right_bezier_point.get_x(), (int)trunk_right_bezier_point.get_y(),
        (int)trunk_right_bezier_point.get_x(), (int)trunk_right_bezier_point.get_y(),
        (int)trunk_top_left.get_x(),     (int)trunk_top_left.get_y(),
        this->trunk_color, false);

    //printf("PASSOU 4.2\n");

    Primitives::draw_curve(surface,
        (int)trunk_bottom_right.get_x(), (int)trunk_bottom_right.get_y(),
        (int)trunk_left_bezier_point.get_x(), (int)trunk_left_bezier_point.get_y(),
        (int)trunk_left_bezier_point.get_x(), (int)trunk_left_bezier_point.get_y(),
        (int)trunk_top_right.get_x(),    (int)trunk_top_right.get_y(),
        this->trunk_color, false);

    //printf("PASSOU 4.3\n");

    // preenchimento do tronco
    Point  trunk_fill     = Utils::universe_to_canvas(Point(this->trunk_fill.get_x(), this->trunk_fill.get_y()), device_width, device_height, universe_width, universe_height);
    Primitives::flood_fill(surface, (int)trunk_fill.get_x(), (int)trunk_fill.get_y(), this->trunk_color);
    //Primitives::set_pixel(surface, (int)trunk_fill.get_x(), (int)trunk_fill.get_y(), this->trunk_color);

    //printf("PASSOU 4.4\n");

    // --- FOLHAS: 3 elipses preenchidas ---
    // raios em unidades do “universo”
    double rx1_u = 0.38 * this->width,  ry1_u = 0.22 * this->height; // raio da elipse central
    double rx2_u = 0.235 * this->width,  ry2_u = 0.18 * this->height; // raio dsa elipses laterais
    double cA = cos(this->rotated_angle);
    double sA = sin(this->rotated_angle);

    int rx_px, ry_px;
    double ang;

    // elipse 1 (central): constrói eixos locais rotacionados no UNIVERSO e mede no CANVAS
    Point tmp  = Utils::universe_to_canvas(Point(this->leaves_first_elipsis_center.get_x() + rx1_u * cA,
                                           this->leaves_first_elipsis_center.get_y() + rx1_u * sA),
                                     device_width, device_height, universe_width, universe_height);
    Point tmp2 = Utils::universe_to_canvas(Point(this->leaves_first_elipsis_center.get_x() - ry1_u * sA,
                                           this->leaves_first_elipsis_center.get_y() + ry1_u * cA),
                                     device_width, device_height, universe_width, universe_height);
    rx_px = (int)lround(sqrt((tmp.get_x()  - leaves_first_elipsis_center.get_x())  * (tmp.get_x()  - leaves_first_elipsis_center.get_x()) +
                             (tmp.get_y()  - leaves_first_elipsis_center.get_y())  * (tmp.get_y()  - leaves_first_elipsis_center.get_y())));
    ry_px = (int)lround(sqrt((tmp2.get_x() - leaves_first_elipsis_center.get_x()) * (tmp2.get_x() - leaves_first_elipsis_center.get_x()) +
                             (tmp2.get_y() - leaves_first_elipsis_center.get_y()) * (tmp2.get_y() - leaves_first_elipsis_center.get_y())));
    ang = atan2(tmp.get_y() - leaves_first_elipsis_center.get_y(),
                tmp.get_x() - leaves_first_elipsis_center.get_x());

    //printf("PASSOU 4.5\n");

    Primitives::draw_rotated_ellipse(surface, (int)leaves_first_elipsis_center.get_x(), (int)leaves_first_elipsis_center.get_y(),
                                     rx_px, ry_px, ang, this->leaves_color, true);

    //printf("PASSOU 4.6\n");

    // elipse 2 (esquerda)
    tmp  = Utils::universe_to_canvas(Point(this->leaves_second_elipsis_center.get_x() + rx2_u * cA,
                                           this->leaves_second_elipsis_center.get_y() + rx2_u * sA),
                                     device_width, device_height, universe_width, universe_height);
    tmp2 = Utils::universe_to_canvas(Point(this->leaves_second_elipsis_center.get_x() - ry2_u * sA,
                                           this->leaves_second_elipsis_center.get_y() + ry2_u * cA),
                                     device_width, device_height, universe_width, universe_height);
    rx_px = (int)lround(sqrt((tmp.get_x()  - leaves_second_elipsis_center.get_x())  * (tmp.get_x()  - leaves_second_elipsis_center.get_x()) +
                             (tmp.get_y()  - leaves_second_elipsis_center.get_y())  * (tmp.get_y()  - leaves_second_elipsis_center.get_y())));
    ry_px = (int)lround(sqrt((tmp2.get_x() - leaves_second_elipsis_center.get_x()) * (tmp2.get_x() - leaves_second_elipsis_center.get_x()) +
                             (tmp2.get_y() - leaves_second_elipsis_center.get_y()) * (tmp2.get_y() - leaves_second_elipsis_center.get_y())));
    ang = atan2(tmp.get_y() - leaves_second_elipsis_center.get_y(),
                tmp.get_x() - leaves_second_elipsis_center.get_x());
    Primitives::draw_rotated_ellipse(surface, (int)leaves_second_elipsis_center.get_x(), (int)leaves_second_elipsis_center.get_y(),
                                     rx_px, ry_px, ang, this->leaves_color, true);
    //printf("PASSOU 4.7\n");

    // elipse 3 (direita)
    tmp  = Utils::universe_to_canvas(Point(this->leaves_third_elipsis_center.get_x() + rx2_u * cA,
                                           this->leaves_third_elipsis_center.get_y() + rx2_u * sA),
                                     device_width, device_height, universe_width, universe_height);
    tmp2 = Utils::universe_to_canvas(Point(this->leaves_third_elipsis_center.get_x() - ry2_u * sA,
                                           this->leaves_third_elipsis_center.get_y() + ry2_u * cA),
                                     device_width, device_height, universe_width, universe_height);
    rx_px = (int)lround(sqrt((tmp.get_x()  - leaves_third_elipsis_center.get_x())  * (tmp.get_x()  - leaves_third_elipsis_center.get_x()) +
                             (tmp.get_y()  - leaves_third_elipsis_center.get_y())  * (tmp.get_y()  - leaves_third_elipsis_center.get_y())));
    ry_px = (int)lround(sqrt((tmp2.get_x() - leaves_third_elipsis_center.get_x()) * (tmp2.get_x() - leaves_third_elipsis_center.get_x()) +
                             (tmp2.get_y() - leaves_third_elipsis_center.get_y()) * (tmp2.get_y() - leaves_third_elipsis_center.get_y())));
    ang = atan2(tmp.get_y() - leaves_third_elipsis_center.get_y(),
                tmp.get_x() - leaves_third_elipsis_center.get_x());
    Primitives::draw_rotated_ellipse(surface, (int)leaves_third_elipsis_center.get_x(), (int)leaves_third_elipsis_center.get_y(),
                                     rx_px, ry_px, ang, this->leaves_color, true);

    //printf("PASSOU 4.8\n");

    // --- MAÇÃ: círculo preenchido ---
    /*double ru = 0.06 * (this->width < this->height ? this->width : this->height);
    tmp = Utils::universe_to_canvas(Point(this->apple_center.get_x() + ru, this->apple_center.get_y()), device_width, device_height, universe_width, universe_height);
    int r_px = abs((int)tmp.get_x() - (int)apple_center.get_x());
    Primitives::draw_circle(surface, (int)apple_center.get_x(), (int)apple_center.get_y(), r_px, this->apple_color, true, true);
    printf("PASSOU 4.9\n");*/

    /*tmp = Utils::universe_to_canvas(Point(this->apple2_center.get_x() + ru, this->apple2_center.get_y()), device_width, device_height, universe_width, universe_height);
    r_px = abs((int)tmp.get_x() - (int)apple2_center.get_x());
    Primitives::draw_circle(surface, (int)apple2_center.get_x(), (int)apple2_center.get_y(), r_px, this->apple_color, true, true);
    printf("PASSOU 4.10\n");*/
}
