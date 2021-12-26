#pragma once
#include "../SDL2-2.0.18/include/SDL.h"
#include "../SDL2_image-2.0.5/include/SDL_image.h"
#include "../SDL2_ttf-2.0.15/include/SDL_ttf.h"
#include <vector>
#include <string>

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
                        SDL_DestroyTexture(boardTexture);
                        createBoard();
                        drawBoard();
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    movePiece(event.button.x, event.button.y);
                    break;
            }
        } while(event.type != SDL_QUIT);
    }

    ~Chess() {
        SDL_DestroyTexture(boardTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        for(auto &i : charTexture) SDL_DestroyTexture(i);
        for(auto &i : pieceTexture) SDL_DestroyTexture(i);

        SDL_Quit();
    }
    
private:

    void loadCharacters();
    void loadImages();
    SDL_Texture *load_texture(char const *path);

    void createBoard();
    void drawBoard();

    void movePiece(int32_t x, int32_t y);

    uint8_t getRow(uint16_t y);
    uint8_t getCol(uint16_t x);
    SDL_Rect getRect(uint8_t row, uint8_t col);
    
    SDL_Window *window{};
    SDL_Renderer *renderer{};

    SDL_Texture *charTexture[95];
    SDL_Texture *pieceTexture[12];

    SDL_Texture *boardTexture{};
    uint16_t top, left, sqSize;

    enum PieceType {
        PAWN = 0,
        BISHOP = 2,
        KNIGHT = 4,
        ROOK = 6,
        QUEEN = 8,
        KING = 10,
        NONE = 12
    };

    enum PieceColor {
        B = 0, W
    };

    struct Location {
        int row, col;
    };

    bool logical(Location from, Location to);
    std::vector<Location> getOptions(Location from, PieceType type, PieceColor color);
    bool inCheck(Location loc, PieceColor color);

    bool longCastleRight(Location from, PieceColor color);
    bool longCastleLeft(Location from, PieceColor color);
    bool shortCastleRight(Location from, PieceColor color);
    bool shortCastleLeft(Location from, PieceColor color);

    struct Move {
        Location before, after;
        PieceType type;
    };

    std::vector<Move> moveLog{Move()};

    std::vector<uint8_t> whiteCaptured, blackCaptured;
    Location whiteKing{7,4}, blackKing{0,4};

    PieceColor turn = W;
    PieceColor being = W;

    bool leftCastleWhite = 1, leftCastleBlack = 1, rightCastleWhite = 1, rightCastleBlack = 1;

    uint8_t board[8][8] = {
        {ROOK + B,KNIGHT + B,BISHOP + B,QUEEN + B,KING + B,BISHOP + B,KNIGHT + B,ROOK + B},
        {PAWN + B,PAWN + B,PAWN + B,PAWN + B,PAWN + B,PAWN + B,PAWN + B,PAWN + B},
        {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
        {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
        {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
        {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
        {PAWN + W,PAWN + W,PAWN + W,PAWN + W,PAWN + W,PAWN + W,PAWN + W,PAWN + W},
        {ROOK + W,KNIGHT + W,BISHOP + W,QUEEN + W,KING + W,BISHOP + W,KNIGHT + W,ROOK + W}
    };

};