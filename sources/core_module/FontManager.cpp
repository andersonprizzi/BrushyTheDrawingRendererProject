// INCLUDES
#include "FontManager.h"


// ATTRIBUTE INITIALIZATION
TTF_Font* FontManager::libertinus_regular_36 = nullptr;
TTF_Font* FontManager::roboto_regular_26 = nullptr;
TTF_Font* FontManager::roboto_regular_15 = nullptr;
TTF_Font* FontManager::roboto_semibold_26 = nullptr;
TTF_Font* FontManager::roboto_semibold_20 = nullptr;
TTF_Font* FontManager::roboto_semibold_16 = nullptr;
TTF_Font* FontManager::default_notification_message_font = nullptr;
TTF_Font* FontManager::default_notification_title_font = nullptr;
TTF_Font* FontManager::default_primary_button_font = nullptr;


// METHOD IMPLEMENTATION
/**
 * @brief
 * This function attempts to load multiple fonts with specific sizes
 * from the assets/fonts directory. If any font fails to load, an
 * error is logged and the function returns false.
 *
 * @return true
 * If all fonts are loaded successfully.
 *
 * @return false
 * If any font fails to load.
 */
bool FontManager::load_fonts() {
    FontManager::libertinus_regular_36 = TTF_OpenFont("assets/fonts/LibertinusKeyboard-Regular.ttf", 36);
    if (!libertinus_regular_36) {
        ErrorHandler::log_error("There was a problem loading a font: %s", TTF_GetError());
        return false;
    }

    FontManager::roboto_regular_26 = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 26);
    if (!roboto_regular_26) {
        ErrorHandler::log_error("There was a problem loading a font: %s", TTF_GetError());
        return false;
    }

    FontManager::roboto_regular_15 = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 15);
    if (!roboto_regular_15) {
        ErrorHandler::log_error("There was a problem loading a font: %s", TTF_GetError());
        return false;
    }

    FontManager::roboto_semibold_26 = TTF_OpenFont("assets/fonts/Roboto-SemiBold.ttf", 26);
    if (!roboto_semibold_26) {
        ErrorHandler::log_error("There was a problem loading a font: %s", TTF_GetError());
        return false;
    }

    FontManager::roboto_semibold_20 = TTF_OpenFont("assets/fonts/Roboto-SemiBold.ttf", 20);
    if (!roboto_semibold_20) {
        ErrorHandler::log_error("There was a problem loading a font: %s", TTF_GetError());
        return false;
    }

    FontManager::roboto_semibold_16 = TTF_OpenFont("assets/fonts/Roboto-SemiBold.ttf", 16);
    if (!roboto_semibold_16) {
        ErrorHandler::log_error("There was a problem loading a font: %s", TTF_GetError());
        return false;
    }

    FontManager::default_notification_message_font = roboto_regular_15;
    FontManager::default_notification_title_font = roboto_semibold_16;
    FontManager::default_primary_button_font = roboto_semibold_20;

    return true;
}


// METHOD IMPLEMENTATION
/**
 * @brief
 * This function ensures that each loaded font is properly closed
 * and sets the corresponding font pointers to nullptr to prevent
 * dangling references.
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

    if (roboto_regular_15) {
        TTF_CloseFont(roboto_regular_15);
        roboto_regular_15 = nullptr;
    }

    if (roboto_semibold_26) {
        TTF_CloseFont(roboto_semibold_26);
        roboto_semibold_26 = nullptr;
    }

    if (roboto_semibold_20) {
        TTF_CloseFont(roboto_semibold_20);
        roboto_semibold_20 = nullptr;
    }

    if (roboto_semibold_16) {
        TTF_CloseFont(roboto_semibold_16);
        roboto_semibold_16 = nullptr;
    }
}
