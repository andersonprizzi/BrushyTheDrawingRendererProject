#include "NotificationManager.h"


NotificationManager::NotificationManager(int w, int h)
    : window_width_(w), window_height_(h), has_current_(false) {}

NotificationManager::~NotificationManager() {
    // nada para liberar, pois usamos valores (não ponteiros dinâmicos)
}

void NotificationManager::push(const Notification& n) {
    queue_.push(n);
}

void NotificationManager::update() {
    if (!has_current_ && !queue_.empty()) {
        current_ = queue_.front();
        queue_.pop();

        current_.start_time = SDL_GetTicks();
        current_.appearing = true;
        current_.disappearing = false;
        current_.alpha = 0.0f;
        current_.pos_y = (float)(window_height_ + current_.rect.h);

        has_current_ = true;
    }

    update_current();
}

void NotificationManager::update_current() {
    if (!has_current_) return;

    Notification& n = current_;
    Uint32 now = SDL_GetTicks();

    const float slide_speed = 5.0f;
    const float fade_step = 15.0f;

    // Slide + fade in
    if (n.appearing) {
        n.pos_y -= slide_speed;
        n.alpha += fade_step;
        if (n.pos_y <= n.rect.y) {
            n.pos_y = (float)n.rect.y;
            n.alpha = 255.0f;
            n.appearing = false;
        }
    }

    // Fechamento automático
    if (!n.appearing && !n.disappearing && (now - n.start_time > n.duration_ms)) {
        n.disappearing = true;
    }

    // Slide + fade out
    if (n.disappearing) {
        n.pos_y += slide_speed;
        n.alpha -= fade_step;
        if (n.alpha <= 0.0f) {
            has_current_ = false; // liberamos o slot
            return;
        }
    }

    // clamp
    if (n.alpha > 255.0f) n.alpha = 255.0f;
    if (n.alpha < 0.0f) n.alpha = 0.0f;
}



void NotificationManager::handle_event(SDL_Event* e) {
    if (!has_current_ || !e) return;

    Notification& n = current_;
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        int x = e->button.x;
        int y = e->button.y;

        SDL_Rect btn = n.close_button;
        btn.x = n.rect.x + n.close_button.x;
        btn.y = (int)(n.pos_y + n.close_button.y);

        if (x >= btn.x && x <= btn.x + btn.w &&
            y >= btn.y && y <= btn.y + btn.h) {
            n.disappearing = true;
        }
    }
}


void NotificationManager::draw_rounded_rect_surface(SDL_Surface* surface, int x, int y, int w, int h, SDL_Color color, int radius, bool filled) {
    if (!surface) return;

    // Fator de supersampling
    const int scale = 3;
    int sw = w * scale;
    int sh = h * scale;
    int sradius = radius * scale;

    // Cria surface temporária grande
    SDL_Surface* tmp_large = SDL_CreateRGBSurface(0, sw, sh, 32,
                                                  0x00FF0000,
                                                  0x0000FF00,
                                                  0x000000FF,
                                                  0xFF000000);
    if (!tmp_large) return;

    SDL_SetSurfaceBlendMode(tmp_large, SDL_BLENDMODE_BLEND);
    SDL_FillRect(tmp_large, NULL, SDL_MapRGBA(tmp_large->format, 0, 0, 0, 0));

    Uint32 px_color = SDL_MapRGBA(tmp_large->format, color.r, color.g, color.b, color.a);

    // Desenha retângulo com cantos arredondados na surface grande
    for (int py = 0; py < sh; py++) {
        for (int px = 0; px < sw; px++) {
            // Distância mínima para os cantos
            int dx = (px < sradius) ? sradius - px : (px >= sw - sradius ? px - (sw - sradius - 1) : 0);
            int dy = (py < sradius) ? sradius - py : (py >= sh - sradius ? py - (sh - sradius - 1) : 0);
            float dist_sq = float(dx*dx + dy*dy);

            if (dist_sq <= float(sradius*sradius)) {
                Primitives::set_pixel(tmp_large, px, py, px_color);
            } else if (filled && (dx == 0 || dy == 0)) {
                // partes centrais não arredondadas
                Primitives::set_pixel(tmp_large, px, py, px_color);
            }
        }
    }

    // Cria surface final com tamanho real
    SDL_Surface* tmp_final = SDL_CreateRGBSurface(0, w, h, 32,
                                                  0x00FF0000,
                                                  0x0000FF00,
                                                  0x000000FF,
                                                  0xFF000000);
    if (!tmp_final) {
        SDL_FreeSurface(tmp_large);
        return;
    }
    SDL_SetSurfaceBlendMode(tmp_final, SDL_BLENDMODE_BLEND);
    SDL_FillRect(tmp_final, NULL, SDL_MapRGBA(tmp_final->format, 0, 0, 0, 0));

    // Downscale: média de scale x scale pixels
    for (int py = 0; py < h; py++) {
        for (int px = 0; px < w; px++) {
            int r_sum = 0, g_sum = 0, b_sum = 0, a_sum = 0;
            for (int sy = 0; sy < scale; sy++) {
                for (int sx = 0; sx < scale; sx++) {
                    Uint32 col = Primitives::get_pixel(tmp_large, px*scale + sx, py*scale + sy);
                    Uint8 r,g,b,a;
                    SDL_GetRGBA(col, tmp_large->format, &r, &g, &b, &a);
                    r_sum += r; g_sum += g; b_sum += b; a_sum += a;
                }
            }
            Uint8 r = Uint8(r_sum / (scale*scale));
            Uint8 g = Uint8(g_sum / (scale*scale));
            Uint8 b = Uint8(b_sum / (scale*scale));
            Uint8 a = Uint8(a_sum / (scale*scale));
            Uint32 final_col = SDL_MapRGBA(tmp_final->format, r, g, b, a);
            Primitives::set_pixel(tmp_final, px, py, final_col);
        }
    }

    // Blit final para a surface de destino
    SDL_Rect dest = { x, y, w, h };
    SDL_BlitSurface(tmp_final, NULL, surface, &dest);

    SDL_FreeSurface(tmp_large);
    SDL_FreeSurface(tmp_final);
}









