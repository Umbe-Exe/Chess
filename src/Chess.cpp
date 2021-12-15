#include "Chess.h"
#include <string>

void Chess::loadCharacters() {
	TTF_Init();

	TTF_Font *font;
	font = TTF_OpenFont("../../font/FreeMonoBold.ttf", 100);

	SDL_Color color = {0, 0, 0};
	for(uint8_t i = 0; i < 95; i++) {
		SDL_Surface *character = TTF_RenderText_Solid(font, (std::string("") += (i + 32)).c_str(), color);

		charTexture[i] = SDL_CreateTextureFromSurface(renderer, character);
		SDL_FreeSurface(character);
	}
	TTF_Quit();
}

void Chess::loadImages() {

	for(uint8_t i = 0; i < 8; i++)
		for(uint8_t j = 0; j < 8; j++)
			if(board[i][j] != nullptr)
				switch(board[i][j]->type) {
					case PAWN:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("../../img/pawnW.png");
					else
						board[i][j]->image = load_texture("../../img/pawnB.png");
					break;
					case BISHOP:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("../../img/bishopW.png");
					else
						board[i][j]->image = load_texture("../../img/bishopB.png");
					break;
					case KNIGHT:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("../../img/knightW.png");
					else
						board[i][j]->image = load_texture("../../img/knightB.png");
					break;
					case ROOK:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("../../img/rookW.png");
					else
						board[i][j]->image = load_texture("../../img/rookB.png");
					break;
					case QUEEN:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("../../img/queenW.png");
					else
						board[i][j]->image = load_texture("../../img/queenB.png");
					break;
					case KING:
					if(board[i][j]->color == W)
						board[i][j]->image = load_texture("../../img/kingW.png");
					else
						board[i][j]->image = load_texture("../../img/kingB.png");
					break;
				}

}

void Chess::destroyTextures() {

	SDL_DestroyTexture(boardTexture);

}

void Chess::createBoard() {

	int w, h, sqSize, leftPadd, topPadd, rightPadd, bottomPadd;
	bool white = 0;
	SDL_Rect square, lineSec;

	SDL_GetWindowSize(window, &w, &h);

	leftPadd = rightPadd = w * 10 / 100;
	topPadd = bottomPadd = h * 10 / 100;

	if(w - leftPadd - rightPadd > h - topPadd - bottomPadd) leftPadd = rightPadd = (w - (h - topPadd - bottomPadd)) / 2;
	else topPadd = bottomPadd = (h - (w - leftPadd - rightPadd)) / 2;

	if(w - leftPadd - rightPadd > h - topPadd - bottomPadd) sqSize = (h - topPadd - bottomPadd) / 8;
	else sqSize = (w - leftPadd - rightPadd) / 8;

	uint16_t lineWidth = sqSize * 50 / 100;

	boardTexture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetRenderTarget(renderer, boardTexture);

	SDL_Rect line[4];
	line[0] = {leftPadd - lineWidth,topPadd - lineWidth,sqSize * 8 + lineWidth * 2,lineWidth};
	line[1] = {leftPadd - lineWidth,topPadd - lineWidth,lineWidth,sqSize * 8 + lineWidth * 2};

	line[2] = {leftPadd + sqSize * 8 + lineWidth,topPadd + sqSize * 8 + lineWidth,-lineWidth,-(sqSize * 8 + lineWidth * 2)};
	line[3] = {leftPadd + sqSize * 8 + lineWidth,topPadd + sqSize * 8 + lineWidth,-(sqSize * 8 + lineWidth * 2),-lineWidth};

	SDL_SetRenderDrawColor(renderer, 127, 127, 127, 1);
	SDL_RenderFillRects(renderer, line, 4);

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

			if(board[j][i]) SDL_RenderCopy(renderer, board[j][i]->image, 0, &square);
		}
	}

	uint16_t charRadius = lineWidth * 30 / 100;

	for(uint8_t i = 0; i < 8; i++) {
		lineSec = {
			leftPadd + sqSize / 2 - charRadius + sqSize * i,
			topPadd + sqSize * 8 + lineWidth / 2 - charRadius,
			charRadius * 2,
			charRadius * 2
		};
		SDL_RenderCopy(renderer, charTexture[65 + i], 0, &lineSec);

		lineSec = {
			leftPadd + sqSize / 2 - charRadius + sqSize * i,
			topPadd - lineWidth / 2 - charRadius,
			charRadius * 2,
			charRadius * 2
		};
		SDL_RenderCopyEx(renderer, charTexture[65 + i], 0, &lineSec, 0, 0, SDL_FLIP_VERTICAL);

		lineSec = {
			leftPadd - lineWidth / 2 - charRadius,
			topPadd + sqSize / 2 - charRadius + sqSize * i,
			charRadius * 2,
			charRadius * 2
		};
		SDL_RenderCopy(renderer, charTexture[24 - i], 0, &lineSec);

		lineSec = {
			leftPadd + sqSize * 8 + lineWidth / 2 - charRadius,
			topPadd + sqSize / 2 - charRadius + sqSize * i,
			charRadius * 2,
			charRadius * 2
		};
		SDL_RenderCopyEx(renderer, charTexture[24 - i], 0, &lineSec, 0, 0, SDL_FLIP_VERTICAL);
	}

	SDL_SetRenderTarget(renderer, 0);
}

void Chess::drawBoard() {
	SDL_RenderCopy(renderer, boardTexture, 0, 0);
	SDL_RenderPresent(renderer);
}

SDL_Texture *Chess::load_texture(char const *path) {
    return IMG_LoadTexture(renderer, path);
}
