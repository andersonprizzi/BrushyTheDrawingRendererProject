#include "App.h"

///////////////////////////////////////////////////////////////////////////////

/**
 * \brief Implementation of the main application's constructor. Initializes
 * the SDL library, creates the window, and retrieves the drawing surface.
 *
 * \param title const std::string& Window title.
 * \param width int Initial window width in pixels.
 * \param height int Initial window height in pixels.
 */
App::App(const std::string& title, int width, int height) {
    this->window = nullptr;
    this->surface = nullptr;
    this->running = false;
    this->window_width = width;
    this->window_height = height;
    this->window_title = title;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error initializing SDL library: %s\n", SDL_GetError());
        exit(1);
    }

    this->window = SDL_CreateWindow(
        this->window_title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        this->window_width,
        this->window_height,
        SDL_WINDOW_RESIZABLE
    );

    if (!(this->window)) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    this->surface = SDL_GetWindowSurface(window);

    SDL_Surface* icon = SDL_LoadBMP("images/Icon_64.bmp");

    if (!icon) {
        fprintf(stderr, "Error loading icon: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    } else {
        SDL_SetWindowIcon(this->window, icon);
        SDL_FreeSurface(icon);
    }
}


///////////////////////////////////////////////////////////////////////////////


/**
 * \brief Implementation of the main application destructor.
 */
App::~App() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}


///////////////////////////////////////////////////////////////////////////////


/**
 * \brief
 *
 * \return void
 */
void App::run() {
    this->running = true;
    bool rodou = false;
    while (running) {
        clear_screen(255, 255, 255);

        Uint32 red = Utils::rgb_to_uint32(surface, 255, 0, 0);
        Uint32 green = Utils::rgb_to_uint32(surface, 0, 255, 0);
        Uint32 blue = Utils::rgb_to_uint32(surface, 0, 0, 255);
        Uint32 black = Utils::rgb_to_uint32(surface, 0, 0, 0);

        Primitives::draw_line(this->surface, 0, 0, 500, 500, blue, true);
        Primitives::draw_line(this->surface, 200, 100, 450, 550, blue, false);
        Primitives::draw_curve(this->surface, 20, 10, 250, 200, 190, 350, 240, 420, red, true);
        //Primitives::draw_circle(this->surface, 300, 300, 100, black, true, false);
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

        this->handle_events();
        this->update_screen();
    }
}


///////////////////////////////////////////////////////////////////////////////


void App::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            surface = SDL_GetWindowSurface(window);
            this->window_width = surface->w;
            this->window_height = surface->h;
            //std::cout << "Size changed: " << width << ", " << height << "\n";
        }

    }
}


///////////////////////////////////////////////////////////////////////////////


void App::clear_screen(Uint8 r, Uint8 g, Uint8 b) {
    Uint32 color = SDL_MapRGB(surface->format, r, g, b);
    SDL_FillRect(surface, NULL, color);
}


///////////////////////////////////////////////////////////////////////////////

void App::update_screen() {
    SDL_UpdateWindowSurface(window);
}


///////////////////////////////////////////////////////////////////////////////


SDL_Surface* App::getSurface() {
    return this->surface;
}


///////////////////////////////////////////////////////////////////////////////


int App::getWidth() const {
    return this->window_width;
}


///////////////////////////////////////////////////////////////////////////////


int App::getHeight() const {
    return this->window_height;
}
