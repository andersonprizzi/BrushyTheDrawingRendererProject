#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "App.h"

class FontManager {
    public:
        static TTF_Font* libertinus_regular_36;
        static TTF_Font* roboto_regular_26;
        static TTF_Font* roboto_regular_15;
        static TTF_Font* roboto_semibold_26;
        static TTF_Font* roboto_semibold_20;
        static TTF_Font* roboto_semibold_16;

        static TTF_Font* default_notification_message_font;
        static TTF_Font* default_notification_title_font;
        static TTF_Font* default_primary_button_font;

        static bool load_fonts();
        static void close_fonts();
};

#endif
