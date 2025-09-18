#include "App.h"


// STATIC ATTRIBUTES INITIALIZATION
float App::new_drawing_button_relative_x_percent = 0.50f;       // Posição horizontal relativa
float App::new_drawing_button_relative_y_percent = 0.70f;       // Posição vertical relativa
float App::load_file_button_relative_x_percent = 0.50f;         // Posição horizontal relativa
int   App::default_button_height = 45;


// CONSTRUCTOR IMPLEMENTATION
/**
 * @brief
 * Implementation of the main application's constructor. Initializes
 * the SDL library, creates the window, and retrieves the drawing surface.
*/
App::App(const std::string& title, float width_percent, float height_percent) {
    this->window = nullptr;
    this->surface = nullptr;
    this->drawing_surface = nullptr;
    this->running = false;
    this->window_width = 0;
    this->window_height = 0;
    this->window_title = title;
    //this->app_state = AppState::MENU_SCREEN;
    this->app_state = AppState::RENDERING_SCREEN; // REMOVER DEPOIS.
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

    this->surface = SDL_GetWindowSurface(window);
    this->drawing_surface = SDL_CreateRGBSurface(0, window_width, window_height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000);
    // Preencher o fundo branco uma vez
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




    // REMOVER DEPOIS.
    SDL_SetWindowSize(this->window, 1200, 600);
    SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}


