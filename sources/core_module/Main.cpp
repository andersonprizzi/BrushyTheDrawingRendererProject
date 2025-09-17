// INCLUDES
#include "App.h"


// MAIN FUNCTION
int main(int argc, char* argv[]) {
    App *app = new App("Brushy: The Drawing Render", 0.7, 0.7);
    app->run();
    return 0;
}

