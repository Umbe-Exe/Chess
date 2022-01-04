#pragma once
#include "../SDL2-2.0.18/include/SDL.h"
#include "../SDL2_ttf-2.0.15/include/SDL_ttf.h"
#include <string>

class LogWindow {

public:
    LogWindow() {
        window = SDL_CreateWindow("ChessLog", 0, SDL_WINDOWPOS_CENTERED, 300, 300, SDL_WINDOW_RESIZABLE | SDL_WINDOW_UTILITY);
        renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    bool inFocus() {
        return SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS;
    }

    void addLog(std::string &str) {
        log += str + " ";

        TTF_Init();
        TTF_Font *font = TTF_OpenFont("font/FreeMonoBold.ttf", 20);
        SDL_Surface *surf = TTF_RenderText_Shaded(font, log.c_str(), {0,0,0,255}, {100,100,100,255});
        SDL_Texture *tx = SDL_CreateTextureFromSurface(renderer, surf);

        SDL_Rect rect = {0,0,surf->w,surf->h};
        SDL_RenderCopy(renderer, tx, 0, &rect);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(tx);
        SDL_FreeSurface(surf);
        TTF_CloseFont(font);
    }

    ~LogWindow() {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
    }
private:
	SDL_Window *window{};
    SDL_Renderer *renderer;

    std::string log;
};