// METHOD IMPLEMENTATION
void App::run() {
    this->running = true;


    // Initializing screen components.
    int btn_w = 300;
    int icon_x = (window_width - 256) / 2;
    int icon_y = 10;
    int icon_w = 256;
    int icon_h = 256;
    int textbox_w = 300;
    int textbox_h = 40;
    int textbox_x = 50;
    int textbox_y = 100;


    // Initializing the app bar.
    AppBar app_bar(this->window_width, 50, "Creating a new project", FontManager::roboto_semibold_20);
    app_bar.setBackgroundColor({255, 255, 255, 255});
    app_bar.setTextColor({0, 0, 0, 255});


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
    this->new_drawing_button = new ButtonComponent(
        static_cast<int>(window_width * this->new_drawing_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2),
        btn_w,
        this->default_button_height,
        Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Start a new drawing",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->surface, Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->load_project_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2) + this->default_button_height + 15,
        btn_w,
        this->default_button_height,
        Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Open project file",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->surface, Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->create_project_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2),
        btn_w,
        this->default_button_height,
        Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "secondary_background_button"),
        "Create project",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->surface, Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
    );

    this->back_menu_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2) + this->default_button_height + 15,
        btn_w,
        this->default_button_height,
        Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_button"),
        "Back to menu",
        FontManager::roboto_semibold_20,
        Colors::uint32_to_sdlcolor(this->surface, Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "button_text_color"))
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

    Uint32 red = Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "red");
    Uint32 green = Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "green");
    Uint32 blue = Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "blue");
    Uint32 black = Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "black");

    Polygon *polygon_1 = new Polygon(true, false, green);
    polygon_1->add_point(Point(350, 400));
    polygon_1->add_point(Point(260, 300));
    polygon_1->add_point(Point(268, 250));


    // Execution loop.
    while (running) {
        if (this->app_state == AppState::MENU_SCREEN) {
            clear_screen(Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "white"));
            new_drawing_button->draw(surface);
            load_project_button->draw(surface);
            app_icon_image->draw(surface);

            text_rect.w = text_title_surface->w;
            text_rect.h = text_title_surface->h;
            text_rect.x = (window_width - text_rect.w) / 2;
            text_rect.y = icon_y + icon_h + 10;
            SDL_BlitSurface(text_title_surface, NULL, surface, &text_rect);

        } else if (this->app_state == AppState::NEW_PROJECT_SCREEN) {
            // Enables text input only if any textbox is active.
            if ((width_textbox && width_textbox->get_active()) || (height_textbox && height_textbox->get_active())) {
                SDL_StartTextInput();
            } else {
                SDL_StopTextInput();
            }

            // Clears the screen with the default background color.
            clear_screen(Colors::get_color(this->surface, Colors::interface_colors_table, Colors::number_of_interface_colors, "primary_background_window"));

            // Draw the app bar.
            app_bar.draw(this->surface);

            // Draws textbox captions.
            if (text_enter_width_surface && text_enter_height_surface) {
                text_rect.w = text_enter_width_surface->w;
                text_rect.h = text_enter_width_surface->h;
                text_rect.x = (window_width - text_rect.w) / 2;
                text_rect.y = width_textbox->rect.y - 30;
                SDL_BlitSurface(text_enter_width_surface, NULL, surface, &text_rect);

                text_rect.w = text_enter_height_surface->w;
                text_rect.h = text_enter_height_surface->h;
                text_rect.x = (window_width - text_rect.w) / 2;
                text_rect.y = height_textbox->rect.y - 30;
                SDL_BlitSurface(text_enter_height_surface, NULL, surface, &text_rect);
            }

            if (width_textbox) {
                width_textbox->render(surface);
            }

            if (height_textbox) {
                height_textbox->render(surface);
            }

            create_project_button->draw(surface);
            back_menu_button->draw(surface);

        } else if (this->app_state == AppState::RENDERING_SCREEN) {
            // Limpa a janela com cinza, se quiser bordas, ou com branco se quiser uniforme
            SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 240, 240, 240));

            // Limpa o canvas de desenho com fundo branco
            SDL_FillRect(drawing_surface, nullptr, SDL_MapRGB(drawing_surface->format, 255, 255, 255));

            polygon_1->draw(drawing_surface);

            SDL_Rect dst_rect;
            dst_rect.w = drawing_surface->w;
            dst_rect.h = drawing_surface->h;
            dst_rect.x = (window_width - dst_rect.w) / 2;
            dst_rect.y = (window_height - dst_rect.h) / 2;

            SDL_BlitSurface(drawing_surface, nullptr, surface, &dst_rect);
            SDL_UpdateWindowSurface(window);

            /*
            clear_screen(255, 255, 255);

            // Test colors - Remove at the end.
            Uint32 red = Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "red");
            Uint32 green = Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "green");
            Uint32 blue = Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "blue");
            Uint32 black = Colors::get_color(this->surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "black");

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
        this->notification_manager->draw(this->surface);

        this->handle_events();
        this->update_screen();
    }

    this->close();
}


// METHOD IMPLEMENTATION
void App::close(int exit_code) {
    if (text_title_surface) SDL_FreeSurface(text_title_surface);

    if (width_textbox) {
        delete width_textbox;
        width_textbox = nullptr;
    }

    SDL_StopTextInput();

    SDL_DestroyWindow(this->window);

    if (ErrorHandler::get_loaded_fonts() == true) {
        FontManager::close_fonts();
    }

    TTF_Quit();
    SDL_Quit();
    exit(exit_code);
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
            surface = SDL_GetWindowSurface(window);
            this->window_width = surface->w;
            this->window_height = surface->h;

            this->new_drawing_button->set_position(
                static_cast<int>(window_width * this->new_drawing_button_relative_x_percent - new_drawing_button->w / 2),
                static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - new_drawing_button->h / 2)
            );

            this->load_project_button->set_position(
                static_cast<int>(window_width * this->load_file_button_relative_x_percent - load_project_button->w / 2),
                static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2) + this->default_button_height + 15
            );
        }

        // Processes mouse click events.
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mx = event.button.x;
            int my = event.button.y;

            // Screen change: MENU_SCREEN > NEW_PROJECT_SCREEN
            if (this->app_state == AppState::MENU_SCREEN && new_drawing_button->is_clicked(mx, my)) {
                this->app_state = AppState::NEW_PROJECT_SCREEN;

            // Screen change: NEW_PROJECT_SCREEN > RENDERING_SCREEN
            } else if (this->app_state == AppState::NEW_PROJECT_SCREEN && create_project_button->is_clicked(mx, my)) {
                if (width_textbox->getText().empty()) {
                    this->notification_manager->push({
                        "Warning!",
                        "The width field is required.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                    continue;
                }

                if (height_textbox->getText().empty()) {
                    this->notification_manager->push({
                        "Warning!",
                        "The height field is required.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                    continue;
                }

                if (Utils::check_window_size(this, std::stoi(width_textbox->getText()), std::stoi(height_textbox->getText()))) {
                    SDL_SetWindowSize(this->window, std::stoi(width_textbox->getText()), std::stoi(height_textbox->getText()));
                    SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    this->app_state = AppState::RENDERING_SCREEN;
                } else {
                    this->notification_manager->push({
                        "Warning!",
                        "The specified width or height is below the minimum allowed or above your monitor's resolution.",
                        { this->window_width - 20 - 300, this->window_height - 20 - 80, 300, 80 },
                    });
                }

            // Screen change: NEW_PROJECT_SCREEN > MENU_SCREEN
            } else if (this->app_state == AppState::NEW_PROJECT_SCREEN && back_menu_button->is_clicked(mx, my)) {
                this->app_state = AppState::MENU_SCREEN;
            }
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
    if (!surface) return;
    SDL_FillRect(surface, nullptr, color);
}


// METHOD IMPLEMENTATION
void App::clear_screen(Uint8 r, Uint8 g, Uint8 b) {
    if (!surface) return;
    Uint32 color = SDL_MapRGB(surface->format, r, g, b);
    SDL_FillRect(surface, NULL, color);
}


// METHOD IMPLEMENTATION
void App::update_screen() {
    SDL_UpdateWindowSurface(window);
}


// METHOD IMPLEMENTATION
SDL_Surface* App::get_surface() {
    return this->surface;
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
