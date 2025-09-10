#include "App.h"

int main(int argc, char ** argv) {
    App *app = new App("Drawing Render", 1280, 600);
    app->run();
    return 0;
}
