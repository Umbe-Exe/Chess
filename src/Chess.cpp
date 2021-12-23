#include "Chess.h"

void Chess::loadCharacters() {
	TTF_Init();

	TTF_Font *font = TTF_OpenFont("../../font/FreeMonoBold.ttf", 100);

	char str[2] = {' ', '\0'};
	SDL_Color color = {0, 0, 0};
	for(uint8_t i = 0; i < 95; i++) {
		str[0] = i + 32;
		SDL_Surface *character = TTF_RenderText_Solid(font, str, color);

		charTexture[i] = SDL_CreateTextureFromSurface(renderer, character);
		SDL_FreeSurface(character);
	}
	TTF_Quit();
}

void Chess::loadImages() {
	pieceTexture[PAWN + W] = load_texture("../../img/pawnW.png");
	pieceTexture[PAWN + B] = load_texture("../../img/pawnB.png");
	pieceTexture[BISHOP + W] = load_texture("../../img/bishopW.png");
	pieceTexture[BISHOP + B] = load_texture("../../img/bishopB.png");
	pieceTexture[KNIGHT + W] = load_texture("../../img/knightW.png");
	pieceTexture[KNIGHT + B] = load_texture("../../img/knightB.png");
	pieceTexture[ROOK + W] = load_texture("../../img/rookW.png");
	pieceTexture[ROOK + B] = load_texture("../../img/rookB.png");
	pieceTexture[QUEEN + W] = load_texture("../../img/queenW.png");
	pieceTexture[QUEEN + B] = load_texture("../../img/queenB.png");
	pieceTexture[KING + W] = load_texture("../../img/kingW.png");
	pieceTexture[KING + B] = load_texture("../../img/kingB.png");
};

SDL_Texture *Chess::load_texture(char const *path) {
	return IMG_LoadTexture(renderer, path);
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
	left = leftPadd;

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

			if(board[j][i] != NONE) SDL_RenderCopy(renderer, pieceTexture[board[j][i]], 0, &square);
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

	if(x > left && x < left + sqSize * 8 && y > top && y < top + sqSize * 8) {
		uint8_t
			i = getRow(y),
			j = getCol(x);

		if(board[i][j] != NONE) {
			SDL_Rect rect = getRect(i, j);

			SDL_SetRenderTarget(renderer, boardTexture);

			if((i + j) % 2) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
			else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

			SDL_RenderFillRect(renderer, &rect);
			SDL_SetRenderTarget(renderer, 0);

			SDL_Event event;
			do {
				SDL_WaitEvent(&event);

				if(event.type == SDL_MOUSEMOTION) {

					rect.x += event.motion.xrel;
					rect.y += event.motion.yrel;

					SDL_RenderCopy(renderer, boardTexture, 0, 0);
					SDL_RenderCopy(renderer, pieceTexture[board[i][j]], 0, &rect);
					SDL_RenderPresent(renderer);
				}
			} while(event.type != SDL_MOUSEBUTTONUP && event.window.event != SDL_WINDOWEVENT_LEAVE);

			SDL_SetRenderTarget(renderer, boardTexture);

			if((rect.x + sqSize / 2) > left && (rect.x + sqSize / 2) < left + sqSize * 8 && (rect.y + sqSize / 2) > top && (rect.y + sqSize / 2) < top + sqSize * 8) {
				uint8_t
					ic = getRow(rect.y + sqSize / 2),
					jc = getCol(rect.x + sqSize / 2);
				rect = getRect(ic, jc);

				if(logical({i, j}, {ic, jc})) {
					if(board[ic][jc] != NONE) {

						if((ic + jc) % 2) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
						else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

						SDL_RenderFillRect(renderer, &rect);
					}
					SDL_RenderCopy(renderer, pieceTexture[board[i][j]], 0, &rect);
					board[ic][jc] = board[i][j];
					board[i][j] = NONE;
				} else {
					rect = getRect(i, j);
					SDL_RenderCopy(renderer, pieceTexture[board[i][j]], 0, &rect);
				}
			}

			SDL_SetRenderTarget(renderer, 0);
			SDL_RenderCopy(renderer, boardTexture, 0, 0);
			SDL_RenderPresent(renderer);
		}
	}
}

