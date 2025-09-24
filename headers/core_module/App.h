#ifndef APP_H
#define APP_H

#define SDL_MAIN_HANDLED

// Forward declaration.
class App;
class ButtonComponent;
class TextboxComponent;
class NotificationManager;
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
#include "Circle.h"
#include "Polygon.h"
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
        NotificationManager *notification_manager;

        // General graphical interface attributes and methods.
        static float new_drawing_button_relative_x_percent;
        static float new_drawing_button_relative_y_percent;
        static float load_file_button_relative_x_percent;
        static int default_button_height;
        static int secondary_button_width;
        static int app_bar_height;
        static int default_margin;

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

        // Graphical interface components attributes: MENU_SCREEN.
        ImageComponent* app_icon_image;
        ButtonComponent* new_drawing_button;
        ButtonComponent* load_project_button;
        SDL_Surface* text_title_surface;
        SDL_Surface* text_enter_height_surface;
        SDL_Surface* text_enter_width_surface;
        SDL_Rect text_rect;

        // Graphical interface components attributes: NEW_PROJECT_SCREEN.
        ButtonComponent* create_project_button;
        ButtonComponent* back_menu_button;
        TextboxComponent* width_textbox;
        TextboxComponent* height_textbox;
        AppBar* app_bar_project_screen;

        // Drawing component list attributes.
        std::list<Point> points;
        std::list<Point> eraser_points;
        std::list<Point> fill_points;
        std::list<std::array<Point,2>> lines;
        std::vector<std::unique_ptr<Shape> > shapes;

        // Drag and drop functionality attributes.
        bool mouse_down = false;
        bool temporary_in_list = false;
        Point temporary_dragging_point = Point(0,0);
        Point initial_point = Point(0,0);

        // Graphical interface components attributes: RENDERING_SCREEN.
        ButtonComponent* eraser_button;
        ButtonComponent* pencil_button;
        ButtonComponent* bucket_button;
        ButtonComponent* line_button;
        ButtonComponent* house_button;
        ButtonComponent* tree_button;
        ButtonComponent* fence_button;
        ButtonComponent* sun_button;
        AppBar* app_bar_rendering_screen;

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
        int get_screen_width();
        int get_screen_height();
        SDL_Surface* get_surface();
};

#endif
