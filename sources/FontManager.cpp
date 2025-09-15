#include "FontManager.h"

/////////////////////////////////////////////////////////////////////////////////////////
// ATTRIBUTE INITIALIZATION                                                            //
/////////////////////////////////////////////////////////////////////////////////////////

TTF_Font* FontManager::libertinus_regular_36 = nullptr;
TTF_Font* FontManager::roboto_regular_26 = nullptr;
TTF_Font* FontManager::roboto_semibold_26 = nullptr;
TTF_Font* FontManager::roboto_semibold_20 = nullptr;



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

bool FontManager::load_fonts() {
    FontManager::libertinus_regular_36 = TTF_OpenFont("assets/fonts/LibertinusKeyboard-Regular.ttf", 36);
    if (!libertinus_regular_36) {
        ErrorHandler::log_error("There was a problem loading a font: %s", TTF_GetError());
        return false;
    }

    FontManager::roboto_regular_26 = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 26);
    if (!roboto_regular_26) {
        fprintf(stderr, "Erro ao carregar fonte: %s\n", TTF_GetError());
        return false;
    }

    FontManager::roboto_semibold_26 = TTF_OpenFont("assets/fonts/Roboto-SemiBold.ttf", 26);
    if (!roboto_semibold_26) {
        fprintf(stderr, "Erro ao carregar fonte: %s\n", TTF_GetError());
        return false;
    }

    FontManager::roboto_semibold_20 = TTF_OpenFont("assets/fonts/Roboto-SemiBold.ttf", 20);
    if (!roboto_semibold_20) {
        fprintf(stderr, "Erro ao carregar fonte: %s\n", TTF_GetError());
        return false;
    }

    return true;
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

void FontManager::close_fonts() {
    if (libertinus_regular_36) {
        TTF_CloseFont(libertinus_regular_36);
        libertinus_regular_36 = nullptr;
    }

    if (roboto_regular_26) {
        TTF_CloseFont(roboto_regular_26);
        roboto_regular_26 = nullptr;
    }

    if (roboto_semibold_26) {
        TTF_CloseFont(roboto_semibold_26);
        roboto_semibold_26 = nullptr;
    }

    if (roboto_semibold_20) {
        TTF_CloseFont(roboto_semibold_20);
        roboto_semibold_20 = nullptr;
    }
}
