#pragma once
#include "../SDL2-2.0.18/include/SDL.h"
#include "../SDL2_image-2.0.5/include/SDL_image.h"
#include "../SDL2_ttf-2.0.15/include/SDL_ttf.h"

class Chess {

public:
    
    Chess() {
        SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

        loadCharacters();
        loadImages();
        createBoard();
        drawBoard();
    }

    void run() {
        SDL_Event event;
        do {

            SDL_WaitEvent(&event);
            switch(event.type) {
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        destroyTextures();
                        createBoard();
                        drawBoard();
                    }
                break;
            }
        } while(event.type != SDL_QUIT);
    }

    ~Chess() {
        SDL_Quit();
    }
    
private:

    void loadCharacters();
    void loadImages();
    void destroyTextures();
    void createBoard();
    void drawBoard();
    SDL_Texture *load_texture(char const *path);
    
    SDL_Window *window{};
    SDL_Renderer *renderer{};

    enum PieceType {
        PAWN,
        BISHOP,
        KNIGHT,
        ROOK,
        QUEEN,
        KING
    };

    enum PieceColor {
        B, W
    };

    struct Piece {
        PieceType type;
        PieceColor color;

        SDL_Texture *image{};
        SDL_Rect position{};

        Piece(PieceType type = PAWN, PieceColor color = W) {
            this->type = type;
            this->color = color;
        }
    };

    SDL_Texture *charTexture[95];
    SDL_Texture *boardTexture{};

    Piece *board[8][8] = {

        {new Piece(ROOK, W),new Piece(KNIGHT, W),new Piece(BISHOP, W),new Piece(QUEEN, W),new Piece(KING, W),new Piece(BISHOP, W),new Piece(KNIGHT, W),new Piece(ROOK, W)},
        {new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W)},
        {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
        {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
        {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
        {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
        {new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B)},
        {new Piece(ROOK, B),new Piece(KNIGHT, B),new Piece(BISHOP, B),new Piece(QUEEN, B),new Piece(KING, B),new Piece(BISHOP, B),new Piece(KNIGHT, B),new Piece(ROOK, B)}

    };

};