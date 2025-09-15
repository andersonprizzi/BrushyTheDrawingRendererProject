#include "App.h"

/////////////////////////////////////////////////////////////////////////////////////////
// ATTRIBUTE INITIALIZATION                                                            //
/////////////////////////////////////////////////////////////////////////////////////////

float App::new_drawing_button_relative_x_percent = 0.50f;       // Posição horizontal relativa
float App::new_drawing_button_relative_y_percent = 0.70f;       // Posição vertical relativa
float App::load_file_button_relative_x_percent = 0.50f;         // Posição horizontal relativa
int   App::default_button_height = 45;



/////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR DEFINITION                                                              //
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 *      Implementation of the main application's constructor. Initializes
 *      the SDL library, creates the window, and retrieves the drawing surface.
*/
App::App(const std::string& title, float width_percent, float height_percent) {
    this->window = nullptr;
    this->surface = nullptr;
    this->running = false;
    this->window_width = 0;
    this->window_height = 0;
    this->window_title = title;
    this->app_state = AppState::MENU_SCREEN;

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
        ErrorHandler::loaded_fonts = true;
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

    // Initializing app icon.
    SDL_Surface* icon = SDL_LoadBMP("assets/Icon_64.bmp");

    if (!icon) {
        ErrorHandler::fatal_error("Failed to load application icon: %s.", SDL_GetError());
    } else {
        SDL_SetWindowIcon(this->window, icon);
        SDL_FreeSurface(icon);
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR DEFINITION                                                               //
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 *      Implementation of the main application destructor.
 */
App::~App() {
    this->exit_app();
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

void App::exit_app() {
    SDL_DestroyWindow(this->window);

    if (ErrorHandler::loaded_fonts) {
        FontManager::close_fonts();
    }

    TTF_Quit();
    SDL_Quit();
}


/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
void App::run() {
    this->running = true;

    // Initializing screen components.
    int btn_w = 300;
    int icon_x = (window_width - 256) / 2;
    int icon_y = 10;
    int icon_w = 256;
    int icon_h = 256;

    // Renderiza o texto para uma surface
    SDL_Color textColor = {0, 0, 0}; // preto
    textSurface = TTF_RenderText_Blended(FontManager::libertinus_regular_36, "BRUSHY", textColor);
    text_enter_height = TTF_RenderText_Blended(FontManager::roboto_semibold_20, "Enter height:", textColor);
    text_enter_width = TTF_RenderText_Blended(FontManager::roboto_semibold_20, "Enter width:", textColor);

    if (!textSurface) {
        fprintf(stderr, "Erro ao renderizar texto: %s\n", TTF_GetError());
    } else {
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = (window_width - textRect.w) / 2;
        textRect.y = icon_y + icon_h + 10;
    }

    myImageComponent = new ImageComponent(
        "assets/Icon_256.bmp",
        icon_x,
        icon_y
    );

    this->new_drawing_button = new ButtonComponent(
        static_cast<int>(window_width * this->new_drawing_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2),
        btn_w,
        this->default_button_height,
        Colors::get_color_by_name(surface, "silver"),
        "Start a new drawing",
        FontManager::roboto_semibold_20,
        {0, 0, 0}
    );

    this->load_project_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2) + this->default_button_height + 15,
        btn_w,
        this->default_button_height,
        Colors::get_color_by_name(surface, "silver"),
        "Open description file",
        FontManager::roboto_semibold_20,
        {0, 0, 0}
    );

    this->create_project_button = new ButtonComponent(
        static_cast<int>(window_width * this->load_file_button_relative_x_percent - btn_w / 2),
        static_cast<int>(window_height * this->new_drawing_button_relative_y_percent - this->default_button_height / 2),
        btn_w,
        this->default_button_height,
        Colors::get_color_by_name(surface, "silver"),
        "Create project",
        FontManager::roboto_semibold_20,
        {0, 0, 0}
    );

    // Criar a textbox (aceita apenas números)
    int textbox_w = 300;
    int textbox_h = 40;
    int textbox_x = 50;
    int textbox_y = 100;

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

    SDL_StartTextInput(); // necessário para capturar SDL_TEXTINPUT

    // Execution loop.
    while (running) {
        if (this->app_state == AppState::MENU_SCREEN) {
            clear_screen(255, 255, 255);
            new_drawing_button->draw(surface);
            load_project_button->draw(surface);
            myImageComponent->draw(surface);

            if (textSurface) {
                SDL_BlitSurface(textSurface, NULL, surface, &textRect);
            }

        } else if (this->app_state == AppState::NEW_PROJECT_SCREEN) {
            clear_screen(255, 255, 255);

            if (text_enter_width && text_enter_height) {
                textRect.w = text_enter_width->w;
                textRect.h = text_enter_width->h;
                textRect.x = (window_width - textRect.w) / 2;
                textRect.y = width_textbox->rect.y - 30;
                SDL_BlitSurface(text_enter_width, NULL, surface, &textRect);

                textRect.w = text_enter_height->w;
                textRect.h = text_enter_height->h;
                textRect.x = (window_width - textRect.w) / 2;
                textRect.y = height_textbox->rect.y - 30;
                SDL_BlitSurface(text_enter_height, NULL, surface, &textRect);
            }

            if (width_textbox) {
                width_textbox->render(surface);
            }

            if (height_textbox) {
                height_textbox->render(surface);
            }

            create_project_button->draw(surface);

        } else if (this->app_state == AppState::RENDERING_SCREEN) {
            clear_screen(255, 255, 255);

            Uint32 red = Colors::get_color_by_name(this->surface, "red");
            Uint32 green = Colors::get_color_by_name(this->surface, "green");
            Uint32 blue = Colors::get_color_by_name(this->surface, "blue");
            Uint32 black = Colors::get_color_by_name(this->surface, "black");

            Primitives::draw_line(this->surface, 0, 0, 500, 500, blue, true);
            Primitives::draw_line(this->surface, 200, 100, 450, 550, blue, false);
            Primitives::draw_curve(this->surface, 20, 10, 250, 200, 190, 350, 240, 420, red, true);
            Primitives::draw_circle(this->surface, 300, 300, 100, black, true, false);
            //Primitives::draw_circle(this->surface, 450, 450, 100, green, false, false);
            //Primitives::draw_circle(this->surface, 100, 400, 50, green, false, true);
            //Primitives::draw_circle(this->surface, 480, 110, 50, red, true, true);
            Primitives::draw_ellipse(this->surface, 680, 89, 100, 50, red, false, false);
            Primitives::draw_ellipse(this->surface, 750, 450, 150, 50, black, false, true);
            Primitives::draw_ellipse(this->surface, 750, 300, 150, 50, black, true, true);
            Primitives::draw_ellipse(this->surface, 1050, 300, 100, 150, blue, true, false);

            Circle *circle1 = new Circle(150, 400, 75, true, true, green);
            circle1->draw(this->surface);

            /*
            // Criar um retângulo vermelho
            Rectangle rect(50, 50, 200, 150, red);
            rect.draw(surface, true);

            Rectangle rect2(50, 170, 200, 250, red);
            rect2.draw(surface, true, true);

            Circle circ(300, 300, 100, black);
            circ.draw(surface, false);

            //CircleFilled circf(300, 300, 70, green);
            //circf.draw(surface);
            Circle filled_circ(300, 300, 70, green);
            filled_circ.draw(surface, true);

            //CircleFilledAA circfa(500, 100, 70, blue);
            //circfa.draw(surface, true);

            Circle aa_filled_circ(500, 100, 70, blue);
            aa_filled_circ.draw(surface, true, true);
            */
        }
        this->handle_events();
        this->update_screen();
    }

    if (textSurface) SDL_FreeSurface(textSurface);

    if (width_textbox) {
        delete width_textbox;
        width_textbox = nullptr;
    }

    SDL_StopTextInput();
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

void App::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

            if (SDL_SaveBMP(screenSurface, "screenshot.bmp") == 0) {
                printf("Tela salva como screenshot.bmp\n");
            } else {
                printf("Erro ao salvar BMP: %s\n", SDL_GetError());
            }

            running = false;
        }

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

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mx = event.button.x;
            int my = event.button.y;

            if (this->app_state == AppState::MENU_SCREEN && new_drawing_button->is_clicked(mx, my)) {
                this->app_state = AppState::NEW_PROJECT_SCREEN;

            } else if (this->app_state == AppState::NEW_PROJECT_SCREEN && create_project_button->is_clicked(mx, my)) {
                SDL_SetWindowSize(this->window, std::stoi(width_textbox->getText()), std::stoi(height_textbox->getText()));
                SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                this->app_state = AppState::RENDERING_SCREEN;
            }
        }

        if (width_textbox) {
            width_textbox->handleEvent(&event);
        }

        if (height_textbox) {
            height_textbox->handleEvent(&event);
        }
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

void App::clear_screen(Uint8 r, Uint8 g, Uint8 b) {
    Uint32 color = SDL_MapRGB(surface->format, r, g, b);
    SDL_FillRect(surface, NULL, color);
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

void App::update_screen() {
    SDL_UpdateWindowSurface(window);
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

SDL_Surface* App::get_surface() {
    return this->surface;
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

int App::get_window_width() {
    return this->window_width;
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

int App::get_window_height() {
    return this->window_height;
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

int App::get_screen_width() {
    return this->screen_width;
}



/////////////////////////////////////////////////////////////////////////////////////////
// METHOD DEFINITION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////

int App::get_screen_height() {
    return this->screen_height;
}