uint8_t Chess::getRow(uint16_t y) {
	return (y - top) / sqSize;
}

uint8_t Chess::getCol(uint16_t x) {
	return (x - left) / sqSize;
}

SDL_Rect Chess::getRect(uint8_t row, uint8_t col) {
	return {col * sqSize + left ,row * sqSize + top,sqSize,sqSize};
}

bool Chess::logical(Location from, Location to) {

	uint8_t 
		moving = board[from.row][from.col], 
		captured = board[to.row][to.col];
	PieceColor 
		movingColor = static_cast<PieceColor>(moving % 2),
		capturedColor = static_cast<PieceColor>(captured % 2);
	PieceType
		movingType = static_cast<PieceType>(moving - movingColor),
		capturedType = static_cast<PieceType>(captured - capturedColor);

	if(captured != NONE) if(movingColor == capturedColor) return false;
	if(turn != movingColor) return false;

	if(movingType != KING) {
		board[from.row][from.col] = NONE;
		board[to.row][to.col] = moving;
		if(inCheck((movingColor == W ? whiteKing : blackKing), movingColor)) {
			board[from.row][from.col] = moving;
			board[to.row][to.col] = captured;
			return false;
		}
		board[from.row][from.col] = moving;
		board[to.row][to.col] = captured;
	}

	if(movingType == PAWN) {
		bool enPassant = 0, promotion = 0;
		Move lastMove = moveLog[moveLog.size() - 1];
		if(being == movingColor) {
			if(from.row == 3) {
				if(lastMove.type == PAWN && lastMove.after.row == 3)
					if(lastMove.after.col == from.col + 1) {
						if(from.row - 1 == to.row && to.col == from.col + 1) enPassant = 1;
					} else if(lastMove.after.col == from.col - 1) {
						if(from.row - 1 == to.row && to.col == from.col - 1) enPassant = 1;
					}
			}
		} else {
			if(from.row == 4) {
				if(lastMove.type == PAWN && lastMove.after.row == 4)
					if(lastMove.after.col == from.col + 1) {
						if(from.row + 1 == to.row && to.col == from.col + 1) enPassant = 1;
					} else if(lastMove.after.col == from.col - 1) {
						if(from.row + 1 == to.row && to.col == from.col - 1) enPassant = 1;
					}
			}
		}
		if(enPassant) {
			captured = board[lastMove.after.row][lastMove.after.col];
			board[lastMove.after.row][lastMove.after.col] = 0;
			if(!inCheck((movingColor == W ? whiteKing : blackKing), movingColor)) {
				if((lastMove.after.row + lastMove.after.col) % 2) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
				else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

				SDL_Rect rect = getRect(lastMove.after.row, lastMove.after.col);
				SDL_RenderFillRect(renderer, &rect);

				if(captured % 2 == W) whiteCaptured.push_back(PAWN);
				else blackCaptured.push_back(PAWN);

				moveLog.push_back({from, to, movingType});
				turn = (PieceColor)!turn;
				return true;
			} //extremely rare case
			board[lastMove.after.row][lastMove.after.col] = captured;
			return false;
		}
	} else if(movingType == KING) {
		bool castleRight = 0, castleLeft = 0;
		if(movingColor == W) {
			if(leftCastleWhite) {
				if(being == movingColor) {
					if(board[whiteKing.row][whiteKing.col - 1] == NONE && board[whiteKing.row][whiteKing.col - 2] == NONE && board[whiteKing.row][whiteKing.col - 3] == NONE &&
					   !inCheck({whiteKing.row,whiteKing.col}, movingColor) && !inCheck({whiteKing.row,whiteKing.col - 1}, movingColor) && !inCheck({whiteKing.row,whiteKing.col - 2}, movingColor)) {
						if(whiteKing.row == to.row && whiteKing.col - 2 == to.col) castleLeft = 1;
					}
				} else {
					if(board[whiteKing.row][whiteKing.col + 1] == NONE && board[whiteKing.row][whiteKing.col + 2] == NONE && board[whiteKing.row][whiteKing.col + 3] == NONE &&
					   !inCheck({whiteKing.row,whiteKing.col}, movingColor) && !inCheck({whiteKing.row,whiteKing.col + 1}, movingColor) && !inCheck({whiteKing.row,whiteKing.col + 2}, movingColor)) {
						if(whiteKing.row == to.row && whiteKing.col + 2 == to.col) castleRight = 1;
					}
				}
			}
			if(rightCastleWhite) {
				if(being == movingColor) {
					if(board[whiteKing.row][whiteKing.col + 1] == NONE && board[whiteKing.row][whiteKing.col + 2] == NONE &&
						 !inCheck({whiteKing.row,whiteKing.col}, movingColor) && !inCheck({whiteKing.row,whiteKing.col + 1}, movingColor) && !inCheck({whiteKing.row,whiteKing.col + 2}, movingColor)) {
						if(whiteKing.row == to.row && whiteKing.col + 2 == to.col) castleRight = 1;
					}
				} else {
					if(board[whiteKing.row][whiteKing.col - 1] == NONE && board[whiteKing.row][whiteKing.col - 2] == NONE &&
					   !inCheck({whiteKing.row,whiteKing.col}, movingColor) && !inCheck({whiteKing.row,whiteKing.col - 1}, movingColor) && !inCheck({whiteKing.row,whiteKing.col - 2}, movingColor)) {
						if(whiteKing.row == to.row && whiteKing.col - 2 == to.col) castleLeft = 1;
					}
				}
			}
		} else {
			if(leftCastleBlack) {
				if(being == movingColor) {
					if(board[blackKing.row][blackKing.col - 1] == NONE && board[blackKing.row][blackKing.col - 2] == NONE &&
					   !inCheck({blackKing.row,blackKing.col}, movingColor) && !inCheck({blackKing.row,blackKing.col - 1}, movingColor) && !inCheck({blackKing.row,blackKing.col - 2}, movingColor)) {
						if(blackKing.row == to.row && blackKing.col - 2 == to.col) castleLeft = 1;
					}
				} else {
					if(board[blackKing.row][blackKing.col + 1] == NONE && board[blackKing.row][blackKing.col + 2] == NONE &&
						 !inCheck({blackKing.row,blackKing.col}, movingColor) && !inCheck({blackKing.row,blackKing.col + 1}, movingColor) && !inCheck({blackKing.row,blackKing.col + 2}, movingColor)) {
						if(blackKing.row == to.row && blackKing.col + 2 == to.col) castleRight = 1;
					}
				}
			}
			if(rightCastleBlack) {
				if(being == movingColor) {
					if(board[blackKing.row][blackKing.col + 1] == NONE && board[blackKing.row][blackKing.col + 2] == NONE && board[blackKing.row][blackKing.col + 3] == NONE &&
						 !inCheck({blackKing.row,blackKing.col}, movingColor) && !inCheck({blackKing.row,blackKing.col + 1}, movingColor) && !inCheck({blackKing.row,blackKing.col + 2}, movingColor)) {
						if(blackKing.row == to.row && blackKing.col + 2 == to.col) castleRight = 1;
					}
				} else {
					if(board[blackKing.row][blackKing.col - 1] == NONE && board[blackKing.row][blackKing.col - 2] == NONE && board[blackKing.row][blackKing.col - 3] == NONE &&
						 !inCheck({blackKing.row,blackKing.col}, movingColor) && !inCheck({blackKing.row,blackKing.col - 1}, movingColor) && !inCheck({blackKing.row,blackKing.col - 2}, movingColor)) {
						if(blackKing.row == to.row && blackKing.col - 2 == to.col) castleLeft = 1;
					}
				}
			}
		}
		if(castleRight || castleLeft) {
			if(movingColor == W) {
				leftCastleWhite = 0;
				rightCastleWhite = 0;
				whiteKing = to;
			} else {
				leftCastleBlack = 0;
				rightCastleBlack = 0;
				blackKing = to;
			}

			if(castleLeft) {
				if(to.row == 7) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
				else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

				SDL_Rect rect = getRect(to.row, 0);
				SDL_RenderFillRect(renderer, &rect);

				board[to.row][to.col + 1] = board[to.row][0];
				board[to.row][0] = NONE;

				rect = getRect(to.row, to.col + 1);
				SDL_RenderCopy(renderer, pieceTexture[board[to.row][to.col + 1]], 0, &rect);
			} else {
				if(to.row == 0) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
				else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
				
				SDL_Rect rect = getRect(to.row, 7);
				SDL_RenderFillRect(renderer, &rect);

				board[to.row][to.col - 1] = board[to.row][7];
				board[to.row][7] = NONE;

				rect = getRect(to.row, to.col - 1);
				SDL_RenderCopy(renderer, pieceTexture[board[to.row][to.col - 1]], 0, &rect);
			}

			moveLog.push_back({from, to, movingType});
			turn = (PieceColor)!turn;
			return true;
		}
	}

	std::vector<Location> options = getOptions(from, movingType, movingColor);

	for(auto &i : options)
		if(i.row == to.row && i.col == to.col) {

			if(movingType == ROOK) {
				if(rightCastleWhite && movingColor == W && being == movingColor && from.col == 7) rightCastleWhite = 0;
				if(rightCastleBlack && movingColor == B && being == movingColor && from.col == 7) rightCastleBlack = 0;

				if(leftCastleWhite && movingColor == W && being == movingColor && from.col == 0) leftCastleWhite = 0;
				if(leftCastleBlack && movingColor == B && being == movingColor && from.col == 0) leftCastleBlack = 0;

				if(rightCastleWhite && movingColor == W && being != movingColor && from.col == 0) rightCastleWhite = 0;
				if(rightCastleBlack && movingColor == B && being != movingColor && from.col == 0) rightCastleBlack = 0;

				if(leftCastleWhite && movingColor == W && being != movingColor && from.col == 7) leftCastleWhite = 0;
				if(leftCastleBlack && movingColor == B && being != movingColor && from.col == 7) leftCastleBlack = 0;
			} else if(movingType == KING) {
				if(movingColor == W) {
					if(rightCastleWhite) rightCastleWhite = 0;
					if(leftCastleWhite) leftCastleWhite = 0;
					whiteKing = to;
				} else {
					if(rightCastleBlack) rightCastleBlack = 0;
					if(leftCastleBlack) leftCastleBlack = 0;
					blackKing = to;
				}
			}

			if(captured != NONE) {
				if(capturedColor == W) whiteCaptured.push_back(capturedType);
				else blackCaptured.push_back(capturedType);
			}
			moveLog.push_back({from, to, movingType});
			turn = (PieceColor)!turn;
			return true;
		}

	return false;
}

