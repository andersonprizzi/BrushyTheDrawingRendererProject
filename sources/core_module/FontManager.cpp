/////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////

#include "FontManager.h"



/////////////////////////////////////////////////////////////////////////////////////////
// ATTRIBUTE INITIALIZATION                                                            //
/////////////////////////////////////////////////////////////////////////////////////////

TTF_Font* FontManager::libertinus_regular_36 = nullptr;
TTF_Font* FontManager::roboto_regular_26 = nullptr;
TTF_Font* FontManager::roboto_semibold_26 = nullptr;
TTF_Font* FontManager::roboto_semibold_20 = nullptr;



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD IMPLEMENTATIONS                                                              //
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 *      This function attempts to load multiple fonts with specific sizes
 *      from the assets/fonts directory. If any font fails to load, an
 *      error is logged and the function returns false.
 *
 * @return true
 *      If all fonts are loaded successfully.
 * @return false
 *      If any font fails to load.
 */
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



/**
 * @brief
 *      This function ensures that each loaded font is properly closed
 *      and sets the corresponding font pointers to nullptr to prevent
 *      dangling references.
 */
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
