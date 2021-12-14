#include "Chess.h"

void Chess::loadImages() {

	for(uint8_t i = 0; i < 8; i++)
		for(uint8_t j = 0; j < 8; j++)
			if(board[i][j] != nullptr)
				switch(board[i][j]->type) {
					case PAWN:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("img/pawnW.bmp");
					else
						board[i][j]->image = load_texture("img/pawnB.bmp");
					break;
					case BISHOP:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("img/bishopW.bmp");
					else
						board[i][j]->image = load_texture("img/bishopB.bmp");
					break;
					case KNIGHT:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("img/knightW.bmp");
					else
						board[i][j]->image = load_texture("img/knightB.bmp");
					break;
					case ROOK:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("img/rookW.bmp");
					else
						board[i][j]->image = load_texture("img/rookB.bmp");
					break;
					case QUEEN:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("img/queenW.bmp");
					else
						board[i][j]->image = load_texture("img/queenB.bmp");
					break;
					case KING:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("img/kingW.bmp");
					else
						board[i][j]->image = load_texture("img/kingB.bmp");
					break;
				}

}

void Chess::align() {

	

}

SDL_Texture *Chess::load_texture(char const *path) {
    return IMG_LoadTexture(renderer, path);
}
