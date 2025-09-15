#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "App.h"

class ErrorHandler {
    public:
        static bool loaded_fonts;
        static void log_error(const char* message, ...);
        static void fatal_error(const char* message, ...);
        static void fatal_error(int exit_code, const char* message, ...);
};

#endif
