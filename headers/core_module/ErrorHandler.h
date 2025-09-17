#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "App.h"

class ErrorHandler {
    private:
        static App *app;
        static bool loaded_fonts;

    public:
        static void initialize(App *app);
        static bool get_loaded_fonts();
        static void set_loaded_fonts(bool value);
        static void log_error(const char* message, ...);
        static void fatal_error(const char* message, ...);
        static void fatal_error(int exit_code, const char* message, ...);
};

#endif
