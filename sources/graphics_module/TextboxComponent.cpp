// INCLUDES
#include "TextboxComponent.h"


// CONSTRUCTOR IMPLEMENTATION
TextboxComponent::TextboxComponent(int x, int y, int w, int h, TTF_Font* font, bool onlyNumbers) {
    rect = {x, y, w, h};
    this->font = font;
    this->onlyNumbers = onlyNumbers;
    bgColor = {220, 220, 220, 255};      // cinza claro
    borderColor = {150, 150, 150, 255};  // cinza mais escuro
    textColor = {0, 0, 0, 255};          // preto
    text.clear();
    active = false;
}


// METHOD IMPLEMENTATION
void TextboxComponent::handleEvent(SDL_Event* e) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        int mx = e->button.x;
        int my = e->button.y;
        // ativa se clicou dentro; desativa se clicou fora
        active = (mx >= rect.x && mx <= rect.x + rect.w &&
                  my >= rect.y && my <= rect.y + rect.h);
    }

    // se não estiver ativa, ignorar eventos de texto/teclado
    if (!active) return;

    if (e->type == SDL_TEXTINPUT) {
        const char* s = e->text.text;
        // s é C-string terminada em '\0'. Iterar até o terminador.
        // Valida se onlyNumbers == true
        bool valid = true;
        for (size_t i = 0; s[i] != '\0'; ++i) {
            unsigned char ch = static_cast<unsigned char>(s[i]);
            if (onlyNumbers && !std::isdigit(ch)) {
                valid = false;
                break;
            }
        }
        if (valid) {
            text.append(s);
        }
    } else if (e->type == SDL_KEYDOWN) {
        // backspace
        if (e->key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
            // para UTF-8 seria necessário tratar multi-byte; aqui presumimos ASCII simples
            text.pop_back();
        }
    }
}


// METHOD IMPLEMENTATION
void TextboxComponent::render(SDL_Surface* targetSurface) {
    if (!targetSurface) return;

    const int border_thickness = 2;
    const int outer_radius = 10;

    // Mapeia cores para o formato da surface
    Uint32 border32 = SDL_MapRGB(targetSurface->format, borderColor.r, borderColor.g, borderColor.b);
    Uint32 bg32     = SDL_MapRGB(targetSurface->format, bgColor.r, bgColor.g, bgColor.b);

    // 1) Preencher retângulo externo com a cor da borda (forma base da borda)
    SDL_Rect outer = rect;
    SDL_FillRect(targetSurface, &outer, border32);

    // 2) Desenhar círculos externos (bordas arredondadas)
    //Primitives::draw_circle(targetSurface, rect.x + outer_radius, rect.y + outer_radius, outer_radius, border32, true, true); // sup. esquerdo
    //Primitives::draw_circle(targetSurface, rect.x + rect.w - outer_radius, rect.y + outer_radius, outer_radius, border32, true, true); // sup. direito
    //Primitives::draw_circle(targetSurface, rect.x + outer_radius, rect.y + rect.h - outer_radius, outer_radius, border32, true, true); // inf. esquerdo
    //Primitives::draw_circle(targetSurface, rect.x + rect.w - outer_radius, rect.y + rect.h - outer_radius, outer_radius, border32, true, true); // inf. direito

    // 3) Preencher interior (fundo) com bgColor (deslocado pela espessura da borda)
    SDL_Rect inner;
    inner.x = rect.x + border_thickness;
    inner.y = rect.y + border_thickness;
    inner.w = rect.w - 2 * border_thickness;
    inner.h = rect.h - 2 * border_thickness;

    if (inner.w > 0 && inner.h > 0) {
        SDL_FillRect(targetSurface, &inner, bg32);

        // desenhar círculos internos (para manter cantos arredondados do fundo)
        int inner_radius = outer_radius - border_thickness;
        if (inner_radius > 0) {
            //Primitives::draw_circle(targetSurface, inner.x + inner_radius, inner.y + inner_radius, inner_radius, bg32, true, true);
            //Primitives::draw_circle(targetSurface, inner.x + inner.w - inner_radius, inner.y + inner_radius, inner_radius, bg32, true, true);
            //Primitives::draw_circle(targetSurface, inner.x + inner_radius, inner.y + inner.h - inner_radius, inner_radius, bg32, true, true);
            //Primitives::draw_circle(targetSurface, inner.x + inner.w - inner_radius, inner.y + inner.h - inner_radius, inner_radius, bg32, true, true);
        }
    }

    // 4) Renderizar texto (centralizado verticalmente)
    if (!text.empty() && font) {
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
        if (textSurface) {
            SDL_Rect dst;
            dst.x = rect.x + 8; // padding esquerdo
            dst.y = rect.y + (rect.h - textSurface->h) / 2;
            dst.w = textSurface->w;
            dst.h = textSurface->h;
            SDL_BlitSurface(textSurface, nullptr, targetSurface, &dst);
            SDL_FreeSurface(textSurface);
        }
    }

    // 5) Desenhar caret (cursor) quando ativa
    if (active && font) {
        // medir largura do texto para posicionar o caret
        int tw = 0, th = 0;
        if (text.empty()) {
            // medir um caráter de referência (altura)
            TTF_SizeText(font, "M", &tw, &th);
            tw = 0;
        } else {
            TTF_SizeText(font, text.c_str(), &tw, &th);
        }
        int caret_x = rect.x + 8 + tw;
        int caret_y = rect.y + (rect.h - th) / 2;
        SDL_Rect caret = { caret_x, caret_y, 2, th };
        Uint32 caretColor32 = SDL_MapRGB(targetSurface->format, 0, 0, 0);
        SDL_FillRect(targetSurface, &caret, caretColor32);
    }
}


// METHOD IMPLEMENTATION
bool TextboxComponent::get_active() {
    return this->active;
}
