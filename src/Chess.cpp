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

void Chess::drawBoard() {

	int w, h, sqSize, leftPadd = 60, topPadd = 60, rightPadd = 60, bottomPadd = 60;
	bool white = 0;
	SDL_Rect square;

	SDL_GetWindowSize(window, &w, &h);

	if(w - leftPadd - rightPadd > h - topPadd - bottomPadd) leftPadd = rightPadd = (w - (h - topPadd - bottomPadd)) / 2;
	else topPadd = bottomPadd = (h - (w - leftPadd - rightPadd)) / 2;

	if(w - leftPadd - rightPadd > h - topPadd - bottomPadd) sqSize = (h - topPadd - bottomPadd) / 8;
	else sqSize = (w - leftPadd - rightPadd) / 8;

	for(uint8_t j = 0; j < 8; j++) {
		white = !white;
		for(uint8_t i = 0; i < 8; i++) {
			square = {
				leftPadd + sqSize * i,
				topPadd + sqSize * j,
				sqSize,
				sqSize
			};
			
			if(white) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
				white = 0;
			} else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
				white = 1;
			}

			SDL_RenderFillRect(renderer, &square);
		}
	}
	SDL_RenderPresent(renderer);
}

SDL_Texture *Chess::load_texture(char const *path) {
    return IMG_LoadTexture(renderer, path);
}
