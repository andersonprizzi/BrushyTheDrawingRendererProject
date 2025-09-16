#ifndef APP_H
#define APP_H

#define SDL_MAIN_HANDLED

// C++ standard library.
#include <string>
#include <iostream>
#include <cmath>
#include <stack>
#include <unordered_map>
//#include <filesystem>

// C standard library.
#include <cstdio>
#include <cstdarg>
#include <strings.h>

// External libraries.
#include <SDL.h>
#include <SDL_ttf.h>

// Header files.
#include "ErrorHandler.h"
#include "FontManager.h"
#include "Colors.h"
#include "Utils.h"
#include "Point.h"
#include "Primitives.h"
#include "ButtonComponent.h"
#include "TextboxComponent.h"
#include "ImageComponent.h"
#include "AppBarComponent.h"
#include "Shape.h"
#include "Circle.h"

class ButtonComponent;
class TextboxComponent;

enum class AppState {
    MENU_SCREEN,
    NEW_PROJECT_SCREEN,
    RENDERING_SCREEN
};

class App {
    private:
        // Application attributes.
        SDL_Window* window;
        SDL_Surface* surface;
        bool running;
        int screen_width;
        int screen_height;
        int window_width;
        int window_height;
        std::string window_title;
        AppState app_state;

        // Graphical interface attributes.
        static float new_drawing_button_relative_x_percent;
        static float new_drawing_button_relative_y_percent;
        static float load_file_button_relative_x_percent;
        static int default_button_height;
        ImageComponent* myImageComponent;
        ButtonComponent* new_drawing_button;
        ButtonComponent* load_project_button;
        ButtonComponent* create_project_button;
        TextboxComponent* width_textbox;
        TextboxComponent* height_textbox;
        SDL_Surface* textSurface;
        SDL_Surface* text_enter_height;
        SDL_Surface* text_enter_width;
        SDL_Rect textRect;

    public:
        App(const std::string& title, float width_percent, float height_percent);
        ~App();
        void run();
        void exit_app();
        void handle_events();
        void clear_screen(Uint8 r, Uint8 g, Uint8 b);
        void update_screen();
        int get_window_width();
        int get_window_height();
        int get_screen_width();
        int get_screen_height();
        SDL_Surface* get_surface();
};

#endif
