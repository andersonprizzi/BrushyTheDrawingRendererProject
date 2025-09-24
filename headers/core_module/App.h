#ifndef APP_H
#define APP_H

#define SDL_MAIN_HANDLED

// Forward declaration.
class App;
class ButtonComponent;
class TextboxComponent;
class NotificationManager;
class Colors;
class House;
class Tree;
class Shape;
class Sun;

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
#include "House.h"
#include "Tree.h"
#include "Fence.h"
#include "Sun.h"
#include "FileManager.h"

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

enum class ColorState {
    PRIMARY,
    SECONDARY,
    TERTIARY
};

class App {
    private:
        // Application attributes.
        SDL_Window* window;
        SDL_Surface* window_surface;
        SDL_Surface* drawing_surface;
        bool running;
        int screen_width;
        int screen_height;
        int window_width;
        int window_height;
        std::string window_title;
        MouseState mouse_state = MouseState::NORMAL_MODE;
        NotificationManager *notification_manager = nullptr;

        // General graphical interface attributes and methods.
        static float new_drawing_button_relative_x_percent;
        static float new_drawing_button_relative_y_percent;
        static float load_file_button_relative_x_percent;
        static int primary_button_height;
        static int primary_button_width;
        static int app_bar_height;
        static int default_margin;
        static int main_image_size;
        static int bottom_image_margin;

        // Screen state attributes and methods.
        AppState app_state;
        void change_screen_state(AppState new_state);
        void load_menu_screen();
        void unload_menu_screen();
        void load_new_project_screen();
        void unload_new_project_screen();
        void load_rendering_screen();
        void unload_rendering_screen();

        // Screen rendering attributes and methods.
        void render_menu_screen();
        void render_new_project_screen();
        void render_rendering_screen();
        bool recreate_drawing_surface(int new_width, int new_height);

        // Drawing component list attributes.
        std::list<Point> points;
        std::list<Point> eraser_points;
        std::list<Point> fill_points;
        std::list<std::array<Point,2>> lines;
        std::vector<std::unique_ptr<Shape>> shapes;

        // Drag and drop functionality attributes.
        bool mouse_down = false;
        bool temporary_in_list = false;
        Point temporary_dragging_point = Point(0,0);
        Point initial_point = Point(0,0);

        // Graphical interface components attributes: MENU_SCREEN.
        ImageComponent* app_icon_image = nullptr;
        ButtonComponent* new_drawing_button = nullptr;
        ButtonComponent* load_project_button = nullptr;
        SDL_Surface* text_title_surface = nullptr;
        SDL_Surface* text_enter_height_surface = nullptr;
        SDL_Surface* text_enter_width_surface = nullptr;
        SDL_Rect text_rect;

        // Graphical interface components attributes: NEW_PROJECT_SCREEN.
        ButtonComponent* create_project_button = nullptr;
        ButtonComponent* back_menu_button = nullptr;
        TextboxComponent* width_textbox = nullptr;
        TextboxComponent* height_textbox = nullptr;
        AppBar* app_bar_project_screen = nullptr;

        // Graphical interface components attributes: RENDERING_SCREEN.
        ButtonComponent* eraser_button = nullptr;
        ButtonComponent* pencil_button = nullptr;
        ButtonComponent* bucket_button = nullptr;
        ButtonComponent* line_button = nullptr;
        ButtonComponent* house_button = nullptr;
        ButtonComponent* tree_button = nullptr;
        ButtonComponent* fence_button = nullptr;
        ButtonComponent* sun_button = nullptr;
        AppBar* app_bar_rendering_screen = nullptr;
        Uint32 background_drawing_color;

        // Colors

    public:
        Uint32 primary_color, second_color, tertiary_color;
        App(const std::string& title, float width_percent, float height_percent);
        static int universe_width;
        static int universe_height;
        void run();
        void close(int exit_code = 1);
        void handle_events();
        void update_screen();
        int get_window_width();
        int get_window_height();
        int get_screen_width();
        int get_screen_height();
        SDL_Surface* get_surface();
        ColorState changing_color = PRIMARY;
};

#endif
