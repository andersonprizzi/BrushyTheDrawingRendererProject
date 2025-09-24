#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations.
class Shape;
struct SDL_Surface;

class FileManager {
    public:
        static bool load_scene(const std::string& file_path, SDL_Surface* target_surface, std::vector<std::unique_ptr<Shape>>& shapes);
};

#endif
