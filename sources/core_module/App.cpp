#include "App.h"


// STATIC ATTRIBUTES INITIALIZATION
float App::new_drawing_button_relative_x_percent = 0.50f;       // Posição horizontal relativa
float App::new_drawing_button_relative_y_percent = 0.70f;       // Posição vertical relativa
float App::load_file_button_relative_x_percent = 0.50f;         // Posição horizontal relativa
int App::default_button_height = 45;
int App::secondary_button_width = 300;
int App::app_bar_height = 50;
int App::universe_width = 100;
int App::universe_height = 75;


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

    // Fills the drawing canvas with white.
    SDL_FillRect(drawing_surface, nullptr, SDL_MapRGB(drawing_surface->format, 255, 255, 255));

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
}


// METHOD IMPLEMENTATION
void App::run() {
    this->running = true;
    this->load_menu_screen();

    // Initializing screen components.
    int btn_w = 300; // REMOVER
    int icon_x = (window_width - 256) / 2;
    int icon_y = 10;
    int icon_w = 256;
    int icon_h = 256;
    int textbox_w = 300;
    int textbox_h = 40;
    int textbox_x = 50;
    int textbox_y = 100;

    // Renderiza o texto para uma surface
    SDL_Color textColor = {0, 0, 0}; // preto
    text_title_surface = TTF_RenderText_Blended(FontManager::libertinus_regular_36, "BRUSHY", textColor);
    text_enter_height_surface = TTF_RenderText_Blended(FontManager::roboto_semibold_20, "Enter height:", textColor);
    text_enter_width_surface = TTF_RenderText_Blended(FontManager::roboto_semibold_20, "Enter width:", textColor);

    if (!text_title_surface || !text_enter_width_surface || !text_enter_height_surface) {
        ErrorHandler::fatal_error("There was a problem rendering text: %s.\n", TTF_GetError());
    }


    // Creating image components.
    this->app_icon_image = new ImageComponent(
        "assets/Icon_256.bmp",
        icon_x,
        icon_y
    );


    // Creating button components.
    this->create_project_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2),
        btn_w,
        this->default_button_height,
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "secondary_background_button"),
        "Create project",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->back_menu_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2) + this->default_button_height + 15,
        btn_w,
        this->default_button_height,
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Back to menu",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    // Creating textbox components.
    width_textbox = new TextboxComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - textbox_w / 2),
        textbox_y,
        textbox_w,
        textbox_h,
        FontManager::roboto_semibold_20,
        true
    );

    height_textbox = new TextboxComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - textbox_w / 2),
        textbox_y + textbox_h + 60,
        textbox_w, textbox_h,
        FontManager::roboto_semibold_20,
        true
    );

    Uint32 red = Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "red");
    Uint32 green = Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "green");
    Uint32 blue = Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "blue");
    Uint32 black = Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "black");

    // Execution loop.
    while (running) {
        if (this->app_state == AppState::MENU_SCREEN) {
            this->render_menu_screen();
            clear_screen(Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "white"));
            new_drawing_button->draw(window_surface);
            load_project_button->draw(window_surface);
            app_icon_image->draw(window_surface);

            text_rect.w = text_title_surface->w;
            text_rect.h = text_title_surface->h;
            text_rect.x = (window_width - text_rect.w) / 2;
            text_rect.y = icon_y + icon_h + 10;
            SDL_BlitSurface(text_title_surface, NULL, window_surface, &text_rect);

        } else if (this->app_state == AppState::NEW_PROJECT_SCREEN) {
            this->render_new_project_screen();

            // Enables text input only if any textbox is active.
            if ((width_textbox && width_textbox->get_active()) || (height_textbox && height_textbox->get_active())) {
                SDL_StartTextInput();
            } else {
                SDL_StopTextInput();
            }

            // Clears the screen with the default background color.
            clear_screen(Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_window"));

            // Draw the app bar.
            app_bar_project_screen->draw(this->window_surface);

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

            if (width_textbox) {
                width_textbox->render(window_surface);
            }

            if (height_textbox) {
                height_textbox->render(window_surface);
            }

            create_project_button->draw(window_surface);
            back_menu_button->draw(window_surface);

        } else if (this->app_state == AppState::RENDERING_SCREEN) {
            this->render_rendering_screen();



            /*
            Sun sol = Sun(20, 20, 50, 35, green, red);
            //sol.scale(3, 1);
            sol.rotate_figure(15);
            sol.draw(drawing_surface);

            Fence cerca2 = Fence(20, 20, 30, 35, black, green);
            cerca2.rotate_figure(50);
            cerca2.scale(1.1, 0.5);
            cerca2.translate(-15, 13);
            cerca2.draw(drawing_surface);

            //House casinha = House(30,20,50,35,red,blue,green);
            //casinha.draw(drawing_surface);

            //casinha.change_origin(Point(50, 10));

            //casinha.translate(-10.0, 10.0);
            //Point centro = Point(casinha.x_origin + 0.5 * casinha.width,casinha.y_origin + 0.5 * casinha.height);
            //casinha.rotate_figure(drawing_surface, 45.0, centro);
            //casinha.rotate_figure(45.0);
            //casinha.draw(drawing_surface);

            //casinha.rotate_figure(45.0);
            //casinha.scale(1.7, 0.5);
            //casinha.draw(drawing_surface);

            */


            /*
            //Primitives::draw_line(this->surface, 0, 0, 500, 500, blue, true);
            //Primitives::draw_line(this->surface, 200, 100, 450, 550, blue, false);
            //Primitives::draw_curve(this->surface, 20, 10, 250, 200, 190, 350, 240, 420, red, true);
            //Primitives::draw_circle(this->surface, 150, 150, 100, black, true, false);
            //Primitives::flood_fill(this->surface, 150, 150, red);
            //Primitives::draw_circle(this->surface, 450, 450, 100, green, false, false);
            //Primitives::draw_circle(this->surface, 100, 400, 50, green, false, true);
            //Primitives::draw_circle(this->surface, 480, 110, 50, red, true, true);
            //Primitives::draw_ellipse(this->surface, 680, 89, 100, 50, red, false, false);
            //Primitives::draw_ellipse(this->surface, 750, 450, 150, 50, black, false, true);
            //Primitives::draw_ellipse(this->surface, 750, 300, 150, 50, black, true, true);
            //Primitives::draw_ellipse(this->surface, 1050, 300, 100, 150, blue, true, false);

            Polygon *polygon_2 = new Polygon(false, true, green, green, Point(110, 120));
            polygon_2->add_point(Point(100, 100));
            polygon_2->add_point(Point(200, 100));
            polygon_2->add_point(Point(200, 200));
            polygon_2->add_point(Point(100, 200));
            polygon_2->draw(this->surface);
            */
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
            /*
            SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
            if (SDL_SaveBMP(screenSurface, "screenshot.bmp") == 0) {
                printf("Tela salva como screenshot.bmp\n");
            } else {
                printf("Erro ao salvar BMP: %s\n", SDL_GetError());
            }
            */
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
                    static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2) + this->default_button_height + 15
                );
            }

            if (this->app_bar_project_screen) {
                this->app_bar_project_screen->set_size(this->window_width, 50);
            }

            if (this->app_bar_rendering_screen) {
                this->app_bar_rendering_screen->set_size(this->window_width, 50);
            }
        }

        // Processes mouse click events.
        int mx = event.button.x;
        int my = event.button.y;

        // For veryfying click in the drawing surface
        SDL_Rect dst_rect;
        dst_rect.w = drawing_surface->w;
        dst_rect.h = drawing_surface->h;
        dst_rect.x = (window_width  - dst_rect.w) / 2;
        dst_rect.y = (window_height - dst_rect.h) / 2;

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT && inside_rect(mx, my, dst_rect)) mouse_down = true;

            // Screen change: MENU_SCREEN > NEW_PROJECT_SCREEN
            if (this->app_state == AppState::MENU_SCREEN && new_drawing_button->is_clicked(mx, my)) {
                this->change_screen_state(AppState::NEW_PROJECT_SCREEN);

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

                SDL_SetWindowSize(this->window, std::stoi(width_textbox->get_text()), std::stoi(height_textbox->get_text()));
                SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                this->change_screen_state(AppState::RENDERING_SCREEN);

            // Screen change: NEW_PROJECT_SCREEN > MENU_SCREEN
            } else if (this->app_state == AppState::NEW_PROJECT_SCREEN && back_menu_button->is_clicked(mx, my)) {
                this->change_screen_state(AppState::MENU_SCREEN);

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
            } else if (this->app_state == AppState::RENDERING_SCREEN && inside_rect(mx, my, dst_rect) && event.button.button != SDL_BUTTON_RIGHT){
                //DRAWING THINGS
                if (this->mouse_state == MouseState::PENCIL_MODE){
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    this->points.emplace_back(cx, cy);
                } if (this->mouse_state == MouseState::ERASER_MODE){
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    this->eraser_points.emplace_back(cx, cy);
                } else if (this->mouse_state == MouseState::BUCKET_MODE) {
                    int cx = mx - dst_rect.x;     // coordenada X no canvas
                    int cy = my - dst_rect.y;     // coordenada Y no canvas

                    this->fill_points.emplace_back(cx, cy);
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


        } else if (event.type == SDL_MOUSEMOTION && mouse_down){
            mx = event.motion.x;
            my = event.motion.y;

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
                    this->lines.emplace_back(std::array<Point,2>{{ initial_point, Point(cx,cy) }});
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
                         c, c, c)));
                    } else if (this->mouse_state == MouseState::TREE_MODE){
                        shapes.emplace_back(std::unique_ptr<Shape>(new Tree (int(std::lround(ur.w)),
                         int(std::lround(ur.h)),
                         int(std::lround(ur.x)),
                         int(std::lround(ur.y)),
                         c, c, c)));
                    } else if (this->mouse_state == MouseState::FENCE_MODE){
                        shapes.emplace_back(std::unique_ptr<Shape>(new Fence (int(std::lround(ur.w)),
                         int(std::lround(ur.h)),
                         int(std::lround(ur.x)),
                         int(std::lround(ur.y)),
                         c, c)));
                    } else if (this->mouse_state == MouseState::SUN_MODE){
                        shapes.emplace_back(std::unique_ptr<Shape>(new Sun (int(std::lround(ur.w)),
                         int(std::lround(ur.h)),
                         int(std::lround(ur.x)),
                         int(std::lround(ur.y)),
                         c, c)));
                    }
                    // limpa estado do drag
                    this->temporary_in_list = true;
                }
            }
        } else if (event.type == SDL_MOUSEBUTTONUP){
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
void App::clear_screen(Uint32 color) {
    if (!window_surface) return;
    SDL_FillRect(window_surface, nullptr, color);
}


// METHOD IMPLEMENTATION
void App::clear_screen(Uint8 r, Uint8 g, Uint8 b) {
    if (!window_surface) return;
    Uint32 color = SDL_MapRGB(window_surface->format, r, g, b);
    SDL_FillRect(window_surface, NULL, color);
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
        static_cast<int>(window_width * this->new_drawing_button_relative_x_percent - this->secondary_button_width / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2),
        this->secondary_button_width,
        this->default_button_height,
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Start a new drawing",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->load_project_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - this->secondary_button_width / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2) + this->default_button_height + 15,
        this->secondary_button_width,
        this->default_button_height,
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Open project file",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
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
}


// METHOD IMPLEMENTATION
void App::unload_new_project_screen() {
    if (this->app_bar_project_screen != nullptr) {
        delete this->app_bar_project_screen;
        this->app_bar_project_screen = nullptr;
    }
}


// METHOD IMPLEMENTATION
void App::load_rendering_screen() {
    this->app_bar_rendering_screen = new AppBar(this->window_width, App::app_bar_height, "Brushy", FontManager::roboto_semibold_20);
    this->app_bar_rendering_screen->set_background_color({255, 255, 255, 255});
    this->app_bar_rendering_screen->setTextColor({0, 0, 0, 255});

    this->pencil_button = new ButtonComponent(
        static_cast<int>(window_width * 0.01),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->secondary_button_width/4),
        static_cast<int>(this->default_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Pencil",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->eraser_button = new ButtonComponent(
        static_cast<int>(window_width * 0.1),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->secondary_button_width/4),
        static_cast<int>(this->default_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Eraser",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->bucket_button = new ButtonComponent(
        static_cast<int>(window_width * 0.19),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->secondary_button_width/4),
        static_cast<int>(this->default_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Bucket",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->line_button = new ButtonComponent(
        static_cast<int>(window_width * 0.28),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->secondary_button_width/4),
        static_cast<int>(this->default_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Line",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->house_button = new ButtonComponent(
        static_cast<int>(window_width * 0.37),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->secondary_button_width/4),
        static_cast<int>(this->default_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "House",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->tree_button = new ButtonComponent(
        static_cast<int>(window_width * 0.46),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->secondary_button_width/4),
        static_cast<int>(this->default_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Tree",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->fence_button = new ButtonComponent(
        static_cast<int>(window_width * 0.55),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->secondary_button_width/4),
        static_cast<int>(this->default_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Fence",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->window_surface, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->sun_button = new ButtonComponent(
        static_cast<int>(window_width * 0.64),
        static_cast<int>(window_height * 0.01),
        static_cast<int>(this->secondary_button_width/4),
        static_cast<int>(this->default_button_height / 1.5),
        Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Sun",
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
}



//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=//
// SCREEN RENDER METHODS                                                     //
//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=//

// METHOD IMPLEMENTATION
void App::render_menu_screen() {

}


// METHOD IMPLEMENTATION
void App::render_new_project_screen() {

}


// METHOD IMPLEMENTATION
void App::render_rendering_screen() {
    // Renders the background surface.
    SDL_FillRect(this->window_surface, nullptr, Colors::get_color(this->window_surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_window"));

    // Renders the drawing surface.
    SDL_FillRect(this->drawing_surface, nullptr, SDL_MapRGB(drawing_surface->format, 255, 255, 255));

    for (Point p : this->points) {
        //printf("(%f, %f)\n", p.get_x(), p.get_y());
        Uint32 p_color = SDL_MapRGB(drawing_surface->format, 0, 240, 100); //TODO: TROCAR PARA COR PRIMÁRIA
        Primitives::set_pixel(drawing_surface, p.get_x(), p.get_y(), p_color);
    }

    for (Point p : this->fill_points) {
        //printf("(%f, %f)\n", p.get_x(), p.get_y());
        Uint32 p_color = SDL_MapRGB(drawing_surface->format, 0, 240, 100); //TODO: TROCAR PARA COR PRIMÁRIA
        Primitives::flood_fill(drawing_surface, p.get_x(), p.get_y(), p_color);
    }

    Uint32 black = Colors::get_color(this->window_surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "black");

    for (auto& seg : lines) {
        Point& p0 = seg[0];
        Point& p1 = seg[1];
        Primitives::draw_line(drawing_surface, p0.get_x(), p0.get_y(), p1.get_x(), p1.get_y(), black, true);
    }

    for (size_t i = 0; i < shapes.size(); ++i) {
        shapes[i]->draw(drawing_surface);
    }

    for (Point p : this->eraser_points) {
        //printf("(%f, %f)\n", p.get_x(), p.get_y());
        Uint32 p_color = SDL_MapRGB(drawing_surface->format, 255, 255, 255); //TODO: TROCAR PARA COR DE FUNDO
        Primitives::set_pixel(drawing_surface, p.get_x(), p.get_y(), p_color);
    }

    SDL_Rect dst_rect;
    dst_rect.w = drawing_surface->w;
    dst_rect.h = drawing_surface->h;
    dst_rect.x = (window_width - dst_rect.w) / 2;
    dst_rect.y = (window_height - dst_rect.h) / 2;
    SDL_BlitSurface(drawing_surface, nullptr, window_surface, &dst_rect);

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
}
