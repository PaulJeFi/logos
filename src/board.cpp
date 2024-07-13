#include <stdint.h>
#include <iostream>

#include "board.h"
#include "bitboard.h"
#include "types.h"


void clearBoard(Board *board) {
	for (int i = 0; i < 12; i++) {
		board->pieces[i] = 0;
	}
	board->turn = true;
	board->halfMoveCounter = 0;
	board->fullMoveCounter = 0;
	board->epSquare = -1;
	board->castling = 0;
}


int stringToSquare(char *sq) {
	// Since a8=63, b8=62,... the 7- is needed
	int file = 7-(sq[0]-'a');
	int rank = sq[1]-'1';
	return 8*rank + file;
}


void boardFromFEN(Board *board, const char *fen) {
	clearBoard(board);
	int piece;
	int square = 63;
	char c;

	char *str = strdup(fen);
	char *strPos = NULL;
	char *token = strtok_r(str, " ", &strPos);

	// Getting the piece placement
	while ((c = *token++)) {
		if (isdigit(c)) {
			square -= c-'0';
		} else if (c != '/') {
			switch (c) {
				case 'P':
					piece = W_PAWN;
					break;
				case 'N':
					piece = W_KNIGHT;
					break;
				case 'B':
					piece = W_BISHOP;
					break;
				case 'R':
					piece = W_ROOK;
					break;
				case 'Q':
					piece = W_QUEEN;
					break;
				case 'K':
					piece = W_KING;
					break;
				case 'p':
					piece = B_PAWN;
					break;
				case 'n':
					piece = B_KNIGHT;
					break;
				case 'b':
					piece = B_BISHOP;
					break;
				case 'r':
					piece = B_ROOK;
					break;
				case 'q':
					piece = B_QUEEN;
					break;
				case 'k':
					piece = B_KING;
					break;
			}
			board->pieces[piece] |= 1ull << square;
			square--;
		}
	}
	// Side to move
	token = strtok_r(NULL, " ", &strPos);
	board->turn = token[0] == 'w' ? true : false;

	// Castling rights
	token = strtok_r(NULL, " ", &strPos);
	while ((c = *token++)) {
		switch(c) {
			case 'K':
				board->castling |= 1ull;
				break;
			case 'Q':
				board->castling |= 1ull << 1;
				break;
			case 'k':
				board->castling |= 1ull << 2;
				break;
			case 'q':
				board->castling |= 1ull << 3;
				break;
		}
	}

	// En passant square
	token = strtok_r(NULL, " ", &strPos);
	if (token[0] == '-') {
		board->epSquare = -1;
	} else {
		board->epSquare = stringToSquare(token);
	}

	// Half move counter
	token = strtok_r(NULL, " ", &strPos);
	int counter = 0;
	while ((c = *token++)) {
		counter *= 10;
		counter += c-'0';
	}
	board->halfMoveCounter = counter;

	// Full move counter
	token = strtok_r(NULL, " ", &strPos);
	counter = 0;
	while (isdigit(c)) {
		counter *= 10;
		counter += c-'0';
	}
	board->fullMoveCounter = counter;
}

void printBoard(Board *board) {
	// TODO
	for (uint64_t bb : board->pieces) {
		printBitboard(bb);
		std::cout << "----------" << "\n";
	}
	std::cout << board->turn << ", ";
	std::cout << board->halfMoveCounter << ", ";
	std::cout << board->fullMoveCounter << ", ";
	std::cout << board->epSquare << ", ";
	std::cout << board->castling << "\n";
}
