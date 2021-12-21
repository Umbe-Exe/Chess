#include "Chess.h"

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

void Chess::createBoard() {

	int w, h, leftPadd, topPadd, rightPadd, bottomPadd;
	bool white = 0;
	SDL_Rect square, lineSec;

	SDL_GetWindowSize(window, &w, &h);

	leftPadd = rightPadd = w * 10 / 100;
	topPadd = bottomPadd = h * 10 / 100;

	if(w - leftPadd - rightPadd > h - topPadd - bottomPadd) leftPadd = rightPadd = (w - (h - topPadd - bottomPadd)) / 2;
	else topPadd = bottomPadd = (h - (w - leftPadd - rightPadd)) / 2;

	if(w - leftPadd - rightPadd > h - topPadd - bottomPadd) sqSize = (h - topPadd - bottomPadd) / 8;
	else sqSize = (w - leftPadd - rightPadd) / 8;

	top = topPadd;
	bottom = top + sqSize * 8;
	left = leftPadd;
	right = left + sqSize * 8;

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

			if(board[j][i]) {
				board[j][i]->position = square;
				SDL_RenderCopy(renderer, board[j][i]->image, 0, &square);
			}
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

void Chess::movePiece(int32_t x, int32_t y) {

	Piece *movingPiece = nullptr;
	uint8_t 
		i = (y - top) / sqSize, 
		j = (x - left) / sqSize;
	SDL_Rect pos;

	if(x > left && x < right && y > top && y < bottom)
		if(movingPiece = board[i][j]) {
			pos = movingPiece->position;
			board[i][j] = nullptr;

			SDL_SetRenderTarget(renderer, boardTexture);

			if((i + j) % 2) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
			else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

			SDL_RenderFillRect(renderer, &movingPiece->position);
			SDL_SetRenderTarget(renderer, 0);

			SDL_Event event;
			do {
				SDL_WaitEvent(&event);

				if(event.type == SDL_MOUSEMOTION) {

					movingPiece->position.x += event.motion.xrel;
					movingPiece->position.y += event.motion.yrel;

					SDL_RenderCopy(renderer, boardTexture, 0, 0);
					SDL_RenderCopy(renderer, movingPiece->image, 0, &movingPiece->position);
					SDL_RenderPresent(renderer);
				}
			} while(event.type != SDL_MOUSEBUTTONUP && event.window.event != SDL_WINDOWEVENT_LEAVE);

			SDL_SetRenderTarget(renderer, boardTexture);

			movingPiece->position = align(movingPiece->position);

			Piece *capturedPiece = nullptr;
			uint8_t
				ic = (movingPiece->position.y - top) / sqSize,
				jc = (movingPiece->position.x - left) / sqSize;

			if(movingPiece->position.x != INT_MAX && logical({i, j}, {ic, jc}, movingPiece->type, movingPiece->color)) {
				if(capturedPiece = board[ic][jc]) {

					if((ic + jc) % 2) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
					else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

					SDL_RenderFillRect(renderer, &capturedPiece->position);

					delete capturedPiece;
				}
				SDL_RenderCopy(renderer, movingPiece->image, 0, &movingPiece->position);

				board[ic][jc] = movingPiece;
			} else {
				SDL_RenderCopy(renderer, movingPiece->image, 0, &pos);

				movingPiece->position = pos;
				board[i][j] = movingPiece;
			}

			SDL_SetRenderTarget(renderer, 0);
			SDL_RenderCopy(renderer, boardTexture, 0, 0);
			SDL_RenderPresent(renderer);
		}
}

SDL_Rect Chess::align(SDL_Rect where) {
	uint16_t
		centerX = where.x + where.w / 2,
		centerY = where.y + where.h / 2;

	if(centerX > left && centerX < right && centerY > top && centerY < bottom) {
		where.x = centerX - ((centerX - left) % where.w);
		where.y = centerY - ((centerY - top) % where.h);
	} else where.x = INT_MAX;

	return where;
}

bool Chess::logical(Location past, Location present, PieceType type, PieceColor color) {

	if(board[present.row][present.col]) if(board[present.row][present.col]->color == color) return false;
	if(turn != color) return false;
	if(type != KING) if(inCheck(color, present)) return false;

	if(type == PAWN) {
		bool enPassant = 0;
		Move lastMove = moveLog[moveLog.size() - 1];
		if(being == color) {
			if(past.row == 3) {
				if(lastMove.type == PAWN && lastMove.after.row == 3)
					if(lastMove.after.col == past.col + 1) {
						if(past.row - 1 == present.row && present.col == past.col + 1) enPassant = 1;
					} else if(lastMove.after.col == past.col - 1) {
						if(past.row - 1 == present.row && present.col == past.col - 1) enPassant = 1;
					}
			}
		} else {
			if(past.row == 4) {
				if(lastMove.type == PAWN && lastMove.after.row == 4)
					if(lastMove.after.col == past.col + 1) {
						if(past.row + 1 == present.row && present.col == past.col + 1) enPassant = 1;
					} else if(lastMove.after.col == past.col - 1) {
						if(past.row + 1 == present.row && present.col == past.col - 1) enPassant = 1;
					}
			}
		}
		if(enPassant) {
			Piece *capturedPiece = board[lastMove.after.row][lastMove.after.col];
			board[lastMove.after.row][lastMove.after.col] = 0;
			if(!inCheck((color == W ? whiteKing : blackKing), color)) {
				if((lastMove.after.row + lastMove.after.col) % 2) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
				else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

				SDL_RenderFillRect(renderer, &capturedPiece->position);

				if(capturedPiece->color == W) whiteCaptured.push_back(PAWN);
				else blackCaptured.push_back(PAWN);

				delete capturedPiece;
				moveLog.push_back({past, present, type});
				turn = (PieceColor)!turn;
				return true;
			} //extremely rare case
			board[lastMove.after.row][lastMove.after.col] = capturedPiece;
			return false;
		}
	} else if(type == KING) {
		bool castleRight = 0, castleLeft = 0;
		if(color == W) {
			if(leftCastleWhite) {
				if(being == color) {
					if(!(board[whiteKing.row][whiteKing.col - 1] || board[whiteKing.row][whiteKing.col - 2] || board[whiteKing.row][whiteKing.col - 3] ||
						 inCheck({whiteKing.row,whiteKing.col}, color) || inCheck({whiteKing.row,whiteKing.col - 1}, color) || inCheck({whiteKing.row,whiteKing.col - 2}, color))) {
						if(whiteKing.row == present.row && whiteKing.col - 2 == present.col) castleLeft = 1;
					}
				} else {
					if(!(board[whiteKing.row][whiteKing.col + 1] || board[whiteKing.row][whiteKing.col + 2] || board[whiteKing.row][whiteKing.col + 3] ||
						 inCheck({whiteKing.row,whiteKing.col}, color) || inCheck({whiteKing.row,whiteKing.col + 1}, color) || inCheck({whiteKing.row,whiteKing.col + 2}, color))) {
						if(whiteKing.row == present.row && whiteKing.col + 2 == present.col) castleRight = 1;
					}
				}
			}
			if(rightCastleWhite) {
				if(being == color) {
					if(!(board[whiteKing.row][whiteKing.col + 1] || board[whiteKing.row][whiteKing.col + 2] ||
						 inCheck({whiteKing.row,whiteKing.col}, color) || inCheck({whiteKing.row,whiteKing.col + 1}, color) || inCheck({whiteKing.row,whiteKing.col + 2}, color))) {
						if(whiteKing.row == present.row && whiteKing.col + 2 == present.col) castleRight = 1;
					}
				} else {
					if(!(board[whiteKing.row][whiteKing.col - 1] || board[whiteKing.row][whiteKing.col - 2] ||
						 inCheck({whiteKing.row,whiteKing.col}, color) || inCheck({whiteKing.row,whiteKing.col - 1}, color) || inCheck({whiteKing.row,whiteKing.col - 2}, color))) {
						if(whiteKing.row == present.row && whiteKing.col - 2 == present.col) castleLeft = 1;
					}
				}
			}
		} else {
			if(leftCastleBlack) {
				if(being == color) {
					if(!(board[whiteKing.row][whiteKing.col - 1] || board[whiteKing.row][whiteKing.col - 2] ||
						 inCheck({whiteKing.row,whiteKing.col}, color) || inCheck({whiteKing.row,whiteKing.col - 1}, color) || inCheck({whiteKing.row,whiteKing.col - 2}, color))) {
						if(whiteKing.row == present.row && whiteKing.col - 2 == present.col) castleLeft = 1;
					}
				} else {
					if(!(board[whiteKing.row][whiteKing.col + 1] || board[whiteKing.row][whiteKing.col + 2] ||
						 inCheck({whiteKing.row,whiteKing.col}, color) || inCheck({whiteKing.row,whiteKing.col + 1}, color) || inCheck({whiteKing.row,whiteKing.col + 2}, color))) {
						if(whiteKing.row == present.row && whiteKing.col + 2 == present.col) castleRight = 1;
					}
				}
			}
			if(rightCastleBlack) {
				if(being == color) {
					if(!(board[whiteKing.row][whiteKing.col + 1] || board[whiteKing.row][whiteKing.col + 2] || board[whiteKing.row][whiteKing.col + 3] ||
						 inCheck({whiteKing.row,whiteKing.col}, color) || inCheck({whiteKing.row,whiteKing.col + 1}, color) || inCheck({whiteKing.row,whiteKing.col + 2}, color))) {
						if(whiteKing.row == present.row && whiteKing.col + 2 == present.col) castleRight = 1;
					}
				} else {
					if(!(board[whiteKing.row][whiteKing.col - 1] || board[whiteKing.row][whiteKing.col - 2] || board[whiteKing.row][whiteKing.col - 3] ||
						 inCheck({whiteKing.row,whiteKing.col}, color) || inCheck({whiteKing.row,whiteKing.col - 1}, color) || inCheck({whiteKing.row,whiteKing.col - 2}, color))) {
						if(whiteKing.row == present.row && whiteKing.col - 2 == present.col) castleLeft = 1;
					}
				}
			}
		}
		if(castleRight || castleLeft) {
			if(color == W) {
				leftCastleWhite = 0;
				rightCastleWhite = 0;
			} else {
				leftCastleBlack = 0;
				rightCastleBlack = 0;
			}

			if(castleLeft) {
				if(past.row == 7) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
				else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
				SDL_RenderFillRect(renderer, &board[present.row][0]->position);

				board[present.row][0]->position.x = board[present.row][present.col]->position.x + sqSize;
				SDL_RenderCopy(renderer, board[present.row][0]->image, 0, &board[present.row][0]->position);

				board[present.row][present.col + 1] = board[present.row][0];
				board[present.row][0] = 0;		
			} else {
				if(past.row == 0) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
				else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
				SDL_RenderFillRect(renderer, &board[present.row][7]->position);

				board[present.row][7]->position.x = board[present.row][present.col]->position.x - sqSize;
				SDL_RenderCopy(renderer, board[present.row][7]->image, 0, &board[present.row][7]->position);

				board[present.row][present.col - 1] = board[present.row][7];
				board[present.row][0] = 0;
			}

			moveLog.push_back({past, present, type});
			turn = (PieceColor)!turn;
			return true;
		}
	}

	std::vector<Location> options = getOptions(past, type, color);

	for(auto &i : options)
		if(i.row == present.row && i.col == present.col) {

			if(type == ROOK) {
				if(rightCastleWhite && color == W && being == color && past.col == 7) rightCastleWhite = 0;
				if(rightCastleBlack && color == B && being == color && past.col == 7) rightCastleBlack = 0;

				if(leftCastleWhite && color == W && being == color && past.col == 0) leftCastleWhite = 0;
				if(leftCastleBlack && color == B && being == color && past.col == 0) leftCastleBlack = 0;

				if(rightCastleWhite && color == W && being != color && past.col == 0) rightCastleWhite = 0;
				if(rightCastleBlack && color == B && being != color && past.col == 0) rightCastleBlack = 0;

				if(leftCastleWhite && color == W && being != color && past.col == 7) leftCastleWhite = 0;
				if(leftCastleBlack && color == B && being != color && past.col == 7) leftCastleBlack = 0;
			} else if(type == KING) {
				if(color == W) {
					rightCastleWhite = 0;
					leftCastleWhite = 0;
				} else {
					rightCastleBlack = 0;
					leftCastleBlack = 0;
				}
			}

			if(board[present.row][present.col]) {
				if(board[present.row][present.col]->color == W) whiteCaptured.push_back(board[present.row][present.col]->type);
				else blackCaptured.push_back(board[present.row][present.col]->type);
			}
			moveLog.push_back({past, present, type});
			turn = (PieceColor)!turn;
			return true;
		}

	return false;
}

SDL_Texture *Chess::load_texture(char const *path) {
    return IMG_LoadTexture(renderer, path);
}

std::vector<Chess::Location> Chess::getOptions(Location past, PieceType type, PieceColor color) {
	std::vector<Location> loc;

	switch(type) {
		case PAWN:
			if(being == color) {
				if(past.row == 6 && !board[4][past.col]) loc.push_back({4,past.col});
				if(!board[past.row - 1][past.col]) loc.push_back({past.row - 1,past.col});
				if(board[past.row - 1][past.col - 1]) loc.push_back({past.row - 1,past.col - 1});
				if(board[past.row - 1][past.col + 1]) loc.push_back({past.row - 1,past.col + 1});
			} else {
				if(past.row == 1 && !board[3][past.col]) loc.push_back({3,past.col});
				if(!board[past.row + 1][past.col]) loc.push_back({past.row + 1,past.col});
				if(board[past.row + 1][past.col - 1]) loc.push_back({past.row + 1,past.col - 1});
				if(board[past.row + 1][past.col + 1]) loc.push_back({past.row + 1,past.col + 1});
			}
			break;
		case KNIGHT:
			loc = {
					{past.row + 1, past.col - 2},
					{past.row + 2, past.col - 1},
					{past.row + 2, past.col + 1},
					{past.row + 1, past.col + 2},
					{past.row - 1, past.col - 2},
					{past.row - 2, past.col - 1},
					{past.row - 2, past.col + 1},
					{past.row - 1, past.col + 2}};
			break;
		case BISHOP:
			for(uint8_t i = 1; i <= (past.row < past.col ? past.row : past.col); i++) {
				if(board[past.row - i][past.col - i]) {
					if(board[past.row - i][past.col - i]->color == color) break;
					else {
						loc.push_back({past.row - i, past.col - i});
						break;
					}
				}
				loc.push_back({past.row - i, past.col - i});
			}
			for(uint8_t i = 1; i <= (7 - past.row < 7 - past.col ? 7 - past.row : 7 - past.col); i++) {
				if(board[past.row + i][past.col + i]) {
					if(board[past.row + i][past.col + i]->color == color) break;
					else {
						loc.push_back({past.row + i, past.col + i});
						break;
					}
				}
				loc.push_back({past.row + i, past.col + i});
			}
			for(uint8_t i = 1; i <= (7 - past.row < past.col ? 7 - past.row : past.col); i++) {
				if(board[past.row + i][past.col - i]) {
					if(board[past.row + i][past.col - i]->color == color) break;
					else {
						loc.push_back({past.row + i, past.col - i});
						break;
					}
				}
				loc.push_back({past.row + i, past.col - i});
			}
			for(uint8_t i = 1; i <= (past.row < 7 - past.col ? past.row : 7 - past.col); i++) {
				if(board[past.row - i][past.col + i]) {
					if(board[past.row - i][past.col + i]->color == color) break;
					else {
						loc.push_back({past.row - i, past.col + i});
						break;
					}
				}
				loc.push_back({past.row - i, past.col + i});
			}
			break;
		case QUEEN:
			for(uint8_t i = 1; i <= (past.row < past.col ? past.row : past.col); i++) {
				if(board[past.row - i][past.col - i]) {
					if(board[past.row - i][past.col - i]->color == color) break;
					else {
						loc.push_back({past.row - i, past.col - i});
						break;
					}
				}
				loc.push_back({past.row - i, past.col - i});
			}
			for(uint8_t i = 1; i <= (7 - past.row < 7 - past.col ? 7 - past.row : 7 - past.col); i++) {
				if(board[past.row + i][past.col + i]) {
					if(board[past.row + i][past.col + i]->color == color) break;
					else {
						loc.push_back({past.row + i, past.col + i});
						break;
					}
				}
				loc.push_back({past.row + i, past.col + i});
			}
			for(uint8_t i = 1; i <= (7 - past.row < past.col ? 7 - past.row : past.col); i++) {
				if(board[past.row + i][past.col - i]) {
					if(board[past.row + i][past.col - i]->color == color) break;
					else {
						loc.push_back({past.row + i, past.col - i});
						break;
					}
				}
				loc.push_back({past.row + i, past.col - i});
			}
			for(uint8_t i = 1; i <= (past.row < 7 - past.col ? past.row : 7 - past.col); i++) {
				if(board[past.row - i][past.col + i]) {
					if(board[past.row - i][past.col + i]->color == color) break;
					else {
						loc.push_back({past.row - i, past.col + i});
						break;
					}
				}
				loc.push_back({past.row - i, past.col + i});
			}
			for(uint8_t i = 1; i <= past.row; i++) {
				if(board[past.row - i][past.col]) {
					if(board[past.row - i][past.col]->color == color) break;
					else {
						loc.push_back({past.row - i, past.col});
						break;
					}
				}
				loc.push_back({past.row - i, past.col});
			}
			for(uint8_t i = 1; i <= 7 - past.row; i++) {
				if(board[past.row + i][past.col]) {
					if(board[past.row + i][past.col]->color == color) break;
					else {
						loc.push_back({past.row + i, past.col});
						break;
					}
				}
				loc.push_back({past.row + i, past.col});
			}
			for(uint8_t i = 1; i <= past.col; i++) {
				if(board[past.row][past.col - i]) {
					if(board[past.row][past.col - i]->color == color) break;
					else {
						loc.push_back({past.row, past.col - i});
						break;
					}
				}
				loc.push_back({past.row, past.col - i});
			}
			for(uint8_t i = 1; i <= 7 - past.col; i++) {
				if(board[past.row][past.col + i]) {
					if(board[past.row][past.col + i]->color == color) break;
					else {
						loc.push_back({past.row, past.col + i});
						break;
					}
				}
				loc.push_back({past.row, past.col + i});
			}
			break;
		case ROOK:
			for(uint8_t i = 1; i <= past.row; i++) {
				if(board[past.row - i][past.col]) {
					if(board[past.row - i][past.col]->color == color) break;
					else {
						loc.push_back({past.row - i, past.col});
						break;
					}
				}
				loc.push_back({past.row - i, past.col});
			}
			for(uint8_t i = 1; i <= 7 - past.row; i++) {
				if(board[past.row + i][past.col]) {
					if(board[past.row + i][past.col]->color == color) break;
					else {
						loc.push_back({past.row + i, past.col});
						break;
					}
				}
				loc.push_back({past.row + i, past.col});
			}
			for(uint8_t i = 1; i <= past.col; i++) {
				if(board[past.row][past.col - i]) {
					if(board[past.row][past.col - i]->color == color) break;
					else {
						loc.push_back({past.row, past.col - i});
						break;
					}
				}
				loc.push_back({past.row, past.col - i});
			}
			for(uint8_t i = 1; i <= 7 - past.col; i++) {
				if(board[past.row][past.col + i]) {
					if(board[past.row][past.col + i]->color == color) break;
					else {
						loc.push_back({past.row, past.col + i});
						break;
					}
				}
				loc.push_back({past.row, past.col + i});
			}
			break;
		case KING:
			if(board[past.row + 1][past.col]) {
				if(board[past.row + 1][past.col]->color != color)
					if(!inCheck({past.row + 1,past.col}, color))
						loc.push_back({past.row + 1, past.col});
			} else if(!inCheck({past.row + 1,past.col}, color))
				loc.push_back({past.row + 1, past.col});

			if(board[past.row - 1][past.col]) {
				if(board[past.row - 1][past.col]->color != color)
					if(!inCheck({past.row - 1,past.col}, color))
						loc.push_back({past.row - 1, past.col});
			} else if(!inCheck({past.row - 1,past.col}, color))
				loc.push_back({past.row - 1, past.col});

			if(board[past.row + 1][past.col + 1]) {
				if(board[past.row + 1][past.col + 1]->color != color)
					if(!inCheck({past.row + 1,past.col + 1}, color))
						loc.push_back({past.row + 1, past.col + 1});
			} else if(!inCheck({past.row + 1,past.col + 1}, color))
				loc.push_back({past.row + 1, past.col + 1});

			if(board[past.row + 1][past.col - 1]) {
				if(board[past.row + 1][past.col - 1]->color != color)
					if(!inCheck({past.row + 1,past.col - 1}, color))
						loc.push_back({past.row + 1, past.col - 1});
			} else if(!inCheck({past.row + 1,past.col - 1}, color))
				loc.push_back({past.row + 1, past.col - 1});

			if(board[past.row - 1][past.col + 1]) {
				if(board[past.row - 1][past.col + 1]->color != color)
					if(!inCheck({past.row - 1,past.col + 1}, color))
						loc.push_back({past.row + 1, past.col + 1});
			} else if(!inCheck({past.row - 1,past.col + 1}, color))
				loc.push_back({past.row - 1, past.col + 1});

			if(board[past.row - 1][past.col - 1]) {
				if(board[past.row - 1][past.col - 1]->color != color)
					if(!inCheck({past.row - 1,past.col - 1}, color))
						loc.push_back({past.row - 1, past.col - 1});
			} else if(!inCheck({past.row - 1,past.col - 1}, color))
				loc.push_back({past.row - 1, past.col - 1});

			if(board[past.row][past.col + 1]) {
				if(board[past.row][past.col + 1]->color != color)
					if(!inCheck({past.row,past.col + 1}, color))
						loc.push_back({past.row, past.col + 1});
			} else if(!inCheck({past.row,past.col + 1}, color))
				loc.push_back({past.row, past.col + 1});

			if(board[past.row][past.col - 1]) {
				if(board[past.row][past.col - 1]->color != color)
					if(!inCheck({past.row,past.col - 1}, color))
						loc.push_back({past.row, past.col - 1});
			} else if(!inCheck({past.row,past.col - 1}, color))
				loc.push_back({past.row, past.col - 1});
			break;
	}
	return loc;
}

bool Chess::inCheck(PieceColor color, Location friendly) {
	return 0;
}

bool Chess::inCheck(Location pos, PieceColor color) {
	return 0;
}