std::vector<Chess::Location> Chess::getOptions(Location from, PieceType type, PieceColor color) {
	std::vector<Location> loc;

	switch(type) {
		case PAWN:
			if(being == color) {
				if(from.row == 6 && board[4][from.col] == NONE) loc.push_back({4,from.col});
				if(board[from.row - 1][from.col] == NONE) loc.push_back({from.row - 1,from.col});
				if(board[from.row - 1][from.col - 1] != NONE) loc.push_back({from.row - 1,from.col - 1});
				if(board[from.row - 1][from.col + 1] != NONE) loc.push_back({from.row - 1,from.col + 1});
			} else {
				if(from.row == 1 && board[3][from.col] == NONE) loc.push_back({3,from.col});
				if(board[from.row + 1][from.col] == NONE) loc.push_back({from.row + 1,from.col});
				if(board[from.row + 1][from.col - 1] != NONE) loc.push_back({from.row + 1,from.col - 1});
				if(board[from.row + 1][from.col + 1] != NONE) loc.push_back({from.row + 1,from.col + 1});
			}
			break;
		case KNIGHT:
			loc = {
					{from.row + 1, from.col - 2},
					{from.row + 2, from.col - 1},
					{from.row + 2, from.col + 1},
					{from.row + 1, from.col + 2},
					{from.row - 1, from.col - 2},
					{from.row - 2, from.col - 1},
					{from.row - 2, from.col + 1},
					{from.row - 1, from.col + 2}};
			break;
		case BISHOP:
			for(uint8_t i = 1; i <= (from.row < from.col ? from.row : from.col); i++) {
				if(board[from.row - i][from.col - i] != NONE) {
					if(board[from.row - i][from.col - i] % 2 == color) break;
					else {
						loc.push_back({from.row - i, from.col - i});
						break;
					}
				}
				loc.push_back({from.row - i, from.col - i});
			}
			for(uint8_t i = 1; i <= (7 - from.row < 7 - from.col ? 7 - from.row : 7 - from.col); i++) {
				if(board[from.row + i][from.col + i] != NONE) {
					if(board[from.row + i][from.col + i] % 2 == color) break;
					else {
						loc.push_back({from.row + i, from.col + i});
						break;
					}
				}
				loc.push_back({from.row + i, from.col + i});
			}
			for(uint8_t i = 1; i <= (7 - from.row < from.col ? 7 - from.row : from.col); i++) {
				if(board[from.row + i][from.col - i] != NONE) {
					if(board[from.row + i][from.col - i] % 2 == color) break;
					else {
						loc.push_back({from.row + i, from.col - i});
						break;
					}
				}
				loc.push_back({from.row + i, from.col - i});
			}
			for(uint8_t i = 1; i <= (from.row < 7 - from.col ? from.row : 7 - from.col); i++) {
				if(board[from.row - i][from.col + i] != NONE) {
					if(board[from.row - i][from.col + i] % 2 == color) break;
					else {
						loc.push_back({from.row - i, from.col + i});
						break;
					}
				}
				loc.push_back({from.row - i, from.col + i});
			}
			break;
		case QUEEN:
			for(uint8_t i = 1; i <= (from.row < from.col ? from.row : from.col); i++) {
				if(board[from.row - i][from.col - i] != NONE) {
					if(board[from.row - i][from.col - i] % 2 == color) break;
					else {
						loc.push_back({from.row - i, from.col - i});
						break;
					}
				}
				loc.push_back({from.row - i, from.col - i});
			}
			for(uint8_t i = 1; i <= (7 - from.row < 7 - from.col ? 7 - from.row : 7 - from.col); i++) {
				if(board[from.row + i][from.col + i] != NONE) {
					if(board[from.row + i][from.col + i] % 2 == color) break;
					else {
						loc.push_back({from.row + i, from.col + i});
						break;
					}
				}
				loc.push_back({from.row + i, from.col + i});
			}
			for(uint8_t i = 1; i <= (7 - from.row < from.col ? 7 - from.row : from.col); i++) {
				if(board[from.row + i][from.col - i] != NONE) {
					if(board[from.row + i][from.col - i] % 2 == color) break;
					else {
						loc.push_back({from.row + i, from.col - i});
						break;
					}
				}
				loc.push_back({from.row + i, from.col - i});
			}
			for(uint8_t i = 1; i <= (from.row < 7 - from.col ? from.row : 7 - from.col); i++) {
				if(board[from.row - i][from.col + i] != NONE) {
					if(board[from.row - i][from.col + i] % 2 == color) break;
					else {
						loc.push_back({from.row - i, from.col + i});
						break;
					}
				}
				loc.push_back({from.row - i, from.col + i});
			}
			for(uint8_t i = 1; i <= from.row; i++) {
				if(board[from.row - i][from.col] != NONE) {
					if(board[from.row - i][from.col] % 2 == color) break;
					else {
						loc.push_back({from.row - i, from.col});
						break;
					}
				}
				loc.push_back({from.row - i, from.col});
			}
			for(uint8_t i = 1; i <= 7 - from.row; i++) {
				if(board[from.row + i][from.col] != NONE) {
					if(board[from.row + i][from.col] % 2 == color) break;
					else {
						loc.push_back({from.row + i, from.col});
						break;
					}
				}
				loc.push_back({from.row + i, from.col});
			}
			for(uint8_t i = 1; i <= from.col; i++) {
				if(board[from.row][from.col - i] != NONE) {
					if(board[from.row][from.col - i] % 2 == color) break;
					else {
						loc.push_back({from.row, from.col - i});
						break;
					}
				}
				loc.push_back({from.row, from.col - i});
			}
			for(uint8_t i = 1; i <= 7 - from.col; i++) {
				if(board[from.row][from.col + i] != NONE) {
					if(board[from.row][from.col + i] % 2 == color) break;
					else {
						loc.push_back({from.row, from.col + i});
						break;
					}
				}
				loc.push_back({from.row, from.col + i});
			}
			break;
		case ROOK:
			for(uint8_t i = 1; i <= from.row; i++) {
				if(board[from.row - i][from.col] != NONE) {
					if(board[from.row - i][from.col] % 2 == color) break;
					else {
						loc.push_back({from.row - i, from.col});
						break;
					}
				}
				loc.push_back({from.row - i, from.col});
			}
			for(uint8_t i = 1; i <= 7 - from.row; i++) {
				if(board[from.row + i][from.col] != NONE) {
					if(board[from.row + i][from.col] % 2 == color) break;
					else {
						loc.push_back({from.row + i, from.col});
						break;
					}
				}
				loc.push_back({from.row + i, from.col});
			}
			for(uint8_t i = 1; i <= from.col; i++) {
				if(board[from.row][from.col - i] != NONE) {
					if(board[from.row][from.col - i] % 2 == color) break;
					else {
						loc.push_back({from.row, from.col - i});
						break;
					}
				}
				loc.push_back({from.row, from.col - i});
			}
			for(uint8_t i = 1; i <= 7 - from.col; i++) {
				if(board[from.row][from.col + i] != NONE) {
					if(board[from.row][from.col + i] % 2 == color) break;
					else {
						loc.push_back({from.row, from.col + i});
						break;
					}
				}
				loc.push_back({from.row, from.col + i});
			}
			break;
		case KING:
			if(from.row < 7) {
				if(board[from.row + 1][from.col] != NONE) {
					if(board[from.row + 1][from.col] % 2 != color)
						if(!inCheck({from.row + 1,from.col}, color))
							loc.push_back({from.row + 1, from.col});
				} else if(!inCheck({from.row + 1,from.col}, color))
					loc.push_back({from.row + 1, from.col});
			}

			if(from.row > 0) {
				if(board[from.row - 1][from.col] != NONE) {
					if(board[from.row - 1][from.col] % 2 != color)
						if(!inCheck({from.row - 1,from.col}, color))
							loc.push_back({from.row - 1, from.col});
				} else if(!inCheck({from.row - 1,from.col}, color))
					loc.push_back({from.row - 1, from.col});
			}

			if(from.row < 7 && from.col < 7) {
				if(board[from.row + 1][from.col + 1] != NONE) {
					if(board[from.row + 1][from.col + 1] % 2 != color)
						if(!inCheck({from.row + 1,from.col + 1}, color))
							loc.push_back({from.row + 1, from.col + 1});
				} else if(!inCheck({from.row + 1,from.col + 1}, color))
					loc.push_back({from.row + 1, from.col + 1});
			}

			if(from.row < 7 && from.col > 0) {
				if(board[from.row + 1][from.col - 1] != NONE) {
					if(board[from.row + 1][from.col - 1] % 2 != color)
						if(!inCheck({from.row + 1,from.col - 1}, color))
							loc.push_back({from.row + 1, from.col - 1});
				} else if(!inCheck({from.row + 1,from.col - 1}, color))
					loc.push_back({from.row + 1, from.col - 1});
			}

			if(from.row > 0 && from.col < 7) {
				if(board[from.row - 1][from.col + 1] != NONE) {
					if(board[from.row - 1][from.col + 1] % 2 != color)
						if(!inCheck({from.row - 1,from.col + 1}, color))
							loc.push_back({from.row + 1, from.col + 1});
				} else if(!inCheck({from.row - 1,from.col + 1}, color))
					loc.push_back({from.row - 1, from.col + 1});
			}

			if(from.row > 0 && from.col > 0) {
				if(board[from.row - 1][from.col - 1] != NONE) {
					if(board[from.row - 1][from.col - 1] % 2 != color)
						if(!inCheck({from.row - 1,from.col - 1}, color))
							loc.push_back({from.row - 1, from.col - 1});
				} else if(!inCheck({from.row - 1,from.col - 1}, color))
					loc.push_back({from.row - 1, from.col - 1});
			}

			if(from.col < 7) {
				if(board[from.row][from.col + 1] != NONE) {
					if(board[from.row][from.col + 1] % 2 != color)
						if(!inCheck({from.row,from.col + 1}, color))
							loc.push_back({from.row, from.col + 1});
				} else if(!inCheck({from.row,from.col + 1}, color))
					loc.push_back({from.row, from.col + 1});
			}

			if(from.col > 0) {
				if(board[from.row][from.col - 1] != NONE) {
					if(board[from.row][from.col - 1] % 2 != color)
						if(!inCheck({from.row,from.col - 1}, color))
							loc.push_back({from.row, from.col - 1});
				} else if(!inCheck({from.row,from.col - 1}, color))
					loc.push_back({from.row, from.col - 1});
			}
			break;
	}
	return loc;
}

