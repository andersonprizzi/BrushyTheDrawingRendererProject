#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "App.h"

/**
 * @brief Manages a queue of notifications, handling their lifecycle,
 * animations, and rendering.
 */
class NotificationManager {
public:
    NotificationManager(int w, int h);
    ~NotificationManager();

    void push(const Notification& n);
    void update();
    void draw(SDL_Surface* surface);
    void handle_event(SDL_Event* e);
    void set_fonts(TTF_Font* title, TTF_Font* message);

private:
    std::queue<Notification> queue_;
    Notification current_;
    bool has_current_;

    int window_width_;
    int window_height_;

    TTF_Font* font_title_;
    TTF_Font* font_message_;

    void update_current();
    void draw_rounded_rect_surface(SDL_Surface* surface,int x, int y, int w, int h, SDL_Color color, int radius, bool filled);
    void draw_notification(SDL_Surface* target, const Notification& n);
};

#endif // NOTIFICATION_MANAGER_H
