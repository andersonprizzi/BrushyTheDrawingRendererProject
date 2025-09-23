#include "AppBarComponent.h"

AppBar::AppBar(int width, int height, const std::string& title, TTF_Font* font)
    : rect{0, 0, width, height},
      bgColor{30, 30, 30, 255},     // fundo padrão: cinza escuro
      textColor{255, 255, 255, 255},// texto padrão: branco
      title(title),
      marginLeft(20),
      font(font) {}

void AppBar::set_background_color(SDL_Color color) {
    bgColor = color;
}

void AppBar::setTextColor(SDL_Color color) {
    textColor = color;
}

void AppBar::setMarginLeft(int margin) {
    marginLeft = margin;
}

void AppBar::set_title(const std::string& newTitle) {
    title = newTitle;
}

void AppBar::set_size(int width, int height) {
    rect.w = width;
    rect.h = height;
}

void AppBar::draw(SDL_Surface* targetSurface) {
    if (!targetSurface) return;

    // Criar uma cor de preenchimento (Uint32 compatível com o formato da surface)
    Uint32 fillColor = SDL_MapRGB(targetSurface->format, bgColor.r, bgColor.g, bgColor.b);

    // Preencher fundo da barra
    SDL_FillRect(targetSurface, &rect, fillColor);

    if (!font || title.empty()) return;

    // Criar surface do texto
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, title.c_str(), textColor);
    if (!textSurface) {
        SDL_Log("Erro ao criar surface do texto: %s", TTF_GetError());
        return;
    }

    int shadowHeight = 7; // altura da sombra

    for (int i = 0; i < shadowHeight; i++) {
        // Alpha diminui conforme a linha fica mais distante
        Uint8 alpha = 10 - (i * 10 / shadowHeight); // 180 → 0

        // Criar uma linha preta com alpha
        SDL_Surface* line = SDL_CreateRGBSurface(0, rect.w, 1, 32,
                                                 0x00FF0000,
                                                 0x0000FF00,
                                                 0x000000FF,
                                                 0xFF000000);
        if (!line) continue;

        SDL_FillRect(line, nullptr, SDL_MapRGBA(line->format, 0, 0, 0, alpha));

        // Habilitar blending
        SDL_SetSurfaceBlendMode(line, SDL_BLENDMODE_BLEND);

        // Definir posição da linha
        SDL_Rect lineRect = {rect.x, rect.y + rect.h + i, rect.w, 1};

        // Blit na surface alvo
        SDL_BlitSurface(line, nullptr, targetSurface, &lineRect);

        SDL_FreeSurface(line);
    }

    // Criar posição do texto (alinhado à esquerda com margem, centralizado verticalmente)
    SDL_Rect textRect;
    textRect.x = marginLeft;
    textRect.y = rect.y + (rect.h - textSurface->h) / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    // Copiar texto para a superfície principal
    SDL_BlitSurface(textSurface, nullptr, targetSurface, &textRect);

    SDL_FreeSurface(textSurface);
}
