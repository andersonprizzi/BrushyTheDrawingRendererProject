#ifndef APP_H
#define APP_H

#define SDL_MAIN_HANDLED

// Forward declaration.
class App;
class ButtonComponent;
class TextboxComponent;
class NotificationManager;

// C++ standard library.
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <list>
#include <vector>
#include <memory>

// C standard library.
#include <cstdio>
#include <cstdarg>
#include <strings.h>

// External libraries.
#include <SDL.h>
#include <SDL_ttf.h>

class House;
class Tree;
class Shape;
class Sun;
// Header files.
#include "Colors.h"
#include "Utils.h"
#include "ErrorHandler.h"
#include "FontManager.h"
#include "Notification.h"
#include "NotificationManager.h"
#include "Point.h"
#include "Primitives.h"
#include "ButtonComponent.h"
#include "TextboxComponent.h"
#include "ImageComponent.h"
#include "AppBarComponent.h"
#include "Shape.h"
#include "Circle.h"
#include "Polygon.h"
#include "House.h"
#include "Tree.h"
#include "Fence.h"
#include "Sun.h"

enum class AppState {
    MENU_SCREEN,
    NEW_PROJECT_SCREEN,
    RENDERING_SCREEN
};

enum class MouseState {
    NORMAL_MODE,
    ERASER_MODE,
    LINE_MODE,
    PENCIL_MODE,
    BUCKET_MODE,
    HOUSE_MODE,
    TREE_MODE,
    FENCE_MODE,
    SUN_MODE
};

class App {
    private:
        // Application attributes.
        SDL_Window* window;
        SDL_Surface* surface;
        SDL_Surface* drawing_surface;
        bool running;
        int screen_width;
        int screen_height;
        int window_width;
        int window_height;

        std::string window_title;
        AppState app_state;
        MouseState mouse_state = MouseState::NORMAL_MODE;
        NotificationManager *notification_manager;

        // Graphical interface attributes.
        static float new_drawing_button_relative_x_percent;
        static float new_drawing_button_relative_y_percent;
        static float load_file_button_relative_x_percent;
        static int default_button_height;
        ImageComponent* app_icon_image;
        ButtonComponent* new_drawing_button;
        ButtonComponent* load_project_button;
        ButtonComponent* create_project_button;
        ButtonComponent* back_menu_button;
        TextboxComponent* width_textbox;
        TextboxComponent* height_textbox;
        SDL_Surface* text_title_surface;
        SDL_Surface* text_enter_height_surface;
        SDL_Surface* text_enter_width_surface;
        SDL_Rect text_rect;

        //======================================
        //PARADAS PARA GUI DO PAINT 2
        std::list<Point> points;
        std::list<Point> eraser_points;
        std::list<Point> fill_points;
        std::list<std::array<Point,2>> lines;

        std::vector<std::unique_ptr<Shape> > shapes;

        //PARA DRAG AND DROP
        bool mouse_down = false; //Informa se o mouse foi baixado dentro da área de desenho (se desenho foi iniciado, basicamente)
        bool temporary_in_list = false;
        Point temporary_dragging_point = Point(0,0);
        Point initial_point = Point(0,0);
        //BOTÕES
        ButtonComponent* eraser_button;
        ButtonComponent* pencil_button;
        ButtonComponent* bucket_button;
        ButtonComponent* line_button;
        ButtonComponent* house_button;
        ButtonComponent* tree_button;
        ButtonComponent* fence_button;
        ButtonComponent* sun_button;

        Uint32 primary_colors[4]; //Pensei pra colocar tuas cores e tal, mas daí tu se vira pra implementar kkk boa sorte
        //======================================

    public:
        App(const std::string& title, float width_percent, float height_percent);
        static int universe_width;
        static int universe_height;
        void run();
        void close(int exit_code = 1);
        void handle_events();
        void clear_screen(Uint32 color);
        void clear_screen(Uint8 r, Uint8 g, Uint8 b);
        void update_screen();
        int get_window_width();
        int get_window_height();
        /*static int get_universe_width();
        static int get_universe_height();*/
        int get_screen_width();
        int get_screen_height();
        SDL_Surface* get_surface();
};

#endif
