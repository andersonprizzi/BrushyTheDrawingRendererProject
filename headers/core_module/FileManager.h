#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <SDL.h>

// Forward declarations.
class Shape;

class FileManager {
    public:
        static bool load_scene(
            const std::string& file_path,
            SDL_Surface* target_surface,
            std::vector<std::unique_ptr<Shape>>& shapes,
            int* out_width,
            int* out_height,
            int* out_universe_w,
            int* out_universe_h,
            Uint32* out_bg_color
        );
};

#endif