bool Chess::inCheck(Location pos, PieceColor color) {
	for(uint8_t i = 1; i <= (pos.row < pos.col ? pos.row : pos.col); i++) {
		if(board[pos.row - i][pos.col - i] != NONE) {
			if(board[pos.row - i][pos.col - i] % 2 == color) break;
			else {
				switch(board[pos.row - i][pos.col - i] - !color) {
					case PAWN:
						if(i == 1)
							return being == color;
						break;
					case BISHOP:
					case QUEEN:
						return 1;
						break;
					case KING:
						if(i == 1)
							return 1;
						break;
				}
				break;
			}
		}

	}
	for(uint8_t i = 1; i <= (7 - pos.row < 7 - pos.col ? 7 - pos.row : 7 - pos.col); i++) {
		if(board[pos.row + i][pos.col + i] != NONE) {
			if(board[pos.row + i][pos.col + i] % 2 == color) break;
			else {
				switch(board[pos.row + i][pos.col + i] - !color) {
					case PAWN:
						if(i == 1)
							return being == color;
						break;
					case BISHOP:
					case QUEEN:
						return 1;
						break;
					case KING:
						if(i == 1)
							return 1;
						break;
				}
				break;
			}
		}

	}
	for(uint8_t i = 1; i <= (7 - pos.row < pos.col ? 7 - pos.row : pos.col); i++) {
		if(board[pos.row + i][pos.col - i] != NONE) {
			if(board[pos.row + i][pos.col - i] % 2 == color) break;
			else {
				switch(board[pos.row + i][pos.col - i] - !color) {
					case PAWN:
						if(i == 1)
							return being == color;
						break;
					case BISHOP:
					case QUEEN:
						return 1;
						break;
					case KING:
						if(i == 1)
							return 1;
						break;
				}
				break;
			}
		}

	}
	for(uint8_t i = 1; i <= (pos.row < 7 - pos.col ? pos.row : 7 - pos.col); i++) {
		if(board[pos.row - i][pos.col + i] != NONE) {
			if(board[pos.row - i][pos.col + i] % 2 == color) break;
			else {
				switch(board[pos.row - i][pos.col + i] - !color) {
					case PAWN:
						if(i == 1)
							return being == color;
						break;
					case BISHOP:
					case QUEEN:
						return 1;
						break;
					case KING:
						if(i == 1)
							return 1;
						break;
				}
				break;
			}
		}
	}
	for(uint8_t i = 1; i <= pos.row; i++) {
		if(board[pos.row - i][pos.col] != NONE) {
			if(board[pos.row - i][pos.col] % 2 == color) break;
			else {
				switch(board[pos.row - i][pos.col] - !color) {
					case ROOK:
					case QUEEN:
						return 1;
						break;
					case KING:
						if(i == 1)
							return 1;
						break;
				}
				break;
			}
		}
	}
	for(uint8_t i = 1; i <= 7 - pos.row; i++) {
		if(board[pos.row + i][pos.col] != NONE) {
			if(board[pos.row + i][pos.col] % 2 == color) break;
			else {
				switch(board[pos.row + i][pos.col] - !color) {
					case ROOK:
					case QUEEN:
						return 1;
						break;
					case KING:
						if(i == 1)
							return 1;
						break;
				}
				break;
			}
		}
	}
	for(uint8_t i = 1; i <= pos.col; i++) {
		if(board[pos.row][pos.col - i] != NONE) {
			if(board[pos.row][pos.col - i] % 2 == color) break;
			else {
				switch(board[pos.row][pos.col - i] - !color) {
					case ROOK:
					case QUEEN:
						return 1;
						break;
					case KING:
						if(i == 1)
							return 1;
						break;
				}
				break;
			}
		}
	}
	for(uint8_t i = 1; i <= 7 - pos.col; i++) {
		if(board[pos.row][pos.col + i] != NONE) {
			if(board[pos.row][pos.col + i] % 2 == color) break;
			else {
				switch(board[pos.row][pos.col + i] - !color) {
					case ROOK:
					case QUEEN:
						return 1;
						break;
					case KING:
						if(i == 1)
							return 1;
						break;
				}
				break;
			}
		}
	}

	if(pos.row < 7 && pos.col > 1)
		if(board[pos.row + 1][pos.col - 2] != NONE)
			if(board[pos.row + 1][pos.col - 2] % 2 != color && board[pos.row + 1][pos.col - 2] - !color == KNIGHT) return 1;
	if(pos.row < 6 && pos.col > 0)
		if(board[pos.row + 2][pos.col - 1] != NONE)
			if(board[pos.row + 2][pos.col - 1] % 2 != color && board[pos.row + 2][pos.col - 1] - !color == KNIGHT) return 1;
	if(pos.row < 6 && pos.col < 7)
		if(board[pos.row + 2][pos.col + 1] != NONE)
			if(board[pos.row + 2][pos.col + 1] % 2 != color && board[pos.row + 2][pos.col + 1] - !color == KNIGHT) return 1;
	if(pos.row < 7 && pos.col < 6)
		if(board[pos.row + 1][pos.col + 2] != NONE)
			if(board[pos.row + 1][pos.col + 2] % 2 != color && board[pos.row + 1][pos.col + 2] - !color == KNIGHT) return 1;
	if(pos.row > 0 && pos.col > 1)
		if(board[pos.row - 1][pos.col - 2] != NONE)
			if(board[pos.row - 1][pos.col - 2] % 2 != color && board[pos.row - 1][pos.col - 2] - !color == KNIGHT) return 1;
	if(pos.row > 1 && pos.col > 0)
		if(board[pos.row - 2][pos.col - 1] != NONE)
			if(board[pos.row - 2][pos.col - 1] % 2 != color && board[pos.row - 2][pos.col - 1] - !color == KNIGHT) return 1;
	if(pos.row > 1 && pos.col < 7)
		if(board[pos.row - 2][pos.col + 1] != NONE)
			if(board[pos.row - 2][pos.col + 1] % 2 != color && board[pos.row - 2][pos.col + 1] - !color == KNIGHT) return 1;
	if(pos.row > 0 && pos.col < 6)
		if(board[pos.row - 1][pos.col + 2] != NONE)
			if(board[pos.row - 1][pos.col + 2] % 2 != color && board[pos.row - 1][pos.col + 2] - !color == KNIGHT) return 1;

	return 0;
}
