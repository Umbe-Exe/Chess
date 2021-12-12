#include <iostream>
#include <SDL.h>

int main(int argc, char *argv[]) {
    int gogogo=1;
    SDL_Event event;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *w = SDL_CreateWindow("hfd", 150, 150, 1280, 720, 0);

        while(gogogo) {
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
            gogogo=0;
    }
    SDL_Quit();
    return 0;
}