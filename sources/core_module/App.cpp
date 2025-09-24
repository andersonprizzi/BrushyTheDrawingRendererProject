#include "App.h"


// STATIC ATTRIBUTES INITIALIZATION
float App::new_drawing_button_relative_x_percent = 0.50f;       // Posição horizontal relativa
float App::new_drawing_button_relative_y_percent = 0.73f;       // Posição vertical relativa // Para mover os TODOS os elementos da tela inicial para cima e para baixo, basta modificar este atributo.
float App::load_file_button_relative_x_percent = 0.50f;         // Posição horizontal relativa
int App::primary_button_height = 45;
int App::primary_button_width = 300;
int App::app_bar_height = 50;
int App::default_margin = 30;
int App::main_image_size = 256;
int App::bottom_image_margin = 15;
int App::universe_width = 40;
int App::universe_height = 30;


// CONSTRUCTOR IMPLEMENTATION
App::App(const std::string& title, float width_percent, float height_percent) {
    this->window = nullptr;
    this->window_surface = nullptr;
    this->drawing_surface = nullptr;
    this->running = false;
    this->window_width = 0;
    this->window_height = 0;
    this->window_title = title;
    this->app_state = AppState::MENU_SCREEN;
    this->notification_manager = nullptr;


    // Initialization of external libraries.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error initializing SDL library: %s.\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Error initializing SDL_ttf library: %s.\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    // Loading fonts.
    if (FontManager::load_fonts() == false) {
        ErrorHandler::fatal_error("Failed to load fonts.");
    } else {
        ErrorHandler::set_loaded_fonts(true);
    }

    // Getting the resolution of the primary monitor.
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    this->screen_width = display_mode.w;
    this->screen_height = display_mode.h;

    // Calculating window width and height.
    this->window_width = static_cast<int>(this->screen_width * width_percent);
    this->window_height = static_cast<int>(this->screen_height * height_percent);

    this->window = SDL_CreateWindow(
        this->window_title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        this->window_width,
        this->window_height,
        SDL_WINDOW_RESIZABLE
    );

    if (!(this->window)) {
        ErrorHandler::fatal_error("Failed to create window: %s.", SDL_GetError());
    }

    this->window_surface = SDL_GetWindowSurface(window);
    this->drawing_surface = SDL_CreateRGBSurface(0, window_width, window_height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000);

    // Initializing notification manager.
    this->notification_manager = new NotificationManager(this->window_width, this->window_height);
    this->notification_manager->set_fonts(FontManager::default_notification_title_font, FontManager::default_notification_message_font);

    // Initializing app icon.
    SDL_Surface* icon = SDL_LoadBMP("assets/Icon_64.bmp");

    if (!icon) {
        ErrorHandler::fatal_error("Failed to load application icon: %s.", SDL_GetError());
    } else {
        SDL_SetWindowIcon(this->window, icon);
        SDL_FreeSurface(icon);
    }

    this->background_drawing_color = Colors::rgb_to_uint32(this->drawing_surface, 255, 255, 255);
    this->primary_color = Colors::rgb_to_uint32(this->drawing_surface, 247, 30, 113);
    this->second_color = Colors::rgb_to_uint32(this->drawing_surface, 30, 113, 247);
    this->tertiary_color = Colors::rgb_to_uint32(this->drawing_surface, 113, 247, 30);
}


// METHOD IMPLEMENTATION
void App::run() {
    this->running = true;
    this->load_menu_screen();

    // Renderiza o texto para uma surface.
    SDL_Color textColor = {0, 0, 0};
    text_enter_height_surface = TTF_RenderText_Blended(FontManager::roboto_semibold_20, "Enter height:", textColor);
    text_enter_width_surface = TTF_RenderText_Blended(FontManager::roboto_semibold_20, "Enter width:", textColor);

    // Execution loop.
    while (running) {
        if (this->app_state == AppState::MENU_SCREEN) {
            this->render_menu_screen();

        } else if (this->app_state == AppState::NEW_PROJECT_SCREEN) {
            this->render_new_project_screen();

        } else if (this->app_state == AppState::RENDERING_SCREEN) {
            this->render_rendering_screen();
        }

        this->notification_manager->update();
        this->notification_manager->draw(this->window_surface);
        this->handle_events();
        this->update_screen();
    }

    this->close();
}


