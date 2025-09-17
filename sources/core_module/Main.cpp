// INCLUDES
#include "App.h"


// MAIN FUNCTION
int main(int argc, char* argv[]) {
    App *app = new App("Brushy: The Drawing Render", 0.7, 0.7);
    app->run();
    return 0;


    /*
    // Inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erro SDL: %s", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0) {
        SDL_Log("Erro TTF: %s", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // Cria janela
    int win_w = 800;
    int win_h = 600;
    SDL_Window* window = SDL_CreateWindow("Teste NotificationManager",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          win_w, win_h,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Erro ao criar janela: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Obtém surface da janela
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    NotificationManager nm(win_w, win_h);

    Notification n;
    n.title = "Avisão";
    n.message = "Message!";
    n.rect = { win_w/2 - 150, win_h - 100, 300, 80 };
    n.close_button = { n.rect.w - 28, 8, 20, 20 };
    n.duration_ms = 6000;

    FontManager::load_fonts();
    nm.set_fonts(FontManager::roboto_semibold_20, FontManager::roboto_semibold_20);

    nm.push(n);
    SDL_Event e;

    // No loop principal
    while (1) {
        while (SDL_PollEvent(&e)) {
            nm.handle_event(&e);
        }
        nm.update();
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 255, 255));
        nm.draw(screenSurface);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
    */
}

