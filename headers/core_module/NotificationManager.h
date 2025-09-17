#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "App.h"

struct Notification {
    std::string title;
    std::string message;

    Uint32 duration_ms = 3000;
    Uint32 start_time = 0;

    bool closed = false;

    SDL_Rect rect = {0, 0, 300, 80};
    SDL_Rect close_button = {0, 0, 20, 20};

    float pos_y = 0.0f;
    float alpha = 0.0f;
    bool appearing = false;
    bool disappearing = false;
};



class NotificationManager {
public:
    NotificationManager(int w, int h);
    ~NotificationManager();
    TTF_Font* font_title_;
    TTF_Font* font_message_;

    void push(const Notification& n);
    void update();
    void draw(SDL_Surface* surface);
    void handle_event(SDL_Event* e);
    void set_fonts(TTF_Font* title, TTF_Font* message);

private:
    std::queue<Notification> queue_;
    Notification* current_;

    int window_width_;
    int window_height_;

    void update_current();
    void draw_rounded_rect_surface(SDL_Surface* surface,
                                                    int x, int y, int w, int h,
                                                    SDL_Color color,
                                                    int radius = 10, bool filled = true);
    void draw_rect_with_alpha(SDL_Surface* target, int x, int y, int w, int h, SDL_Color color);
    void draw_notification(SDL_Surface* target, Notification* n);

};

#endif // NOTIFICATION_MANAGER_H