// Desenha retângulo com alpha em SDL_Surface
void draw_rect_with_alpha(SDL_Surface* target, int x, int y, int w, int h, SDL_Color color) {
    if (!target) return;

    SDL_Surface* tmp = SDL_CreateRGBSurface(0, w, h, 32,
                                            target->format->Rmask,
                                            target->format->Gmask,
                                            target->format->Bmask,
                                            target->format->Amask);
    if (!tmp) return;

    SDL_FillRect(tmp, NULL, SDL_MapRGBA(tmp->format, color.r, color.g, color.b, color.a));
    SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_BLEND);

    SDL_Rect dest = { x, y, w, h };
    SDL_BlitSurface(tmp, NULL, target, &dest);

    SDL_FreeSurface(tmp);
}


/*
void NotificationManager::draw_notification(SDL_Surface* target, Notification* n) {
    if (!n || !target) return;

    Uint8 a = (Uint8)n->alpha;
    SDL_Color bg = { 50, 50, 50, a };
    draw_rounded_rect_surface(target, n->rect.x, (int)n->pos_y, n->rect.w, n->rect.h, bg, 10, true);

    SDL_Rect btn = n->close_button;
    btn.x = n->rect.x + n->close_button.x;
    btn.y = (int)(n->pos_y + n->close_button.y);
    SDL_Color red = { 200, 50, 50, a };
    draw_rounded_rect_surface(target, btn.x, btn.y, btn.w, btn.h, red, 5, true);

    // Texto
    // renderText(target, n->title, ..., n->rect.x + 10, (int)n->pos_y + 5);
    // renderText(target, n->message, ..., n->rect.x + 10, (int)n->pos_y + 30);
}
*/



void NotificationManager::draw_notification(SDL_Surface* target, const Notification& n) {
    if (!target) return;

    Uint8 a = static_cast<Uint8>(n.alpha);
    SDL_Color bg = {50, 50, 50, a};

    // Fundo arredondado da notificação
    draw_rounded_rect_surface(target,
                              n.rect.x,
                              static_cast<int>(n.pos_y),
                              n.rect.w,
                              n.rect.h,
                              bg,
                              10,
                              true);

    // Botão de fechar (calculado relativo ao rect)
    SDL_Rect btn = n.close_button;
    btn.x = n.rect.x + n.close_button.x;
    btn.y = static_cast<int>(n.pos_y) + n.close_button.y;

    SDL_Color red = {200, 50, 50, a};
    draw_rounded_rect_surface(target, btn.x, btn.y, btn.w, btn.h, red, 8, true);

    SDL_Color text_color = {255, 255, 255, a};

    if (font_title_) {
        Primitives::draw_text(target, font_title_, n.title, n.rect.x + 10, static_cast<int>(n.pos_y) + 8, text_color);
    }

    if (font_message_) {
        Primitives::draw_text(target, font_message_, n.message, n.rect.x + 10, static_cast<int>(n.pos_y) + 30, text_color);
    }
}



void NotificationManager::draw(SDL_Surface* target) {
    //if (!current_) return;
    draw_notification(target, current_);
}


void NotificationManager::set_fonts(TTF_Font* title, TTF_Font* message) {
    font_title_ = title;
    font_message_ = message;
}
