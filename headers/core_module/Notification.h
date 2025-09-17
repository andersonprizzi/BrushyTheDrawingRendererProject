#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "App.h"

/**
 * @brief Represents a single notification data container.
 *
 * This class only stores data; all logic
 * is handled by NotificationManager.
 */
class Notification {
    public:
        std::string title;
        std::string message;
        Uint32 duration_ms;
        Uint32 start_time;
        bool closed;
        SDL_Rect rect;
        SDL_Rect close_button;
        float pos_y;           // Current Y position (for animations)
        float alpha;           // Transparency (0–255)
        bool appearing;        // If notification is currently appearing
        bool disappearing;     // If notification is currently disappearing

    public:
        Notification()
            : duration_ms(3000),
              start_time(0),
              closed(false),
              rect{0, 0, 300, 80},
              close_button{0, 0, 20, 20},
              pos_y(0.0f),
              alpha(0.0f),
              appearing(false),
              disappearing(false) {}

        Notification(const std::string& t, const std::string& m, const SDL_Rect& r, Uint32 duration = 3000)
        : title(t),
          message(m),
          duration_ms(duration),
          start_time(0),
          closed(false),
          rect(r),
          close_button{r.w - 23, 8, 15, 15}, // Calcula automaticamente baseado no rect.
          pos_y(0.0f),
          alpha(0.0f),
          appearing(false),
          disappearing(false) {}
};

#endif
