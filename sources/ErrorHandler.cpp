/////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////

#include "ErrorHandler.h"

/////////////////////////////////////////////////////////////////////////////////////////
// ATTRIBUTE INITIALIZATION                                                            //
/////////////////////////////////////////////////////////////////////////////////////////

bool  ErrorHandler::loaded_fonts = false;



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 *      This method logs a non-fatal error message to stderr.
 *
 * @param message
 *      The error message format string.
 * @param ...
 *      Optional arguments for the format string.
 */
void ErrorHandler::log_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 *      This method logs a fatal error message, cleans up SDL/TTF, and exits the program
 *      with error code 1.
 *
 * @param message
 *      The error message format string.
 * @param ...
 *      Optional arguments for the format string.
 */
void ErrorHandler::fatal_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    fprintf(stderr, "[FATAL] ");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);

    if (loaded_fonts) {
        FontManager::close_fonts();
    }

    SDL_Quit();
    TTF_Quit();
    exit(1);
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 *      This method logs a fatal error message, cleans up SDL/TTF, and exits the program.
 *
 * @param exit_code
 *      The exit code used to terminate the program.
 * @param message
 *      The error message format string.
 * @param ...
 *      Optional arguments for the format string.
 */
void ErrorHandler::fatal_error(int exit_code, const char* message, ...) {
    va_list args;
    va_start(args, message);
    fprintf(stderr, "[FATAL] ");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);

    // TODO: Call exit_app() instead

    if (loaded_fonts) {
        FontManager::close_fonts();
    }

    SDL_Quit();
    TTF_Quit();
    exit(exit_code);
}
