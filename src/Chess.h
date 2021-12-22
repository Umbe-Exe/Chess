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
        for(uint8_t i = 0; i < 8; i++)
            for(uint8_t j = 0; j < 8; j++)
                if(board[i][j]) delete board[i][j];

        SDL_DestroyTexture(boardTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        for(auto &i : charTexture) SDL_DestroyTexture(i);

        SDL_Quit();
    }
    
private:

    void loadCharacters();
    void loadImages();
    void createBoard();
    void drawBoard();
    void movePiece(int32_t x, int32_t y);
    SDL_Rect align(SDL_Rect where);
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

    struct Location {
        int row, col;
    };

    bool logical(Location past, Location present);
    std::vector<Location> getOptions(Location past, PieceType type, PieceColor color);
    bool inCheck(PieceColor color);
    bool inCheck(Location pos, PieceColor color);

    struct Piece {
        PieceType type;
        PieceColor color;

        SDL_Texture *image{};
        SDL_Rect position{};

        Piece(PieceType type, PieceColor color) {
            this->type = type;
            this->color = color;
        }
        ~Piece() {
            if(image) SDL_DestroyTexture(image);
        }
    };

    struct Move {
        Location before, after;
        PieceType type;
    };

    std::vector<Move> moveLog{Move()};

    std::vector<PieceType> whiteCaptured, blackCaptured;
    Location whiteKing{7,4}, blackKing{0,4};

    SDL_Texture *charTexture[95];
    SDL_Texture *boardTexture{};

    uint16_t top, bottom, left, right, sqSize;

    PieceColor turn = W;
    PieceColor being = W;

    bool leftCastleWhite = 1, leftCastleBlack = 1, rightCastleWhite = 1, rightCastleBlack = 1;

    Piece *board[8][8] = {

        {new Piece(ROOK, B),new Piece(KNIGHT, B),new Piece(BISHOP, B),new Piece(QUEEN, B),new Piece(KING, B),new Piece(BISHOP, B),new Piece(KNIGHT, B),new Piece(ROOK, B)},
        {new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B),new Piece(PAWN, B)},
        {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
        {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
        {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
        {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
        {new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W),new Piece(PAWN, W)},
        {new Piece(ROOK, W),new Piece(KNIGHT, W),new Piece(BISHOP, W),new Piece(QUEEN, W),new Piece(KING, W),new Piece(BISHOP, W),new Piece(KNIGHT, W),new Piece(ROOK, W)}

    };

};