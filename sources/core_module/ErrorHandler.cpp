// INCLUDES
#include "ErrorHandler.h"


// STATIC ATTRIBUTES INITIALIZATION
App* ErrorHandler::app = nullptr;
bool ErrorHandler::loaded_fonts = false;


// METHOD IMPLEMENTATION
/**
 * @brief
 * This method initializes the ErrorHandler with a pointer to the App instance.
 *
 * @param app
 * Pointer to the application instance used by the ErrorHandler.
 */
void ErrorHandler::initialize(App *app) {
    ErrorHandler::app = app;
}


// METHOD IMPLEMENTATION
/**
 * @brief
 * This method returns whether the fonts have been successfully loaded.
 *
 * @return true
 * If the fonts are loaded.
 *
 * @return flase
 * If the fonts are not loaded.
 */
bool ErrorHandler::get_loaded_fonts() {
    return loaded_fonts;
}


// METHOD IMPLEMENTATION
/**
 * @brief
 * Sets the loaded_fonts status to the given value.
 *
 * @param value
 * Boolean indicating whether the fonts are loaded (true) or not (false).
 */
void ErrorHandler::set_loaded_fonts(bool value) {
    ErrorHandler::loaded_fonts = value;
}


// METHOD IMPLEMENTATION
/**
 * @brief
 * This method logs a non-fatal error message to stderr.
 *
 * @param message
 * The error message format string.
 *
 * @param ...
 * Optional arguments for the format string.
 */
void ErrorHandler::log_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
}


// METHOD IMPLEMENTATION
/**
 * @brief
 * This method logs a fatal error message, cleans up SDL/TTF, and exits the program
 * with error code 1.
 *
 * @param message
 * The error message format string.
 *
 * @param ...
 * Optional arguments for the format string.
 */
void ErrorHandler::fatal_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    fprintf(stderr, "[FATAL] ");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
    ErrorHandler::app->close(1);
}


// METHOD IMPLEMENTATION
/**
 * @brief
 * This method logs a fatal error message, cleans up SDL/TTF, and exits the program.
 *
 * @param exit_code
 * The exit code used to terminate the program.
 *
 * @param message
 * The error message format string.
 *
 * @param ...
 * Optional arguments for the format string.
 */
void ErrorHandler::fatal_error(int exit_code, const char* message, ...) {
    va_list args;
    va_start(args, message);
    fprintf(stderr, "[FATAL] ");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
    ErrorHandler::app->close(exit_code);
}