// METHOD IMPLEMENTATION
void App::close(int exit_code) {
    if (text_title_surface) SDL_FreeSurface(text_title_surface);

    SDL_DestroyWindow(this->window);

    if (ErrorHandler::get_loaded_fonts() == true) {
        FontManager::close_fonts();
    }

    TTF_Quit();
    SDL_Quit();
    exit(exit_code);
}


bool inside_rect(int mx, int my, SDL_Rect dst_rect){
    if (mx >= dst_rect.x && mx < dst_rect.x + dst_rect.w && my >= dst_rect.y && my < dst_rect.y + dst_rect.h) return true;
    return false;
}


// METHOD IMPLEMENTATION
void App::handle_events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        // Processes the program exit event.
        if (event.type == SDL_QUIT) {
            running = false;
        }

        // Processes window resizing.
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            window_surface = SDL_GetWindowSurface(window);
            this->window_width = window_surface->w;
            this->window_height = window_surface->h;

            if (this->new_drawing_button) {
                this->new_drawing_button->set_position(
                    static_cast<int>(window_width * this->new_drawing_button_relative_x_percent - new_drawing_button->w / 2),
                    static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - new_drawing_button->h / 2)
                );
            }

            if (this->load_project_button) {
                this->load_project_button->set_position(
                    static_cast<int>(window_width * this->load_file_button_relative_x_percent - load_project_button->w / 2),
                    static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->primary_button_height / 2) + this->primary_button_height + 15
                );
            }

            if (this->app_bar_project_screen) {
                this->app_bar_project_screen->set_size(this->window_width, App::app_bar_height);
            }

            if (this->app_bar_rendering_screen) {
                this->app_bar_rendering_screen->set_size(this->window_width, App::app_bar_height);
            }

            if (this->app_state == AppState::MENU_SCREEN) {
                text_rect.w = text_title_surface->w;
                text_rect.h = text_title_surface->h;
                text_rect.x = (window_width - text_rect.w) / 2;
                text_rect.y = this->new_drawing_button->y - text_rect.h - 30;
                this->app_icon_image->set_x((window_width - App::main_image_size) / 2);
                this->app_icon_image->set_y(this->text_rect.y - App::main_image_size - App::bottom_image_margin);
            }
        }

        // Processes mouse click events.
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mx = event.button.x;
            int my = event.button.y;

            // For veryfying click in the drawing surface
            SDL_Rect dst_rect;

            if (drawing_surface) {
                dst_rect.w = drawing_surface->w;
                dst_rect.h = drawing_surface->h;
                dst_rect.x = (window_width - dst_rect.w - 2) / 2;
                dst_rect.y = (window_height - dst_rect.h + App::app_bar_height) / 2;
            }

            if (event.button.button == SDL_BUTTON_LEFT && inside_rect(mx, my, dst_rect)) mouse_down = true;

            // Screen change: MENU_SCREEN > NEW_PROJECT_SCREEN
            if (this->app_state == AppState::MENU_SCREEN && new_drawing_button->is_clicked(mx, my)) {
                this->change_screen_state(AppState::NEW_PROJECT_SCREEN);

            // Se o usuário clicar em "Open project file"
            } else if (this->app_state == AppState::MENU_SCREEN && load_project_button->is_clicked(mx, my)) {
                int loaded_width = 0, loaded_height = 0;
                int universe_w = 0, universe_h = 0;
                Uint32 background_color = 0;

                if (FileManager::load_scene("ExemploCorrigido.csv", this->window_surface, this->shapes,
                                            &loaded_width, &loaded_height,
                                            &universe_w, &universe_h,
                                            &background_color))
                {
                    if (universe_w > 0 && universe_h > 0) {
                        App::universe_width = universe_w;
                        App::universe_height = universe_h;
                    }

                    if (loaded_width > 0 && loaded_height > 0) {
                        this->recreate_drawing_surface(loaded_width, loaded_height);
                        SDL_SetWindowSize(this->window, loaded_width + 2 * App::default_margin, loaded_height + 2 * App::default_margin + App::app_bar_height);
                        SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    }

                    this->background_drawing_color = background_color;
                    this->change_screen_state(AppState::RENDERING_SCREEN);
                } else {
                    this->notification_manager->push({
                        "Error!",
                        "Could not load project file.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                }

            // Screen change: NEW_PROJECT_SCREEN > RENDERING_SCREEN
            } else if (this->app_state == AppState::NEW_PROJECT_SCREEN && create_project_button->is_clicked(mx, my)) {
                if (width_textbox->get_text().empty()) {
                    this->notification_manager->push({
                        "Warning!",
                        "The width field is required.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                    continue;
                }

                if (height_textbox->get_text().empty()) {
                    this->notification_manager->push({
                        "Warning!",
                        "The height field is required.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                    continue;
                }

                if (!Utils::check_window_size(this, std::stoi(width_textbox->get_text()), std::stoi(height_textbox->get_text()))) {
                    this->notification_manager->push({
                        "Warning!",
                        "The specified width or height is below the minimum allowed or above your monitor's resolution.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                    continue;
                }

                this->recreate_drawing_surface(std::stoi(width_textbox->get_text()), std::stoi(height_textbox->get_text()));
                SDL_SetWindowSize(this->window, std::stoi(width_textbox->get_text()) + 2 * App::default_margin, std::stoi(height_textbox->get_text()) + 2 * App::default_margin + App::app_bar_height);
                SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                this->change_screen_state(AppState::RENDERING_SCREEN);


            // Screen change: NEW_PROJECT_SCREEN > MENU_SCREEN
            } else if (this->app_state == AppState::NEW_PROJECT_SCREEN && back_menu_button->is_clicked(mx, my)) {
                this->change_screen_state(AppState::MENU_SCREEN);


            } else if (this->app_state == AppState::RENDERING_SCREEN && this->save_button->is_clicked(mx, my)) {
                if (SDL_SaveBMP(this->drawing_surface, "screenshot.bmp") == 0) {
                    this->notification_manager->push({
                        "Screenshot saved!",
                        "Screenshot saved at the root directory.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                } else {
                    printf("Erro ao salvar BMP: %s\n", SDL_GetError());
                }
            } else if (this->app_state == AppState::RENDERING_SCREEN && this->pencil_button->is_clicked(mx, my)) {
                this->mouse_state = MouseState::PENCIL_MODE;
                this->notification_manager->push({
                        "Pencil mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
            } else if (this->app_state == AppState::RENDERING_SCREEN && this->eraser_button->is_clicked(mx, my)) {
                this->mouse_state = MouseState::ERASER_MODE;
                this->notification_manager->push({
                        "Eraser mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });

            } else if (this->app_state == AppState::RENDERING_SCREEN && this->bucket_button->is_clicked(mx, my)) {
                this->mouse_state = MouseState::BUCKET_MODE;
                this->notification_manager->push({
                        "Bucket mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });

            } else if (this->app_state == AppState::RENDERING_SCREEN && this->line_button->is_clicked(mx, my)) {
                this->mouse_state = MouseState::LINE_MODE;
                this->notification_manager->push({
                        "Line mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });

            } else if (this->app_state == AppState::RENDERING_SCREEN && this->house_button->is_clicked(mx, my)) {
                this->mouse_state = MouseState::HOUSE_MODE;
                this->notification_manager->push({
                        "House mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                this->mouse_down = false;
                this->temporary_in_list = false;

            }else if (this->app_state == AppState::RENDERING_SCREEN && this->tree_button->is_clicked(mx, my)) {
                this->mouse_state = MouseState::TREE_MODE;
                this->notification_manager->push({
                        "Tree mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                this->mouse_down = false;
                this->temporary_in_list = false;

            }else if (this->app_state == AppState::RENDERING_SCREEN && this->fence_button->is_clicked(mx, my)) {
                this->mouse_state = MouseState::FENCE_MODE;
                this->notification_manager->push({
                        "Fence mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                this->mouse_down = false;
                this->temporary_in_list = false;

            }else if (this->app_state == AppState::RENDERING_SCREEN && this->sun_button->is_clicked(mx, my)) {
                this->mouse_state = MouseState::SUN_MODE;
                this->notification_manager->push({
                        "Sun mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                this->mouse_down = false;
                this->temporary_in_list = false;

            }else if (this->app_state == AppState::RENDERING_SCREEN && this->primary_selector->is_clicked(mx, my)) {
                this->mouse_state = MouseState::COLOR_MODE;
                this->notification_manager->push({
                        "Primary color mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                this->changing_color = ColorState::PRIMARY;
            }else if (this->app_state == AppState::RENDERING_SCREEN && this->secondary_selector->is_clicked(mx, my)) {
                this->mouse_state = MouseState::COLOR_MODE;
                this->notification_manager->push({
                        "Secondary color mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                this->changing_color = ColorState::SECONDARY;
            }else if (this->app_state == AppState::RENDERING_SCREEN && this->tertiary_selector->is_clicked(mx, my)) {
                this->mouse_state = MouseState::COLOR_MODE;
                this->notification_manager->push({
                        "Tertiary color mode",
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                this->changing_color = ColorState::TERTIARY;
            }else if (this->app_state == AppState::RENDERING_SCREEN && this->mouse_state == MouseState::COLOR_MODE && this->red_button->is_clicked(mx, my)) {
                std::string temp = "";
                if (this->changing_color == ColorState::PRIMARY){
                    temp = "Primary color is set red";
                    this->primary_color = red_button->color;
                }
                else if (this->changing_color == ColorState::SECONDARY){
                    temp = "Seconday color is set red";
                    this->second_color = red_button->color;
                }
                else if (this->changing_color == ColorState::TERTIARY){
                    temp = "Tertiary color is set red";
                    this->tertiary_color = red_button->color;
                }

                this->notification_manager->push({
                        temp,
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
            }else if (this->app_state == AppState::RENDERING_SCREEN && this->mouse_state == MouseState::COLOR_MODE && this->blue_button->is_clicked(mx, my)) {
                std::string temp = "";
                if (this->changing_color == ColorState::PRIMARY){
                    temp = "Primary color is set blue";
                    this->primary_color = blue_button->color;
                }
                else if (this->changing_color == ColorState::SECONDARY){
                    temp = "Seconday color is set blue";
                    this->second_color = blue_button->color;
                }
                else if (this->changing_color == ColorState::TERTIARY){
                    temp = "Tertiary color is set blue";
                    this->tertiary_color = blue_button->color;
                }

                this->notification_manager->push({
                        temp,
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
            }else if (this->app_state == AppState::RENDERING_SCREEN && this->mouse_state == MouseState::COLOR_MODE && this->green_button->is_clicked(mx, my)) {
                std::string temp = "";
                if (this->changing_color == ColorState::PRIMARY){
                    temp = "Primary color is set green";
                    this->primary_color = green_button->color;
                }
                else if (this->changing_color == ColorState::SECONDARY){
                    temp = "Seconday color is set green";
                    this->second_color = green_button->color;
                }
                else if (this->changing_color == ColorState::TERTIARY){
                    temp = "Tertiary color is set green";
                    this->tertiary_color = green_button->color;
                }

                this->notification_manager->push({
                        temp,
                        "Right-click to return to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
            } else if (this->app_state == AppState::RENDERING_SCREEN && inside_rect(mx, my, dst_rect) && event.button.button != SDL_BUTTON_RIGHT){
                //DRAWING THINGS
                if (this->mouse_state == MouseState::PENCIL_MODE){
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    this->points.emplace_back(Point(cx, cy,this->primary_color));
                } if (this->mouse_state == MouseState::ERASER_MODE){
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    this->eraser_points.emplace_back(cx, cy);
                } else if (this->mouse_state == MouseState::BUCKET_MODE) {
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    this->fill_points.emplace_back(Point(cx, cy, this->primary_color));
                }else if (this->mouse_state == MouseState::LINE_MODE) {
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    this->initial_point = Point(cx, cy);
                    this->lines.emplace_back(std::array<Point,2>{{ Point(cx,cy), Point(cx,cy) }});
                    this->temporary_in_list = true;
                } else if (this->mouse_state == MouseState::HOUSE_MODE || this->mouse_state == MouseState::TREE_MODE || this->mouse_state == MouseState::FENCE_MODE || this->mouse_state == MouseState::SUN_MODE) {
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    this->initial_point = Point(cx, cy);
                    this->temporary_in_list = false;
                }
            }
        }

        if (event.type == SDL_MOUSEMOTION && mouse_down){
            int mx = event.motion.x;
            int my = event.motion.y;

            // For veryfying click in the drawing surface
            SDL_Rect dst_rect;

            if (drawing_surface) {
                dst_rect.w = drawing_surface->w;
                dst_rect.h = drawing_surface->h;
                dst_rect.x = (window_width - dst_rect.w - 2) / 2;
                dst_rect.y = (window_height - dst_rect.h + App::app_bar_height) / 2;
            }

            if (this->app_state == AppState::RENDERING_SCREEN && inside_rect(mx, my, dst_rect)){
                //DRAWING THINGS
                if (this->mouse_state == MouseState::PENCIL_MODE){
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    points.emplace_back(cx, cy);
                }else if (this->mouse_state == MouseState::ERASER_MODE){
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    eraser_points.emplace_back(cx, cy);
                }else if (this->mouse_state == MouseState::LINE_MODE){
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas
                    if (!this->lines.empty()) lines.pop_back();
                    this->temporary_in_list = true;
                    this->lines.emplace_back(std::array<Point,2>{{ initial_point, Point(cx,cy,this->primary_color) }});
                } else if (this->mouse_state == MouseState::HOUSE_MODE || this->mouse_state == MouseState::TREE_MODE || this->mouse_state == MouseState::FENCE_MODE || this->mouse_state == MouseState::SUN_MODE){
                    // mouse -> canvas
                    int cx1 = mx - dst_rect.x;
                    int cy1 = my - dst_rect.y;

                    this->temporary_dragging_point = Point(cx1, cy1);
                    if (this->temporary_in_list){
                        if (!this->shapes.empty()) shapes.pop_back();

                        this->temporary_in_list = false;
                    }

                    Point c0 = Point(this->initial_point.get_x(), this->initial_point.get_y());
                    Point c1 = Point(this->temporary_dragging_point.get_x(), this->temporary_dragging_point.get_y());
                    Utils::UniverseRect ur = Utils::canvas_drag_to_universe(c0, c1, drawing_surface->w, drawing_surface->h, App::universe_width, App::universe_height);

                    // Construtor do House é (width, height, x_origin, y_origin, cores...)
                    Uint32 c = SDL_MapRGB(window_surface->format, 255, 100, 50);

                    if (this->mouse_state == MouseState::HOUSE_MODE){
                        shapes.emplace_back(std::unique_ptr<Shape>(new House( int(std::lround(ur.w)),
                         int(std::lround(ur.h)),
                         int(std::lround(ur.x)),
                         int(std::lround(ur.y)),
                         this->primary_color, this->second_color, this->tertiary_color)));
                    } else if (this->mouse_state == MouseState::TREE_MODE){
                        shapes.emplace_back(std::unique_ptr<Shape>(new Tree (int(std::lround(ur.w)),
                         int(std::lround(ur.h)),
                         int(std::lround(ur.x)),
                         int(std::lround(ur.y)),
                         this->primary_color, this->second_color, this->tertiary_color)));
                    } else if (this->mouse_state == MouseState::FENCE_MODE){
                        shapes.emplace_back(std::unique_ptr<Shape>(new Fence (int(std::lround(ur.w)),
                         int(std::lround(ur.h)),
                         int(std::lround(ur.x)),
                         int(std::lround(ur.y)),
                         this->primary_color, this->second_color)));
                    } else if (this->mouse_state == MouseState::SUN_MODE){
                        shapes.emplace_back(std::unique_ptr<Shape>(new Sun (int(std::lround(ur.w)),
                         int(std::lround(ur.h)),
                         int(std::lround(ur.x)),
                         int(std::lround(ur.y)),
                         this->primary_color, this->second_color)));
                    }
                    // limpa estado do drag
                    this->temporary_in_list = true;
                }
            }
        }

        if (event.type == SDL_MOUSEBUTTONUP){
            //printf("event.type == SDL_MOUSEBUTTONUP\n");
            if (event.button.button == SDL_BUTTON_RIGHT && mouse_down == false) {
                this->mouse_state = MouseState::NORMAL_MODE;
                this->notification_manager->push({
                        "Normal mode",
                        "Returned to normal.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
            }

            if (event.button.button == SDL_BUTTON_LEFT) mouse_down = false;
        }

        this->notification_manager->handle_event(&event);

        if (width_textbox) {
            width_textbox->handleEvent(&event);
        }

        if (height_textbox) {
            height_textbox->handleEvent(&event);
        }
    }
}


// METHOD IMPLEMENTATION
void App::update_screen() {
    SDL_UpdateWindowSurface(window);
}



//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=//
// GETTER AND SETTER METHODS                                                 //
//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=//

// METHOD IMPLEMENTATION
SDL_Surface* App::get_surface() {
    return this->window_surface;
}


// METHOD IMPLEMENTATION
int App::get_window_width() {
    return this->window_width;
}


// METHOD IMPLEMENTATION
int App::get_window_height() {
    return this->window_height;
}


// METHOD IMPLEMENTATION
int App::get_screen_width() {
    return this->screen_width;
}


// METHOD IMPLEMENTATION
int App::get_screen_height() {
    return this->screen_height;
}



//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=//
// SCREEN CHANGE METHODS                                                     //
//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=//

// METHOD IMPLEMENTATION
void App::change_screen_state(AppState new_state) {
    // Unloading resources for the current state.
    switch (this->app_state) {
        case AppState::MENU_SCREEN:
            this->unload_menu_screen();
            break;
        case AppState::NEW_PROJECT_SCREEN:
            this->unload_new_project_screen();
            break;
        case AppState::RENDERING_SCREEN:
            this->unload_rendering_screen();
            break;
    }

    // Definition of the new state.
    this->app_state = new_state;

    // Loading resources for the new state.
    switch (this->app_state) {
        case AppState::MENU_SCREEN:
            this->load_menu_screen();
            break;
        case AppState::NEW_PROJECT_SCREEN:
            this->load_new_project_screen();
            break;
        case AppState::RENDERING_SCREEN:
            this->load_rendering_screen();
            break;
    }
}


// METHOD IMPLEMENTATION
void App::load_menu_screen() {
    this->new_drawing_button = new ButtonComponent(
        static_cast<int>(window_width * this->new_drawing_button_relative_x_percent - this->primary_button_width / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->primary_button_height / 2),
        this->primary_button_width,
        this->primary_button_height,
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Start a new drawing",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->load_project_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - this->primary_button_width / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->primary_button_height / 2) + this->primary_button_height + 15,
        this->primary_button_width,
        this->primary_button_height,
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Open project file",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    text_title_surface = TTF_RenderText_Blended(FontManager::libertinus_regular_36, "BRUSHY", Colors::uint32_to_sdlcolor(this->window_surface, Colors::rgb_to_uint32(this->window_surface, 0, 0, 0)));

    if (!text_title_surface) {
        ErrorHandler::fatal_error("There was a problem rendering text: %s.\n", TTF_GetError());
    }

    text_rect.w = text_title_surface->w;
    text_rect.h = text_title_surface->h;
    text_rect.x = (window_width - text_rect.w) / 2;
    text_rect.y = this->new_drawing_button->y - text_rect.h - 30;

    this->app_icon_image = new ImageComponent(
        "assets/Icon_256.bmp",
        (window_width - App::main_image_size) / 2,
        this->text_rect.y - App::main_image_size - App::bottom_image_margin
    );
}


// METHOD IMPLEMENTATION
void App::unload_menu_screen() {
    if (this->new_drawing_button != nullptr) {
        delete this->new_drawing_button;
        this->new_drawing_button = nullptr;
    }

    if (this->load_project_button != nullptr) {
        delete this->load_project_button;
        this->load_project_button = nullptr;
    }
}


// METHOD IMPLEMENTATION
void App::load_new_project_screen() {
    this->app_bar_project_screen = new AppBar(this->window_width, App::app_bar_height, "Creating a new project", FontManager::roboto_semibold_20);
    this->app_bar_project_screen->set_background_color({255, 255, 255, 255});
    this->app_bar_project_screen->setTextColor({0, 0, 0, 255});

    int btn_w = 300; // REMOVER
    int icon_x = (window_width - 256) / 2;
    int icon_y = 10;
    int icon_w = 256;
    int icon_h = 256;
    int textbox_w = 300;
    int textbox_h = 40;
    int textbox_x = 50;
    int textbox_y = 100;

    this->create_project_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->primary_button_height / 2),
        btn_w,
        this->primary_button_height,
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "secondary_background_button"),
        "Create project",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->back_menu_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->primary_button_height / 2) + this->primary_button_height + 15,
        btn_w,
        this->primary_button_height,
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Back to menu",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->width_textbox = new TextboxComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - textbox_w / 2),
        textbox_y,
        textbox_w,
        textbox_h,
        FontManager::roboto_semibold_20,
        true
    );

    this->height_textbox = new TextboxComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - textbox_w / 2),
        textbox_y + textbox_h + 60,
        textbox_w, textbox_h,
        FontManager::roboto_semibold_20,
        true
    );
}


// METHOD IMPLEMENTATION
void App::unload_new_project_screen() {
    if (this->app_bar_project_screen != nullptr) {
        delete this->app_bar_project_screen;
        this->app_bar_project_screen = nullptr;
    }

    if (this->back_menu_button != nullptr) {
        delete this->back_menu_button;
        this->back_menu_button = nullptr;
    }

    if (this->app_bar_project_screen != nullptr) {
        delete this->app_bar_project_screen;
        this->app_bar_project_screen = nullptr;
    }

    if (this->width_textbox != nullptr) {
        delete this->width_textbox;
        this->width_textbox = nullptr;
    }

    if (this->height_textbox != nullptr) {
        delete this->height_textbox;
        this->height_textbox = nullptr;
    }
}


// METHOD IMPLEMENTATION
void App::load_rendering_screen() {
    this->app_bar_rendering_screen = new AppBar(this->window_width, App::app_bar_height, "", FontManager::roboto_semibold_20);
    this->app_bar_rendering_screen->set_background_color({255, 255, 255, 255});
    this->app_bar_rendering_screen->setTextColor({0, 0, 0, 255});

    this->pencil_button = new ButtonComponent(
        static_cast<int>(window_width * 0.01),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Pencil",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->eraser_button = new ButtonComponent(
        static_cast<int>(window_width * 0.1),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Eraser",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->bucket_button = new ButtonComponent(
        static_cast<int>(window_width * 0.19),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Bucket",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->line_button = new ButtonComponent(
        static_cast<int>(window_width * 0.28),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Line",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->house_button = new ButtonComponent(
        static_cast<int>(window_width * 0.37),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "House",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->tree_button = new ButtonComponent(
        static_cast<int>(window_width * 0.46),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Tree",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->fence_button = new ButtonComponent(
        static_cast<int>(window_width * 0.55),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Fence",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->sun_button = new ButtonComponent(
        static_cast<int>(window_width * 0.64),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Sun",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->primary_selector = new ButtonComponent(
        static_cast<int>(window_width * 0.73),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/8),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "C1",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->secondary_selector = new ButtonComponent(
        static_cast<int>(window_width * 0.78),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/8),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "C2",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->tertiary_selector = new ButtonComponent(
        static_cast<int>(window_width * 0.83),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/8),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "C3",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->green_button = new ButtonComponent(
        static_cast<int>(window_width * 0.88),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/8),
        static_cast<int>(this->primary_button_height / 3),
        Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "green"),
        "",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->red_button = new ButtonComponent(
        static_cast<int>(window_width * 0.88),
        static_cast<int>(window_height * 0.05),
        static_cast<int>(this->primary_button_width/8),
        static_cast<int>(this->primary_button_height / 3),
        Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "red"),
        "",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->blue_button = new ButtonComponent(
        static_cast<int>(window_width * 0.93),
        static_cast<int>(window_height * 0.05),
        static_cast<int>(this->primary_button_width/8),
        static_cast<int>(this->primary_button_height / 3),
        Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "aqua"),
        "",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->save_button = new ButtonComponent(
        static_cast<int>(window_width * 0.98),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->primary_button_width/4),
        static_cast<int>(this->primary_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Save",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );
}


// METHOD IMPLEMENTATION
void App::unload_rendering_screen() {
    if (this->app_bar_rendering_screen != nullptr) {
        delete this->app_bar_rendering_screen;
        this->app_bar_rendering_screen = nullptr;
    }

    if (this->pencil_button != nullptr) {
        delete this->pencil_button;
        this->pencil_button = nullptr;
    }

    if (this->eraser_button != nullptr) {
        delete this->eraser_button;
        this->eraser_button = nullptr;
    }

    if (this->bucket_button != nullptr) {
        delete this->bucket_button;
        this->bucket_button = nullptr;
    }

    if (this->line_button != nullptr) {
        delete this->line_button;
        this->line_button = nullptr;
    }

    if (this->house_button != nullptr) {
        delete this->house_button;
        this->house_button = nullptr;
    }

    if (this->tree_button != nullptr) {
        delete this->tree_button;
        this->tree_button = nullptr;
    }

    if (this->fence_button != nullptr) {
        delete this->fence_button;
        this->fence_button = nullptr;
    }

    if (this->sun_button != nullptr) {
        delete this->sun_button;
        this->sun_button = nullptr;
    }

    if (this->save_button != nullptr) {
        delete this->save_button;
        this->save_button = nullptr;
    }
}



//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=//
// SCREEN RENDER METHODS                                                     //
//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=//

// METHOD IMPLEMENTATION
void App::render_menu_screen() {
    // Renders the background surface.
    SDL_FillRect(this->window_surface, nullptr, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "main_background_window"));

    // Renders the program name.
    SDL_BlitSurface(text_title_surface, NULL, window_surface, &text_rect);

    this->new_drawing_button->draw(window_surface);
    this->load_project_button->draw(window_surface);
    this->app_icon_image->draw(window_surface);
}


// METHOD IMPLEMENTATION
void App::render_new_project_screen() {
    // Enables text input only if any textbox is active.
    if ((this->width_textbox && this->width_textbox->get_active()) || (this->height_textbox && this->height_textbox->get_active())) {
        SDL_StartTextInput();
    } else {
        SDL_StopTextInput();
    }

    // Renders the background surface.
    SDL_FillRect(this->window_surface, nullptr, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_window"));

    if (width_textbox) {
        width_textbox->render(window_surface);
    }

    if (height_textbox) {
        height_textbox->render(window_surface);
    }

    // Draws textbox captions.
    if (text_enter_width_surface && text_enter_height_surface) {
        text_rect.w = text_enter_width_surface->w;
        text_rect.h = text_enter_width_surface->h;
        text_rect.x = (window_width - text_rect.w) / 2;
        text_rect.y = width_textbox->rect.y - 30;
        SDL_BlitSurface(text_enter_width_surface, NULL, window_surface, &text_rect);

        text_rect.w = text_enter_height_surface->w;
        text_rect.h = text_enter_height_surface->h;
        text_rect.x = (window_width - text_rect.w) / 2;
        text_rect.y = height_textbox->rect.y - 30;
        SDL_BlitSurface(text_enter_height_surface, NULL, window_surface, &text_rect);
    }

    this->create_project_button->draw(window_surface);
    this->back_menu_button->draw(window_surface);
    this->app_bar_project_screen->draw(this->window_surface);
}


// METHOD IMPLEMENTATION
void App::render_rendering_screen() {
    // Renders the background surface.
    SDL_FillRect(this->window_surface, nullptr, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_window"));

    // Renders the drawing surface.
    SDL_FillRect(this->drawing_surface, nullptr, this->background_drawing_color);

    for (auto& seg : lines) {
        Point& p0 = seg[0];
        Point& p1 = seg[1];
        Primitives::draw_line(drawing_surface, p0.get_x(), p0.get_y(), p1.get_x(), p1.get_y(), p1.color, true);
    }

    for (size_t i = 0; i < shapes.size(); ++i) {
        shapes[i]->draw(drawing_surface);
    }

    for (Point p : this->points) {
        Primitives::set_pixel(drawing_surface, p.get_x(), p.get_y(), p.color);
    }

    for (Point p : this->fill_points) {
        Uint32 p_color = SDL_MapRGB(drawing_surface->format, 0, 240, 100); //TODO: TROCAR PARA COR PRIMÁRIA
        Primitives::flood_fill(drawing_surface, p.get_x(), p.get_y(), p.color);
    }

    for (Point p : this->eraser_points) {
        Uint32 p_color = SDL_MapRGB(drawing_surface->format, 255, 255, 255); //TODO: TROCAR PARA COR DE FUNDO
        Primitives::set_pixel(drawing_surface, p.get_x(), p.get_y(), this->background_drawing_color);
    }

    SDL_Rect drawing_surface_rectangle;
    drawing_surface_rectangle.w = drawing_surface->w;
    drawing_surface_rectangle.h = drawing_surface->h;
    drawing_surface_rectangle.x = (window_width - drawing_surface_rectangle.w - 2) / 2;
    drawing_surface_rectangle.y = (window_height - drawing_surface_rectangle.h + App::app_bar_height) / 2;
    SDL_BlitSurface(drawing_surface, nullptr, window_surface, &drawing_surface_rectangle);

    // Draws the components of the graphical interface.
    this->app_bar_rendering_screen->draw(this->window_surface);
    this->pencil_button->draw(this->window_surface);
    this->eraser_button->draw(this->window_surface);
    this->bucket_button->draw(this->window_surface);
    this->line_button->draw(this->window_surface);
    this->house_button->draw(this->window_surface);
    this->tree_button->draw(this->window_surface);
    this->fence_button->draw(this->window_surface);
    this->sun_button->draw(this->window_surface);
    this->primary_selector->draw(this->window_surface);
    this->secondary_selector->draw(this->window_surface);
    this->tertiary_selector->draw(this->window_surface);
    this->green_button->draw(this->window_surface);
    this->red_button->draw(this->window_surface);
    this->blue_button->draw(this->window_surface);
    this->save_button->draw(this->window_surface);
}


// METHOD IMPLEMENTATION
bool App::recreate_drawing_surface(int new_width, int new_height) {
    if (this->drawing_surface != nullptr) {
        SDL_FreeSurface(this->drawing_surface);
        this->drawing_surface = nullptr;
    }

    this->drawing_surface = SDL_CreateRGBSurface(
        0,
        new_width,
        new_height,
        32,
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
        0x00000000
    );

    if (!this->drawing_surface) {
        ErrorHandler::fatal_error("Unable to recreate drawing surface: %s", SDL_GetError());
        return false;
    }

    return true;
}
