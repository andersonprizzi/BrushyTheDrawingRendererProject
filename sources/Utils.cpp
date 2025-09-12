#include "Utils.h"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

// Verifica se posição está dentro dos limites da tela
int Utils::verify_limits(SDL_Surface* surface, int x, int y) {
    if (surface->w < x || surface->h < y || x < 0 || y < 0){
        printf("ERROR: POINT OUT OF BOUNDS.\n");
        return 0;
    }

    return 1;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//



