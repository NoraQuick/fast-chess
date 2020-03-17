
 ============================================================================
 Name        : fast-chess.c
 Author      : Frederico Jordan <fredericojordan@gmail.com>
 Version     :
 Copyright   : Copyright (c) 2016 Frederico Jordan
 Description : Simple chess game!
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "fast-chess.h"

char FILES[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
char RANKS[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

//declaring a bit array data structure.
Bitboard FILES_BB[8] = { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
Bitboard RANKS_BB[8] = { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

//declaring the starting position. FEN ~ stands for Extended Position Description
char INITIAL_FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

//initializing the board and the Chess pieces 
int INITIAL_BOARD[NUM_SQUARES] = { WHITE|ROOK, WHITE|KNIGHT, WHITE|BISHOP, WHITE|QUEEN, WHITE|KING, WHITE|BISHOP, WHITE|KNIGHT, WHITE|ROOK,
                                   WHITE|PAWN, WHITE|PAWN,   WHITE|PAWN,   WHITE|PAWN,  WHITE|PAWN, WHITE|PAWN,   WHITE|PAWN,   WHITE|PAWN,
                                   EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY,
                                   EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY,
                                   EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY,
                                   EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY,
                                   BLACK|PAWN, BLACK|PAWN,   BLACK|PAWN,   BLACK|PAWN,  BLACK|PAWN, BLACK|PAWN,   BLACK|PAWN,   BLACK|PAWN,
                                   BLACK|ROOK, BLACK|KNIGHT, BLACK|BISHOP, BLACK|QUEEN, BLACK|KING, BLACK|BISHOP, BLACK|KNIGHT, BLACK|ROOK };

int PIECE_VALUES[] = { 0, 100, 300, 310, 500, 900, 42000 };

//Initializing Pawn Bonus
int PAWN_BONUS[] = {0,   0,   0,   0,   0,   0,   0,   0,
					0,   0,   0, -40, -40,   0,   0,   0,
					1,   2,   3, -10, -10,   3,   2,   1,
					2,   4,   6,   8,   8,   6,   4,   2,
					3,   6,   9,  12,  12,   9,   6,   3,
					4,   8,  12,  16,  16,  12,   8,   4,
					5,  10,  15,  20,  20,  15,  10,   5,
					0,   0,   0,   0,   0,   0,   0,   0};
//Initializing Knight Bonus
int KNIGHT_BONUS[] = {-10, -30, -10, -10, -10, -10, -30, -10,
					  -10,   0,   0,   0,   0,   0,   0, -10,
					  -10,   0,   5,   5,   5,   5,   0, -10,
					  -10,   0,   5,  10,  10,   5,   0, -10,
					  -10,   0,   5,  10,  10,   5,   0, -10,
					  -10,   0,   5,   5,   5,   5,   0, -10,
					  -10,   0,   0,   0,   0,   0,   0, -10,
					  -10, -10, -10, -10, -10, -10, -10, -10};
//Initializing Bishop Bonus
int BISHOP_BONUS[] = {-10, -10, -20, -10, -10, -20, -10, -10,
					  -10,   0,   0,   0,   0,   0,   0, -10,
					  -10,   0,   5,   5,   5,   5,   0, -10,
					  -10,   0,   5,  10,  10,   5,   0, -10,
					  -10,   0,   5,  10,  10,   5,   0, -10,
					  -10,   0,   5,   5,   5,   5,   0, -10,
					  -10,   0,   0,   0,   0,   0,   0, -10,
					  -10, -10, -10, -10, -10, -10, -10, -10};
//Initializing King Bonus
int KING_BONUS[] = { 0,  20,  40, -20,   0, -20,  40,  20,
				   -20, -20, -20, -20, -20, -20, -20, -20,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40};
//Initializing Knight End game Bonus
int KING_ENDGAME_BONUS[] = { 0,  10,  20,  30,  30,  20,  10,   0,
							10,  20,  30,  40,  40,  30,  20,  10,
							20,  30,  40,  50,  50,  40,  30,  20,
							30,  40,  50,  60,  60,  50,  40,  30,
							30,  40,  50,  60,  60,  50,  40,  30,
							20,  30,  40,  50,  50,  40,  30,  20,
							10,  20,  30,  40,  40,  30,  20,  10,
							 0,  10,  20,  30,  30,  20,  10,   0};
//Initializing Flip Vertical
int FLIP_VERTICAL[] = {56,  57,  58,  59,  60,  61,  62,  63,
			           48,  49,  50,  51,  52,  53,  54,  55,
			           40,  41,  42,  43,  44,  45,  46,  47,
			           32,  33,  34,  35,  36,  37,  38,  39,
			           24,  25,  26,  27,  28,  29,  30,  31,
			           16,  17,  18,  19,  20,  21,  22,  23,
			            8,   9,  10,  11,  12,  13,  14,  15,
			            0,   1,   2,   3,   4,   5,   6,   7};

//This function will initialize the start of the game and copy data from source to destination
	void getInitialGame(Game * game) {
	memcpy(game->position.board, INITIAL_BOARD, sizeof(game->position.board));
	game->position.toMove = WHITE;
	game->position.epSquare = -1;
	game->position.castlingRights = CASTLE_KINGSIDE_WHITE|CASTLE_QUEENSIDE_WHITE|CASTLE_KINGSIDE_BLACK|CASTLE_QUEENSIDE_BLACK;
	game->position.halfmoveClock = 0;
	game->position.fullmoveNumber = 1;

	game->moveListLen = 0;
	memset(game->moveList, 0, MAX_PLYS_PER_GAME*sizeof(int));
	memset(game->positionHistory, 0, MAX_PLYS_PER_GAME*MAX_FEN_LEN*sizeof(char));
	memcpy(game->positionHistory[0], INITIAL_FEN, sizeof(INITIAL_FEN));
}

//Fen stands for FenForge.
//This function is used to allow an online multiplayer
void getFenGamec
	game->moveListLen = 0;
	memset(game->moveList, 0, MAX_PLYS_PER_GAME*sizeof(int));
	memset(game->positionHistory, 0, MAX_PLYS_PER_GAME*MAX_FEN_LEN*sizeof(char));
	memcpy(game->positionHistory[0], fen, fenLen);
}

//This function will load the info about an online player
int loadFen(Position * position, char fen[]) {
	// ===== BOARD =====
	memset(position->board, EMPTY, sizeof(position->board));
//This is the starting position
	int rank = 7;
	int boardPos = rank*8; // the board of the Position
	char * charPos = fen;

	char pieceCode = *(charPos);
//loop for getting all the pieces until end 
	while(pieceCode != ' ') {
		if (pieceCode == '/') {
			rank--;
			boardPos = rank*8;
		} else if (isdigit(pieceCode)) {
			int emptySquares = atoi(charPos);
			boardPos += emptySquares;
		} else {
			position->board[boardPos++] = char2piece(pieceCode);
		}

		pieceCode = *(++charPos);
	}


	// ===== TO MOVE =====
	char *nextFenField = strchr(fen, ' ') + 1;
//the team movement turns 
	if (*nextFenField == 'b') {
		position->toMove = BLACK;
	} else {
		position->toMove = WHITE;
	}

	// ===== CASTLING RIGHTS =====
	nextFenField = strchr(nextFenField, ' ') + 1;
	position->castlingRights = 0;
	if (strchr(nextFenField, 'K'))
		position->castlingRights |= CASTLE_KINGSIDE_WHITE;
	if (strchr(nextFenField, 'Q'))
		position->castlingRights |= CASTLE_QUEENSIDE_WHITE;
	if (strchr(nextFenField, 'k'))
		position->castlingRights |= CASTLE_KINGSIDE_BLACK;
	if (strchr(nextFenField, 'q'))
		position->castlingRights |= CASTLE_QUEENSIDE_BLACK;

	// ===== EN PASSANT =====
	nextFenField = strchr(nextFenField, ' ') + 1;
//other side turn and position
	if (*nextFenField == '-') {
		position->epSquare = -1;
	} else {
		position->epSquare = str2index(nextFenField);
	}

	// ===== HALF MOVE CLOCK =====
	if (!strchr(nextFenField, ' ')) {
		position->halfmoveClock = 0;
		position->fullmoveNumber = 1;
		return 1+nextFenField-fen;
	}
	nextFenField = strchr(nextFenField, ' ') + 1;
 // Now the capture/pawn halfmove clock:
	position->halfmoveClock = atoi(nextFenField);

	// ===== FULL MOVE NUMBER =====
	if (!strchr(nextFenField, ' ')) {
		position->fullmoveNumber = 0;
		return 1+nextFenField-fen;
	}
	nextFenField = strchr(nextFenField, ' ') + 1;
// Finally, the fullmove counter:
	position->fullmoveNumber = atoi(nextFenField);

	return 1+nextFenField-fen;
}

int toFen(char * fen, Position * position) {
	int charCount = toMinFen(fen, position);
	fen[charCount-1] = ' ';

	// ===== HALF MOVE CLOCK =====
	snprintf(&fen[charCount++], 2, "%d", position->halfmoveClock);
	if (position->halfmoveClock >= 10) {
		charCount++;
		if (position->halfmoveClock >= 100) {
			charCount++;
		}
	}
	fen[charCount++] = ' ';

	// ===== FULL MOVE NUMBER =====
	snprintf(&fen[charCount++], 2, "%d", position->fullmoveNumber);
	if (position->fullmoveNumber >= 10) {
		charCount++;
		if (position->fullmoveNumber >= 100) {
			charCount++;
		}
	}
	fen[charCount++] = '\0';

	return charCount;
}
//This function is for the board position.
int toMinFen(char * fen, Position * position) {
	int charCount = 0;

	// ===== BOARD =====
	int rank = 7;
	int file = 0;
	int emptySquares = 0;
//keep looping until rank is grater or equal to 0
	while(rank >= 0) {
		int piece = position->board[8*rank+file];

		if ( piece == EMPTY ) {
			emptySquares++; //add one to the emptySquares 
		} else {
			if (emptySquares != 0) {        //if is is nor an enmtySquares
			    snprintf(&fen[charCount++], 2, "%d", emptySquares);
				emptySquares = 0;
			}
			fen[charCount++] = piece2char(piece);
		}

		file++;
		if ( file > 7 ) {
			if (emptySquares != 0) {
			    snprintf(&fen[charCount++], 2, "%d", emptySquares);
				emptySquares = 0;
			}
			file = 0;
			rank--;
			fen[charCount++] = '/';
		}
	}
	fen[charCount-1] = ' ';


	// ===== TO MOVE =====
	if (position->toMove == BLACK) {
		fen[charCount++] = 'b';
	} else {
		fen[charCount++] = 'w';
	}
	fen[charCount++] = ' ';

	// ===== CASTLING RIGHTS =====
	if (position->castlingRights == 0) {
			fen[charCount++] = '-';
	} else {
		if (position->castlingRights & CASTLE_KINGSIDE_WHITE) {
			fen[charCount++] = 'K';
		}
		if (position->castlingRights & CASTLE_QUEENSIDE_WHITE) {
			fen[charCount++] = 'Q';
		}
		if (position->castlingRights & CASTLE_KINGSIDE_BLACK) {
			fen[charCount++] = 'k';
		}
		if (position->castlingRights & CASTLE_QUEENSIDE_BLACK) {
			fen[charCount++] = 'q';
		}
	}
	fen[charCount++] = ' ';

	// ===== EN PASSANT =====
	if (position->epSquare == -1) {
			fen[charCount++] = '-';
	} else {
		fen[charCount++] = getFile(position->epSquare);
		fen[charCount++] = getRank(position->epSquare);
	}
	fen[charCount++] = '\0';

	return charCount;
}

// ========= UTILITY =========
BOOL fromInitial(Game * game) {
	if ( strcmp(game->positionHistory[0], INITIAL_FEN) == 0 )
		return TRUE;
	else
		return FALSE;
}

Bitboard index2bb(int index) {
	Bitboard bb = 1;
	return bb << index;
}

int str2index(char *str) {
	int i, file_num=0, rank_num=0;
	for(i=0; i<8; i++) {
		if ( str[0] == FILES[i] )
			file_num = i;
		if ( str[1] == RANKS[i] )
			rank_num = i;
	}
	return 8*rank_num + file_num;
}

Bitboard str2bb(char *str) {
	return index2bb(str2index(str));
}
//Boolean checker for available positions
BOOL isSet(Bitboard bb, int index) {
	if (bb & index2bb(index))
		return TRUE;
	else
		return FALSE;
}
//This function to check the number of Squares 
Bitboard lsb(Bitboard bb) {
	int i;
	for (i=0; i<NUM_SQUARES; i++) {
		Bitboard bit = index2bb(i);
		if (bb & bit)
			return bit;
	}
	return 0;
}
//This will return the number of squares in the game
Bitboard msb(Bitboard bb) {
	int i;
	for (i=0; i<NUM_SQUARES; i++) {
		Bitboard bit = index2bb(63-i);
		if (bb & bit)
			return bit;
	}
	return 0;
}
//This function is for the index of the bitBoard.
int bb2index(Bitboard bb) {
	int i;
	for (i=0; i<NUM_SQUARES; i++) {
		Bitboard bit = index2bb(i);
		if (bb & bit)
			return i;
	}
	return -1;
}
//This is the movement list for the game 
char * movelist2str(Game * game) {
	char * movestr = NULL;

	if (game->moveListLen == 0) {
		movestr = (char*) malloc(sizeof(char));
		movestr[0] = 0;
		return movestr;
	}

	movestr = (char*) malloc (5*game->moveListLen);

	int i;
	for (i=0;i<game->moveListLen;i++) {
		int leaving = getFrom(game->moveList[i]);
		int arriving = getTo(game->moveList[i]);
		movestr[5*i] = getFile(leaving);
		movestr[5*i+1] = getRank(leaving);
		movestr[5*i+2] = getFile(arriving);
		movestr[5*i+3] = getRank(arriving);
		movestr[5*i+4] = ' ';
	}

	movestr[5*game->moveListLen-1] = 0;

	return movestr;
}
// This function will have the last move in the game
Move getLastMove(Game * game) {
	if (game->moveListLen == 0)
		return 0;
	else
		return game->moveList[game->moveListLen-1];
}
//This function to has the start of the game
BOOL startsWith(const char *str, const char *pre) {
    size_t lenpre = strlen(pre), lenstr = strlen(str);

    if (lenpre > lenstr)
    	return FALSE;

    return strncmp(pre, str, lenpre) == 0 ? TRUE : FALSE;
}
//This function will read from a file names book.txt 
int countBookOccurrences(Game * game) {
    FILE * fp = fopen("book.txt", "r");

    if (fp == NULL)
        return 0;

    char * moveList = movelist2str(game);
    char *line = (char *) malloc(sizeof(char) * MAX_BOOK_ENTRY_LEN);
    int charPos = 0, occurrences = 0;

    while (TRUE) {
		char ch = getc(fp);
		line[charPos++] = ch;

		if ( ch == '\n' || ch == EOF ) {
			line[charPos-1] = '\0';

			if (startsWith(line, moveList) && strlen(line) > strlen(moveList)) {
				occurrences++;
			}

			if ( ch == EOF )
				break;

			charPos = 0;
		}
    }

    fclose(fp);
    free(line);
    free(moveList);

    return occurrences;
}
// This function will check for the possibilities of the movement
Move getBookMove(Game * game) {
	Move move = 0;
	int moveNum = rand() % countBookOccurrences(game);

	FILE * fp = fopen("book.txt", "r");

	if (fp == NULL)
		return 0;

	char * moveList = movelist2str(game);
	char *line = (char *) malloc(sizeof(char) * MAX_BOOK_ENTRY_LEN);
	int charPos = 0, occurrences = 0;

	while (TRUE) {
		char ch = getc(fp);
		line[charPos++] = ch;

		if ( ch == '\n' ) {
			line[charPos] = '\0';

			if (startsWith(line, moveList)) {
				if ( occurrences == moveNum ) {
					int ind = game->moveListLen*5;
					move = parseMove(&line[ind]);
					break;
				}
				occurrences++;
			}

			charPos = 0;
		}
	}

	fclose(fp);
	free(line);
	free(moveList);

	return move;
}

char getFile(int position) {
	int file = position%8;
	return FILES[file];
}

char getRank(int position) {
	int rank = (int) (position/8);
	return RANKS[rank];
}
//this function is used to genera moves in the game
Move generateMove(int leavingSquare, int arrivingSquare) {
	int leaving = (leavingSquare << 8);
	int arriving = arrivingSquare;
	return (Move) (leaving + arriving);
}

int getFrom(Move move) {
	return (move >> 8) & 0xFF;
}
//for movements
int getTo(Move move) {
	return move & 0xFF;
}

int char2piece(char pieceCode) {
	switch(pieceCode) {
	case 'P':
		return WHITE|PAWN;
	case 'N':
		return WHITE|KNIGHT;
	case 'B':
		return WHITE|BISHOP;
	case 'R':
		return WHITE|ROOK;
	case 'Q':
		return WHITE|QUEEN;
	case 'K':
		return WHITE|KING;

	case 'p':
		return BLACK|PAWN;
	case 'n':
		return BLACK|KNIGHT;
	case 'b':
		return BLACK|BISHOP;
	case 'r':
		return BLACK|ROOK;
	case 'q':
		return BLACK|QUEEN;
	case 'k':
		return BLACK|KING;
	}
	return 0;
}
//switch statement to check the input from the user 
char piece2char(int piece) {
	switch(piece) {
	case WHITE|PAWN:
		return 'P';
	case WHITE|KNIGHT:
		return 'N';
	case WHITE|BISHOP:
		return 'B';
	case WHITE|ROOK:
		return 'R';
	case WHITE|QUEEN:
		return 'Q';
	case WHITE|KING:
		return 'K';
	case BLACK|PAWN:
		return 'p';
	case BLACK|KNIGHT:
		return 'n';
	case BLACK|BISHOP:
		return 'b';
	case BLACK|ROOK:
		return 'r';
	case BLACK|QUEEN:
		return 'q';
	case BLACK|KING:
		return 'k';
	case EMPTY:
		return '.';
	}
	return 0;
}
// switch statement to check the specified piece 
char * piece2str(int piece) {
	switch(piece&PIECE_MASK) {
	case PAWN:
		return "Pawn";
	case KNIGHT:
		return "Knight";
	case BISHOP:
		return "Bishop";
	case ROOK:
		return "Rook";
	case QUEEN:
		return "Queen";
	case KING:
		return "King";
	case EMPTY:
		return "none";
	}
	return "";
}
//to print the bit board for the game
void printBitboard(Bitboard bitboard) {
	int rank, file;

	printf("\n");
	for (rank=0; rank<8; rank++) {
		printf("%d", 8-rank);
		for (file=0; file<8; file++) {
			if ( bitboard>>(file + (7-rank)*8) & 1 ) {
				printf(" #");
			} else {
				printf(" .");
			}
		}
		printf("\n");
	}
	printf("  a b c d e f g h\n");
	fflush(stdout);
}
//This print statement for the bord of the chess game
void printBoard(int board[]) {
	int rank, file;

	printf("\n");
	for (rank=0; rank<8; rank++) {
		printf("%d", 8-rank);
		for (file=0; file<8; file++) {
			int position = file + (7-rank)*8;
			printf(" %c", piece2char(board[position]));
		}
		printf("\n");
	}
	printf("  a b c d e f g h\n");
	fflush(stdout);
}
//To print the pices positions
void printGame(Game * game) {
	printf("Game -> %p (%lu)", game, sizeof(*game));
	printBoard(game->position.board);
	printf("board -> %p (%lu)\n", game->position.board, sizeof(game->position.board));
	printf("toMove = %d -> %p (%lu)\n", game->position.toMove, &game->position.toMove, sizeof(game->position.toMove));
	printf("ep = %d -> %p (%lu)\n", game->position.epSquare, &game->position.epSquare, sizeof(game->position.epSquare));
	printf("castle rights = %d -> %p (%lu)\n", game->position.castlingRights, &game->position.castlingRights, sizeof(game->position.castlingRights));
	printf("half clock = %d -> %p (%lu)\n", game->position.halfmoveClock, &game->position.halfmoveClock, sizeof(game->position.halfmoveClock));
	printf("full num = %d -> %p (%lu)\n", game->position.fullmoveNumber, &game->position.fullmoveNumber, sizeof(game->position.fullmoveNumber));

	printf("moveListLen = %d -> %p (%lu)\n", game->moveListLen, &game->moveListLen, sizeof(game->moveListLen));
	printf("moveList -> %p (%lu)\n", game->moveList, sizeof(game->moveList));
	printf("positionHistory -> %p (%lu)\n", game->positionHistory, sizeof(game->positionHistory));
	fflush(stdout);
}

Bitboard not(Bitboard bb) {
	return ~bb & ALL_SQUARES;
}
//switch between colors 
char opponent(char color) {
	switch(color) {
	case WHITE:
		return BLACK;
	case BLACK:
		return WHITE;
	}
	return -1;
}
// cont the number of bits
int countBits(Bitboard bb) {
	int i, bitCount = 0;
	for (i=0; i<NUM_SQUARES; i++) {
		if (index2bb(i) & bb)
			bitCount++;
	}
	return bitCount;
}

void sortNodes(Node * sortedNodes, Node * nodes, int len, char color) {
	Node nodeBuffer[len];

	int i, j;
	BOOL sorted;
	for (i=0; i<len; i++) {
		sorted = FALSE;

		for (j=0; j<i; j++) {
			if ( (color == WHITE && nodes[i].score > sortedNodes[j].score) ||
				 (color == BLACK && nodes[i].score < sortedNodes[j].score) ) {
				sorted = TRUE;
				memcpy(nodeBuffer, &sortedNodes[j], (i-j)*sizeof(Node));
				memcpy(&sortedNodes[j+1], nodeBuffer, (i-j)*sizeof(Node));
				sortedNodes[j] = nodes[i];
				break;
			}
		}

		if ( sorted == FALSE ) {
			sortedNodes[i] = nodes[i];
		}
	}
}
//This function will print the moves in the game
void printMove(Move move) {
	printf("%c%c to %c%c", getFile(getFrom(move)), getRank(getFrom(move)), getFile(getTo(move)), getRank(getTo(move)));
}
//This function will print the full moves of the game
void printFullMove(Move move, int board[]) {
	int from = getFrom(move);
	int piece = board[from];
	printf("%s from ", piece2str(piece));
	printMove(move);
}
//This function will print the legal moves of the game
void printLegalMoves(Position * position) {
	int i;
	Move moves[MAX_BRANCHING_FACTOR];
	int moveCount = legalMoves(moves, position, position->toMove);
	for (i=0; i<moveCount; i++) {
		printf("%2d. ", i+1);
		printFullMove(moves[i], position->board);
//		printMove(moves[i]);
		printf("\n");
	}
	fflush(stdout);
}

void printNode(Node node) {
	printMove(node.move);
	printf(": %d", node.score);
}
//This function is for the time in the game
void getTimestamp(char * timestamp) {
    time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(timestamp, 20, "%Y-%m-%d_%H.%M.%S", tm_info);
}

void dumpContent(Game * game) {
	char * movelist = movelist2str(game);

	char filename[50];
	sprintf(filename, "chess_game_");
	getTimestamp(&filename[strlen(filename)]);
	sprintf(&filename[strlen(filename)], ".txt");

	FILE * file = fopen(filename, "w+");

	fprintf(file, "movelist = %s\nposition history:\n", movelist);

	int i;
	for (i=0; i<game->moveListLen+1; i++)
		fprintf(file, "%s\n", game->positionHistory[i]);

	free(movelist);
	fclose(file);

	printf("Dumped game content to: %s\n", filename);
	fflush(stdout);
}

void dumpPGN(Game * game, char color, BOOL hasAI) {
	char filename[50];
	sprintf(filename, "chess_game_");
	getTimestamp(&filename[strlen(filename)]);
	sprintf(&filename[strlen(filename)], ".pgn");

	FILE * file = fopen(filename, "w+");

	char date[12];
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(date, 11, "%Y.%m.%d", tm_info);


	fprintf(file, "[Event \"Casual Game\"]\n");
	fprintf(file, "[Site \"?\"]\n");
	fprintf(file, "[Date \"%s\"]\n", date);
	fprintf(file, "[Round \"-\"]\n");

	if  ( hasAI ) {
		if ( color == WHITE ) {
			fprintf(file, "[White \"%s\"]\n", HUMAN_NAME);
			fprintf(file, "[Black \"%s\"]\n", ENGINE_NAME);
		} else {
			fprintf(file, "[White \"%s\"]\n", ENGINE_NAME);
			fprintf(file, "[Black \"%s\"]\n", HUMAN_NAME);
		}
	} else {
		fprintf(file, "[White \"Unknown Human Player\"]\n");
		fprintf(file, "[Black \"Unknown Human Player\"]\n");
	}

	if ( hasGameEnded(&game->position) ) {
		if ( endNodeEvaluation(&game->position) == winScore(WHITE) ) {
			fprintf(file, "[Result \"1-0\"]\n");
		} else if ( endNodeEvaluation(&game->position) == winScore(BLACK) ) {
			fprintf(file, "[Result \"0-1\"]\n");
		} else if ( endNodeEvaluation(&game->position) == 0 ) {
			fprintf(file, "[Result \"1/2-1/2\"]\n");
		}
	} else {
		fprintf(file, "[Result \"*\"]\n");
	}

	if ( strcmp(game->positionHistory[0], INITIAL_FEN) == 0) {
		fprintf(file, "[Variant \"Standard\"]\n");
	} else {
		fprintf(file, "[Variant \"From Position\"]\n");
		fprintf(file, "[FEN \"%s\"]\n", game->positionHistory[0]);
	}

	fprintf(file, "[PlyCount \"%d\"]\n\n", game->moveListLen);


	int i;
	char ply[8];
	for (i=0;i<game->moveListLen;i++) {
		if (i%2==0) fprintf(file, "%d. ", 1+(i/2));
		move2str(ply, game, i);
		fprintf(file, "%s ", ply);
	}

	fclose(file);

	printf("Dumped game pgn to: %s\n", filename);
	fflush(stdout);
}
//This will take care of the movement during the game
void move2str(char * str, Game * game, int moveNumber) { // TODO: refactor
	Position posBefore, posAfter;
	loadFen(&posBefore, game->positionHistory[moveNumber]);
	loadFen(&posAfter, game->positionHistory[moveNumber+1]);
	Move move = game->moveList[moveNumber];
	int leavingSquare = getFrom(move);
	int arrivingSquare = getTo(move);
	int movingPiece = posBefore.board[leavingSquare];
	int capturedPiece = posBefore.board[arrivingSquare];

	int length = 0;
	if ( (movingPiece&PIECE_MASK) == KING && abs(leavingSquare-arrivingSquare) == 2 ) { // if castling
		if ( index2bb(arrivingSquare)&FILE_G ) {
			sprintf(str, "O-O");
			length += 3;
		} else if ( index2bb(arrivingSquare)&FILE_C ) {
			sprintf(str, "O-O-O");
			length += 5;
		}
	} else { // if not castling
		if ( (movingPiece&PIECE_MASK) == PAWN ) {
			if ( capturedPiece != EMPTY ) {
				str[length++] = getFile(leavingSquare);
			}
		} else {
			str[length++] = piece2char(movingPiece&PIECE_MASK);
		}

		if( isAmbiguous(&posBefore, move) ) {
			if ( countBits( getColoredPieces(posBefore.board, movingPiece&COLOR_MASK)&getPieces(posBefore.board, movingPiece&PIECE_MASK)&fileFilter(index2bb(leavingSquare)) ) == 1 ) {
				str[length++] = getFile(leavingSquare);
			} else {
				str[length++] = getRank(leavingSquare);
			}
		}

		if ( capturedPiece != EMPTY ) {
			str[length++] = 'x';
		}

		str[length++] = getFile(arrivingSquare);
		str[length++] = getRank(arrivingSquare);
	}

	if ( isCheckmate(&posAfter) ) {
		str[length++] = '#';
	} else if (isCheck(posAfter.board, posAfter.toMove)) {
		str[length++] = '+';
	}

	str[length++] = 0;
}

BOOL isAmbiguous(Position * posBefore, Move move) {
	int i, attackCount = 0;
	Move moves[MAX_BRANCHING_FACTOR];
	int moveCount = legalMoves(moves, posBefore, posBefore->toMove);

	for (i=0; i<moveCount; i++) {
		if ( getTo(moves[i]) == getTo(move) &&
				posBefore->board[getFrom(moves[i])] == posBefore->board[getFrom(move)] ) {
			attackCount++;
		}
	}

	return attackCount > 1;
}

unsigned long hashPosition(Position * position) {
	char fen[MAX_FEN_LEN];
	toMinFen(fen, position);

    unsigned long hash = 5381;
    int c, i=0;

    while ((c = fen[i++])) {
    	hash = ((hash << 5) + hash) + c;
    }

    return hash;
}
//This to read from the file
void writeToHashFile(Position * position, int evaluation, int depth) {
    FILE * fp = fopen("hashfile", "a");

    if (fp == NULL)
        return;

    char fen[MAX_FEN_LEN];
    toMinFen(fen, position);

    fprintf(fp, "%08lx %d %d %s\n", hashPosition(position), depth, evaluation, fen);
    fclose(fp);
}

// ====== BOARD FILTERS ======

Bitboard getColoredPieces(int board[], char color) {
	int i;
	Bitboard colored_squares = 0;

	for (i=0; i<NUM_SQUARES; i++)
		if (board[i] != EMPTY && (board[i]&COLOR_MASK) == color)
			colored_squares |= index2bb(i);

	return colored_squares;
}
//This function to make ure there are empty squares
Bitboard getEmptySquares(int board[]) {
	int i;
	Bitboard empty_squares = 0;

	for (i=0; i<NUM_SQUARES; i++)
		if (board[i] == EMPTY)
			empty_squares |= index2bb(i);

	return empty_squares;
}

Bitboard getOccupiedSquares(int board[]) {
	return not(getEmptySquares(board));
}
//This function will get the pieces of the game
Bitboard getPieces(int board[], int pieceType) {
	int i;
	Bitboard pieces = 0;

	for (i=0; i<NUM_SQUARES; i++)
		if ((board[i]&PIECE_MASK) == pieceType)
			pieces |= index2bb(i);

	return pieces;
}

Bitboard fileFilter(Bitboard positions) {
	Bitboard filter = 0;
	int i;

	for (i=0; i<8; i++)
		if (positions&FILES_BB[i])
			filter |= FILES_BB[i];
	return filter;
}

Bitboard rankFilter(Bitboard positions) {
	Bitboard filter = 0;
	int i;

	for (i=0; i<8; i++)
		if (positions&RANKS_BB[i])
			filter |= RANKS_BB[i];
	return filter;
}
//the number of pieces in the game
char countPieces(Bitboard bitboard) {
	int i, count=0;
	for (i=0; i<NUM_SQUARES; i++) {
		if (index2bb(i)&bitboard)
			count += 1;
	}
    return count;
}

// ==============================================================
// 					========= DIRECTIONS ==========
// ==============================================================

// Shifts entire bitboard to the east
Bitboard east(Bitboard bb) {
	// Shift bitboard to the right, nullify far left column
    return (bb << 1) & not(FILE_A);
}

// Shifts entire bitboard to the west
Bitboard west(Bitboard bb) {
	// Shift bitboard to the left, nullify far right column
    return (bb >> 1) & not(FILE_H);
}

// Shifts entire bitboard to the north
Bitboard north(Bitboard bb) {
	// Shift bitboard north, nullify bottommost row
    return (bb << 8) & not(RANK_1);
}

// Shifts entire bitboard to the south
Bitboard south(Bitboard bb) {
	// Shift bitboard south, nullify topmost row
    return (bb >> 8) & not(RANK_8);
}

// Combinations of the above to get NE, SE, NW, SW movements

Bitboard NE(Bitboard bb) {
    return north(east(bb));
}

Bitboard NW(Bitboard bb) {
    return north(west(bb));
}

Bitboard SE(Bitboard bb) {
    return south(east(bb));
}

Bitboard SW(Bitboard bb) {
    return south(west(bb));
}

// Knight movements

Bitboard WNW(Bitboard moving_piece) {
	// Move bitboard two spaces west and one space north, nullify 2 right and 1 bottom
    return moving_piece << 6 & not(FILE_G | FILE_H | RANK_1);
}

Bitboard ENE(Bitboard moving_piece) {
	// Move bitboard two spaces east and one space north, nullify 2 left and 1 bottom
    return moving_piece << 10 & not(FILE_A | FILE_B | RANK_1);
}

Bitboard NNW(Bitboard moving_piece) {
	// Move bitboard two spaces north and one space west, nullify 2 bottom and 1 right
    return moving_piece << 15 & not(FILE_H | RANK_1 | RANK_2);
}

Bitboard NNE(Bitboard moving_piece) {
	// Move bitboard two spaces north and one space east, nullify 2 bottom and 1 left
    return moving_piece << 17 & not(FILE_A | RANK_1 | RANK_2);
}

Bitboard ESE(Bitboard moving_piece) {
	// Move bitboard two spaces east and one space south, nullify 2 right and 1 top
    return moving_piece >> 6 & not(FILE_A | FILE_B | RANK_8);
}

Bitboard WSW(Bitboard moving_piece) {
	// Move bitboard two spaces west and one space south, nullify 2 left and 1 top
    return moving_piece >> 10 & not(FILE_G | FILE_H | RANK_8);
}

Bitboard SSE(Bitboard moving_piece) {
	// Move bitboard two spaces south and one space east, nullify 2 top and 1 left
    return moving_piece >> 15 & not(FILE_A | RANK_7 | RANK_8);
}

Bitboard SSW(Bitboard moving_piece) {
	// Move bitboard two spaces south and one space west, nullify 2 top and 1 right
    return moving_piece >> 17 & not(FILE_H | RANK_7 | RANK_8);
}

// ==============================================================
// 					========= PIECES ==========
// ==============================================================

// ========== PAWN ===========

// Retrieves all of the pawns on the board as a bitboard
Bitboard getPawns(int board[]) { return getPieces(board, PAWN); }

// Moves pawn one space forward
Bitboard pawnSimplePushes(Bitboard moving_piece, int board[], char color) {
	switch(color) {
	case WHITE:
		// Bitwise and between the piece movement and empty squares
		return north(moving_piece) & getEmptySquares(board);
	case BLACK:
		// Bitwise and between the piece movement and empty squares
		return south(moving_piece) & getEmptySquares(board);
	}
	return 0;
}

// Moves pawn 2 spaces forward
Bitboard pawnDoublePushes(Bitboard moving_piece, int board[], char color) {
	switch(color) {
	case WHITE:
		// Bitwise and between the piece movement and empty squares
		return north(pawnSimplePushes(moving_piece, board, color)) & (getEmptySquares(board) & RANK_4);
	case BLACK:
		// Bitwise and between the piece movement and empty squares
		return south(pawnSimplePushes(moving_piece, board, color)) & (getEmptySquares(board) & RANK_5);
	}
	return 0;
}

// Returns bitwise or of moving pawn one or two space forward
Bitboard pawnPushes(Bitboard moving_piece, int board[], char color) {
	return pawnSimplePushes(moving_piece, board, color) | pawnDoublePushes(moving_piece, board, color);
}

// Attacks with pawn moving east
Bitboard pawnEastAttacks(Bitboard moving_piece, int board[], char color) {
	switch(color) {
	case WHITE:
		// White comes from south, therefore moves northeast
        return NE(moving_piece);
	case BLACK:
		// Black comes from north, therefore moves southeast
        return SE(moving_piece);
	}
	return 0;
}

// Attacks with pawn moving west
Bitboard pawnWestAttacks(Bitboard moving_piece, int board[], char color) {
	switch(color) {
	case WHITE:
		// White comes from south, therefore moves northeast
        return NW(moving_piece);
	case BLACK:
		// Black comes from north, therefore moves southeast
        return SW(moving_piece);
	}
	return 0;
}

// Returns bitwise or of attacking east and west with pawn
Bitboard pawnAttacks(Bitboard moving_piece, int board[], char color) {
	return pawnEastAttacks(moving_piece, board, color) | pawnWestAttacks(moving_piece, board, color);
}

// Captures with pawn
Bitboard pawnSimpleCaptures(Bitboard moving_piece, int board[], char color) {
	// Bitwise and between the pawn movement and the retrieval of colored pieces from the board
	return pawnAttacks(moving_piece, board, color) & getColoredPieces(board, opponent(color));
}

// Checks to see if the "En passant" maneuver is a valid move
Bitboard pawnEpCaptures(Bitboard moving_piece, Position * position, char color) {
	// If the en passant square does not exist, return 0
	if (position->epSquare == -1)
		return 0;

	Bitboard valid_ep_square = 0;

	// Get the valid en passant move as a bitboard
	switch(color) {
	case WHITE:
		valid_ep_square = index2bb(position->epSquare) & RANK_6;
		break;
	case BLACK:
		valid_ep_square = index2bb(position->epSquare) & RANK_3;
		break;
	}

	// Return the pawn attack bitwise anded with the en passant move
	return pawnAttacks(moving_piece, position->board, color) & valid_ep_square;
}

// Returns the simple capture bitwise or'ed with the en passant capture
Bitboard pawnCaptures(Bitboard moving_piece, Position * position, char color) {
    return pawnSimpleCaptures(moving_piece, position->board, color) | pawnEpCaptures(moving_piece, position, color);
}

// Returns the bitwise or of capturing or moving forward with a pawn
Bitboard pawnMoves(Bitboard moving_piece, Position * position, char color) {
	return pawnPushes(moving_piece, position->board, color) | pawnCaptures(moving_piece, position, color);
}

// Checks to see if a pawn is moving 2 spaces forward
BOOL isDoublePush(int leaving, int arriving) {
	if ( (index2bb(leaving)&RANK_2) && (index2bb(arriving)&RANK_4) )
		return TRUE;
	if ( (index2bb(leaving)&RANK_7) && (index2bb(arriving)&RANK_5) )
		return TRUE;
	return FALSE;
}

// Retrieves the square at which an en passant move is valid
char getEpSquare(int leaving) {
	// If the pawn is leaving rank 2, then return the square after the square that was left
	if (index2bb(leaving)&RANK_2)
		return leaving+8;
	// If the pawn was leaving rank 7, return the square before the square that was left
	if (index2bb(leaving)&RANK_7)
		return leaving-8;
	return -1;
}

// Checks to see if two same-colored pawns are on the same file (i.e. one behind another)
BOOL isDoubledPawn(Bitboard position, int board[]) {
	char pieceColor = board[bb2index(position)]&COLOR_MASK;

	if (countPieces( getPawns(board)&getColoredPieces(board, pieceColor)&fileFilter(position) ) > 1)
		return TRUE;
	return FALSE;
}

// Checks to see if a pawn has no friendly pawn on an adjacent file
BOOL isIsolatedPawn(Bitboard position, int board[]) {
	Bitboard sideFiles = fileFilter(east(position) | west(position));
	char pieceColor = board[bb2index(position)]&COLOR_MASK;

	if (countPieces( getPawns(board)&getColoredPieces(board, pieceColor)&sideFiles ) == 0)
		return TRUE;
	return FALSE;
}

// Checks to see if a pawn is behind all same-color pawns on the adjacent files
BOOL isBackwardsPawn(Bitboard position, int board[]) {
	// Fill all elements to the sides of the current position
	Bitboard squaresFilter = east(position) | west(position);
	char pieceColor = board[bb2index(position)]&COLOR_MASK;

	// Fill squaresFilter with a ray going towards the opponent from the current position
	if ( pieceColor == BLACK ) {
		squaresFilter |= northRay(squaresFilter);
	} else {
		squaresFilter |= southRay(squaresFilter);
	}

	// If no same-colored pieces are in squaresFilter, then the pawn is backwards
	if (countPieces( getPawns(board)&getColoredPieces(board, pieceColor)&squaresFilter ) == 0)
		return TRUE;
	return FALSE;
}

// Checks to see if a pawn has no opposing pawns blocking it from reaching the other side of the board
BOOL isPassedPawn(Bitboard position, int board[]) {
	Bitboard squaresFilter = 0;
	char pieceColor = board[bb2index(position)]&COLOR_MASK;

	// Fill a 3 wide ray going towards the opponent from the current position
	if ( pieceColor == BLACK ) {
		squaresFilter |= southRay(east(position)) | southRay(west(position)) | southRay(position);
	} else {
		squaresFilter |= northRay(east(position)) | northRay(west(position)) | northRay(position);
	}

	// If no opposite colored pawns are in that ray, the pawn is not opposed
	if (countPieces( getPawns(board)&getColoredPieces(board, opponent(pieceColor))&squaresFilter ) == 0)
		return TRUE;
	return FALSE;
}

// Checks to see if there are no pawns on a given file
BOOL isOpenFile(Bitboard position, int board[]) {
	if (countPieces( getPawns(board)&fileFilter(position) ) == 0)
		return TRUE;
	return FALSE;
}

// Checks to see if there is only one pawn on a given file
BOOL isSemiOpenFile(Bitboard position, int board[]) {
	if (countPieces( getPawns(board)&fileFilter(position) ) == 1)
		return TRUE;
	return FALSE;
}

// ========== KNIGHT =========

// Retrieves all knights on the board
Bitboard getKnights(int board[]) { return getPieces(board, KNIGHT); }

// Returns a bitboard containing all of the valid knight attacks from the current location
Bitboard knightAttacks(Bitboard moving_piece) {
    return NNE(moving_piece) | ENE(moving_piece) |
           NNW(moving_piece) | WNW(moving_piece) |
           SSE(moving_piece) | ESE(moving_piece) |
           SSW(moving_piece) | WSW(moving_piece);
}

// Returns all of the valid attacks that do not already have a same-colored piece in them
Bitboard knightMoves(Bitboard moving_piece, int board[], char color) {
    return knightAttacks(moving_piece) & not(getColoredPieces(board, color));
}

// Fills all possible squares a knight can hit within a certain number of jumps
Bitboard knightFill(Bitboard moving_piece, int jumps) {
	// Start with moving piece
	Bitboard fill = moving_piece;
	int i;
	// For i < jumps
	for (i=0; i<jumps; i++) {
		// Find knight attacks from current fill, bitwise or the result onto fill
		fill |= knightAttacks(fill);
	}
	return fill;
}

// Calculates how many moves it will take to reach a square from the current square
int knightDistance(Bitboard leaving_square, Bitboard arriving_square) {
	Bitboard fill = leaving_square;
	int dist = 0;

	// While not at the arriving square
	while ((fill & arriving_square) == 0) {
		// Increment distance (number of moves)
		dist++;
		// Add knightAttacks at fill to fill
		fill |= knightAttacks(fill);
	}
	return dist;
}

// ========== KING ===========

// Gets the location of the king at a given color
Bitboard getKing(int board[], char color) {
	return getPieces(board, KING) & getColoredPieces(board, color);
}

// Return a bitboard of all king attacks
Bitboard kingAttacks(Bitboard moving_piece) {
	// Fill east and west
	Bitboard kingAtks = moving_piece | east(moving_piece) | west(moving_piece);
	// Fill north and south from both
	kingAtks |= north(kingAtks) | south(kingAtks);
	// Return with center piece removed
    return kingAtks & not(moving_piece);
}

// Return all king attacks that are not occupied by a same-colored piece
Bitboard kingMoves(Bitboard moving_piece, int board[], char color) {
    return kingAttacks(moving_piece) & not(getColoredPieces(board, color));
}

// Checks if castling kingside is valid
BOOL canCastleKingside(Position * position, char color) {
	switch(color) {

	case WHITE:
		// If can castle kingside, all of the pieces are in the right spot, 
		// none of the spaces are under attack, then return true
		if ( (position->castlingRights&CASTLE_KINGSIDE_WHITE) &&
			 (position->board[str2index("e1")] == (WHITE|KING)) &&
			 (position->board[str2index("f1")] == EMPTY) &&
			 (position->board[str2index("g1")] == EMPTY) &&
			 (position->board[str2index("h1")] == (WHITE|ROOK)) &&
			 (!isAttacked(str2bb("e1"), position->board, opponent(color))) &&
			 (!isAttacked(str2bb("f1"), position->board, opponent(color))) &&
			 (!isAttacked(str2bb("g1"), position->board, opponent(color))) )
			return TRUE;
		else
			return FALSE;

	case BLACK:
		// If can castle kingside, all of the pieces are in the right spot, 
		// none of the spaces are under attack, then return true
		if ( (position->castlingRights&CASTLE_KINGSIDE_BLACK) &&
			 (position->board[str2index("e8")] == (BLACK|KING)) &&
			 (position->board[str2index("f8")] == EMPTY) &&
			 (position->board[str2index("g8")] == EMPTY) &&
			 (position->board[str2index("h8")] == (BLACK|ROOK)) &&
			 (!isAttacked(str2bb("e8"), position->board, opponent(color))) &&
			 (!isAttacked(str2bb("f8"), position->board, opponent(color))) &&
			 (!isAttacked(str2bb("g8"), position->board, opponent(color))) )
			return TRUE;
		else
			return FALSE;

	}
	return FALSE;
}

// Checks if castling queenside is valid
BOOL canCastleQueenside(Position * position, char color) {
	switch(color) {

	case WHITE:
		// If can castle queenside, all of the pieces are in the right spot, 
		// none of the spaces are under attack, then return true
		if ( (position->castlingRights&CASTLE_QUEENSIDE_WHITE) &&
			 (position->board[str2index("a1")] == (WHITE|ROOK)) &&
			 (position->board[str2index("b1")] == EMPTY) &&
			 (position->board[str2index("c1")] == EMPTY) &&
			 (position->board[str2index("d1")] == EMPTY) &&
			 (position->board[str2index("e1")] == (WHITE|KING)) &&
			 (!isAttacked(str2bb("c1"), position->board, opponent(color))) &&
			 (!isAttacked(str2bb("d1"), position->board, opponent(color))) &&
			 (!isAttacked(str2bb("e1"), position->board, opponent(color))) )
			return TRUE;
		else
			return FALSE;

	case BLACK:
		// If can castle queenside, all of the pieces are in the right spot, 
		// none of the spaces are under attack, then return true
		if ( (position->castlingRights&CASTLE_QUEENSIDE_BLACK) &&
				 (position->board[str2index("a8")] == (BLACK|ROOK)) &&
				 (position->board[str2index("b8")] == EMPTY) &&
				 (position->board[str2index("c8")] == EMPTY) &&
				 (position->board[str2index("d8")] == EMPTY) &&
				 (position->board[str2index("e8")] == (BLACK|KING)) &&
				 (!isAttacked(str2bb("c8"), position->board, opponent(color))) &&
				 (!isAttacked(str2bb("d8"), position->board, opponent(color))) &&
				 (!isAttacked(str2bb("e8"), position->board, opponent(color))) )
			return TRUE;
		else
			return FALSE;

	}
	return FALSE;
}

// Removes the right to castle from a piece
char removeCastlingRights(char original_rights, char removed_rights) {
	// Flips bits of removed rights, bitwise and's it onto the original
    return (char) (original_rights & ~(removed_rights));
}

// ========== BISHOP =========

// Gets all bishops on the board
Bitboard getBishops(int board[]) { return getPieces(board, BISHOP); }

// Generates a northeastern attack ray from all elements on the passed in bitboard
Bitboard NE_ray(Bitboard bb) {
	int i;
	// Add first NE ray
	Bitboard ray = NE(bb);

	// Add remaining NE rays starting from current ray
	for (i=0; i<6; i++) {
		ray |= NE(ray);
	}

	return ray & ALL_SQUARES;
}

// Generates southeastern attack ray from all elements on the passed in bitboard
Bitboard SE_ray(Bitboard bb) {
	int i;
	// Add first SE ray
	Bitboard ray = SE(bb);

	// Add remaining SE rays
	for (i=0; i<6; i++) {
		ray |= SE(ray);
	}

	return ray & ALL_SQUARES;
}

// Generates northwestern attack ray from all elements on the passed in bitboard
Bitboard NW_ray(Bitboard bb) {
	int i;
	// Add first NW ray
	Bitboard ray = NW(bb);

	// Add remaining NW rays
	for (i=0; i<6; i++) {
		ray |= NW(ray);
	}

	return ray & ALL_SQUARES;
}

// Generates southwestern attack ray from all elements on the passed in bitboard
Bitboard SW_ray(Bitboard bb) {
	int i;
	// Add first SW ray
	Bitboard ray = SW(bb);

	// Add remaining SW rays
	for (i=0; i<6; i++) {
		ray |= SW(ray);
	}

	return ray & ALL_SQUARES;
}

// Generates northeastern attack ray from the location of the piece
Bitboard NE_attack(Bitboard single_piece, int board[], char color) {
	// Find pieces blocking the attack
	Bitboard blocker = lsb(NE_ray(single_piece) & getOccupiedSquares(board));

	// If there is a blocker, return the ray of the piece xor'd with the ray of the blocker
	if (blocker) {
		return NE_ray(single_piece) ^ NE_ray(blocker);
	} else {
		return NE_ray(single_piece);
	}
}

// Generates northwestern attack ray from the location of the piece
Bitboard NW_attack(Bitboard single_piece, int board[], char color) {
	// Find pieces blocking the attack
	Bitboard blocker = lsb(NW_ray(single_piece) & getOccupiedSquares(board));

	// If there is a blocker, return the ray of the piece xor'd with the ray of the blocker
	if (blocker) {
		return NW_ray(single_piece) ^ NW_ray(blocker);
	} else {
		return NW_ray(single_piece);
	}
}

// Generates southeastern attack ray from the location of the piece
Bitboard SE_attack(Bitboard single_piece, int board[], char color) {
	// Find pieces blocking the attack
	Bitboard blocker = msb(SE_ray(single_piece) & getOccupiedSquares(board));

	// If there is a blocker, return the ray of the piece xor'd with the ray of the blocker
	if (blocker) {
		return SE_ray(single_piece) ^ SE_ray(blocker);
	} else {
		return SE_ray(single_piece);
	}
}

// Generates southwestern attack ray from the location of the piece
Bitboard SW_attack(Bitboard single_piece, int board[], char color) {
	// Find pieces blocking the attack
	Bitboard blocker = msb(SW_ray(single_piece) & getOccupiedSquares(board));

	// If there is a blocker, return the ray of the piece xor'd with the ray of the blocker
	if (blocker) {
		return SW_ray(single_piece) ^ SW_ray(blocker);
	} else {
		return SW_ray(single_piece);
	}
}

// Return attack vectors along the sw-ne vector
Bitboard diagonalAttacks(Bitboard single_piece, int board[], char color) {
    return NE_attack(single_piece, board, color) | SW_attack(single_piece, board, color);
}

// Return attack vectors along the nw-se vector
Bitboard antiDiagonalAttacks(Bitboard single_piece, int board[], char color) {
    return NW_attack(single_piece, board, color) | SE_attack(single_piece, board, color);
}

// Return both diagonal and anti-diagonal attacks
Bitboard bishopAttacks(Bitboard moving_pieces, int board[], char color) {
    return diagonalAttacks(moving_pieces, board, color) | antiDiagonalAttacks(moving_pieces, board, color);
}

// Return all bishop attacks that are not occupied by friendly pieces
Bitboard bishopMoves(Bitboard moving_piece, int board[], char color) {
    return bishopAttacks(moving_piece, board, color) & not(getColoredPieces(board, color));
}

// ========== ROOK ===========

// Find all rooks on the board
Bitboard getRooks(int board[]) { return getPieces(board, ROOK); }

// Generates northern attack ray for all elements on passed-in board
Bitboard northRay(Bitboard moving_pieces) {
	// Add first N ray
    Bitboard ray_atks = north(moving_pieces);

    int i;
	// Add remaining N rays
    for (i=0; i<6; i++) {
        ray_atks |= north(ray_atks);
    }

    return ray_atks & ALL_SQUARES;
}

// Generates southern attack ray for all elements on passed-in board
Bitboard southRay(Bitboard moving_pieces) {
	// Add first S ray
    Bitboard ray_atks = south(moving_pieces);

    int i;
	// Add remaining S rays
    for (i=0; i<6; i++) {
        ray_atks |= south(ray_atks);
    }

    return ray_atks & ALL_SQUARES;
}

// Generates eastern attack ray for all elements on passed-in board
Bitboard eastRay(Bitboard moving_pieces) {
	// Add first E ray
    Bitboard ray_atks = east(moving_pieces);

    int i;
	// Add remaining E rays
    for (i=0; i<6; i++) {
        ray_atks |= east(ray_atks);
    }

    return ray_atks & ALL_SQUARES;
}

// Generates western attack ray for all elements on passed-in board
Bitboard westRay(Bitboard moving_pieces) {
	// Add first W ray
    Bitboard ray_atks = west(moving_pieces);

    int i;
	// Add remaining W rays
    for (i=0; i<6; i++) {
        ray_atks |= west(ray_atks);
    }

    return ray_atks & ALL_SQUARES;
}

// Generates northern attack vector from the location of the piece
Bitboard northAttack(Bitboard single_piece, int board[], char color) {
	// Find pieces blocking the attack
    Bitboard blocker = lsb(northRay(single_piece) & getOccupiedSquares(board));

	// If there is a blocker, return the ray of the piece xor'd with the ray of the blocker
    if (blocker)
        return northRay(single_piece) ^ northRay(blocker);
    else
        return northRay(single_piece);
}

// Generates southern attack vector from the location of the piece
Bitboard southAttack(Bitboard single_piece, int board[], char color) {
	// Find pieces blocking the attack
    Bitboard blocker = msb(southRay(single_piece) & getOccupiedSquares(board));

	// If there is a blocker, return the ray of the piece xor'd with the ray of the blocker
    if (blocker)
        return southRay(single_piece) ^ southRay(blocker);
    else
        return southRay(single_piece);
}

// Returns northern and southern attack vectors
Bitboard fileAttacks(Bitboard single_piece, int board[], char color) {
    return northAttack(single_piece, board, color) | southAttack(single_piece, board, color);
}

// Generates eastern attack vector from the location of the piece
Bitboard eastAttack(Bitboard single_piece, int board[], char color) {
	// Find pieces blocking the attack
    Bitboard blocker = lsb(eastRay(single_piece) & getOccupiedSquares(board));

	// If there is a blocker, return the ray of the piece xor'd with the ray of the blocker
    if (blocker)
        return eastRay(single_piece) ^ eastRay(blocker);
    else
        return eastRay(single_piece);
}

// Generates western attack vector from the location of the piece
Bitboard westAttack(Bitboard single_piece, int board[], char color) {
	// Find pieces blocking the attack
    Bitboard blocker = msb(westRay(single_piece) & getOccupiedSquares(board));

	// If there is a blocker, return the ray of the piece xor'd with the ray of the blocker
    if (blocker)
        return westRay(single_piece) ^ westRay(blocker);
    else
        return westRay(single_piece);
}

// Returns eastern and western attack vectors
Bitboard rankAttacks(Bitboard single_piece, int board[], char color) {
    return eastAttack(single_piece, board, color) | westAttack(single_piece, board, color);
}

// Returns full set of rook attacks (rank and file attack vectors)
Bitboard rookAttacks(Bitboard moving_piece, int board[], char color) {
    return fileAttacks(moving_piece, board, color) | rankAttacks(moving_piece, board, color);
}

// Return all valid attacks that are not occupied by a same-colored piece
Bitboard rookMoves(Bitboard moving_piece, int board[], char color) {
    return rookAttacks(moving_piece, board, color) & not(getColoredPieces(board, color));
}

// ========== QUEEN ==========

// Get all queens on the board
Bitboard getQueens(int board[]) { return getPieces(board, QUEEN); }

// Return queen attacks (combiniation of bishop and rook attacks)
Bitboard queenAttacks(Bitboard moving_piece, int board[], char color) {
    return rookAttacks(moving_piece, board, color) | bishopAttacks(moving_piece, board, color);
}

// Return queen moves (combination of bishop and rook moves)
Bitboard queenMoves(Bitboard moving_piece, int board[], char color) {
    return rookMoves(moving_piece, board, color) | bishopMoves(moving_piece, board, color);
}

// ======== MAKE MOVE ========

// ==== Move Piece ====
void movePiece(int board[], Move move) {
	board[getTo(move)] = board[getFrom(move)]; // Take the selected, valid, position for the piece and give it the information from the current position.
	board[getFrom(move)] = EMPTY; // Take the previous position and clear it of all piece data.
}

// ==== Current position information update ====
void updatePosition(Position * newPosition, Position * position, Move move) {
	memcpy(newPosition, position, sizeof(Position)); // Copy information of selected, valid, position.
	int leavingSquare = getFrom(move); // Get coordinates of position piece is on.
	int arrivingSquare = getTo(move); // Get cordinates of selected position.
	int piece = position->board[leavingSquare]; // Get the piece information that is stored on the position it is leaving.

	// ===== MOVE PIECE =====
	movePiece(newPosition->board, move);

	// ===== TO MOVE =====
	newPosition->toMove = opponent(position->toMove);

	// ===== MOVE COUNTS =====
	// Half move clock takes care of enforcing the fifty-move rule (in this program 75). The count is reset if a capture happens or a pawn moves.
	newPosition->halfmoveClock += 1; // Add one to the count
	if (position->toMove == BLACK) { // If it's black it has negative positions 
		newPosition->fullmoveNumber += 1;
	}

	if (position->board[arrivingSquare] != EMPTY) {
		newPosition->halfmoveClock = 0; // If there is a capture, reset the half move clock
	}

	// ===== PAWNS =====
	newPosition->epSquare = -1;
	if ( (piece&PIECE_MASK) == PAWN ) {
		newPosition->halfmoveClock = 0; //If a pawn moves, reset the half move clock

		if (arrivingSquare == position->epSquare) {
		    if (index2bb(position->epSquare)&RANK_3) {
				newPosition->board[(int)(position->epSquare+8)] = EMPTY;
		    }

		    if (index2bb(position->epSquare)&RANK_6) {
		    	newPosition->board[(int)(position->epSquare-8)] = EMPTY;
		    }
		}

		if (isDoublePush(leavingSquare, arrivingSquare)) {
			newPosition->epSquare = getEpSquare(leavingSquare);
		}

		if (index2bb(arrivingSquare)&(RANK_1|RANK_8)) {
			newPosition->board[arrivingSquare] = position->toMove|QUEEN;
		}

	}

	// ===== CASTLING =====
	// Castling rules: 
	// The kind and the rook may not have moved from their starting position if you want to castle
	// All spaces between the king and the rook must be empty
	// The king cannot be in check
	// The squares that the king passes over myst not be under atack, nor the square where it lands on
	if (leavingSquare == str2index("a1")) {
		newPosition->castlingRights = removeCastlingRights(newPosition->castlingRights, CASTLE_QUEENSIDE_WHITE);
	}
	else if (leavingSquare == str2index("h1")) {
		newPosition->castlingRights = removeCastlingRights(newPosition->castlingRights, CASTLE_KINGSIDE_WHITE);
	}
	else if (leavingSquare == str2index("a8")) {
		newPosition->castlingRights = removeCastlingRights(newPosition->castlingRights, CASTLE_QUEENSIDE_BLACK);
	}
	else if (leavingSquare == str2index("h8")) {
		newPosition->castlingRights = removeCastlingRights(newPosition->castlingRights, CASTLE_KINGSIDE_BLACK);
	}

	if ( piece == (WHITE|KING) ) {
		newPosition->castlingRights = removeCastlingRights(newPosition->castlingRights, (CASTLE_KINGSIDE_WHITE|CASTLE_QUEENSIDE_WHITE));
		if (leavingSquare == str2index("e1")) {

			if (arrivingSquare == str2index("g1"))
				movePiece(newPosition->board, generateMove(str2index("h1"), str2index("f1")));

            if (arrivingSquare == str2index("c1"))
				movePiece(newPosition->board, generateMove(str2index("a1"), str2index("d1")));
		}
	} else if ( piece == (BLACK|KING) ) {
		newPosition->castlingRights = removeCastlingRights(newPosition->castlingRights, CASTLE_KINGSIDE_BLACK|CASTLE_QUEENSIDE_BLACK);
		if (leavingSquare == str2index("e8")) {

			if (arrivingSquare == str2index("g8"))
				movePiece(newPosition->board, generateMove(str2index("h8"), str2index("f8")));

            if (arrivingSquare == str2index("c8"))
				movePiece(newPosition->board, generateMove(str2index("a8"), str2index("d8")));
		}
	}
}

// ==== Another move function ====
void makeMove(Game * game, Move move) {
	Position newPosition;
	updatePosition(&newPosition, &(game->position), move); // Move the piece to the next position selected
	memcpy(&(game->position), &newPosition, sizeof(Position)); // Copy the memory of the position onto the next position

	game->moveListLen += 1; // List of moves made increases

	// ===== MOVE LIST =====
	game->moveList[game->moveListLen-1] = move;

	// ===== POSITION HISTORY =====
	toFen(game->positionHistory[game->moveListLen], &game->position); // Stores each move in "position history"
}

// ==== Unmake move (for undo function) ====
void unmakeMove(Game * game) {
	Position newPosition;
	if (game->moveListLen >= 1) { // If the user has made at least one move
			loadFen(&newPosition, game->positionHistory[game->moveListLen-1]); // Get the games move history
			memcpy(&(game->position), &newPosition, sizeof(Position)); // Get the last move made 

			game->moveList[game->moveListLen-1] = 0; // Remove that last move from the list of moves
			memset(game->positionHistory[game->moveListLen], 0, MAX_FEN_LEN*sizeof(char)); // Make the size of the list the size of the new list

			game->moveListLen -= 1; // Make the list length one less
		} else { // return to initial game
			loadFen(&newPosition, game->positionHistory[0]); // Else there is no moves to undo
			memcpy(&(game->position), &newPosition, sizeof(Position));

			game->moveListLen = 0; // The list is size 0
			memset(game->moveList, 0, MAX_PLYS_PER_GAME*sizeof(int));
			memset(&game->positionHistory[1], 0, (MAX_PLYS_PER_GAME-1)*MAX_FEN_LEN*sizeof(char));
		}
}

// ======== MOVE GEN =========
// ==== Makes a list of moves ====
Bitboard getMoves(Bitboard movingPiece, Position * position, char color) {
	int piece = position->board[bb2index(movingPiece)] & PIECE_MASK;

	switch (piece) {
	case PAWN:
		return pawnMoves(movingPiece, position, color); // List pawn moves and info
	case KNIGHT:
		return knightMoves(movingPiece, position->board, color); // List knight moves and info
	case BISHOP:
		return bishopMoves(movingPiece, position->board, color); // List bishop moves and info
	case ROOK:
		return rookMoves(movingPiece, position->board, color); // List rook moves and info
	case QUEEN:
		return queenMoves(movingPiece, position->board, color); // List queen moves and info 
	case KING:
		return kingMoves(movingPiece, position->board, color); // List king moves and info
	}
	return 0;
}

// List of legal moves
int pseudoLegalMoves(Move * moves, Position * position, char color) {
	int leavingSquare, arrivingSquare, moveCount = 0;

	for (leavingSquare=0; leavingSquare<NUM_SQUARES; leavingSquare++) {
		int piece = position->board[leavingSquare]; // Get the position of the square the piece is leaving

		if (piece != EMPTY && (piece&COLOR_MASK) == color) { // Get the peices information and the colour of it
			Bitboard targets = getMoves(index2bb(leavingSquare), position, color);

			for (arrivingSquare=0; arrivingSquare<NUM_SQUARES; arrivingSquare++) {
				if (isSet(targets, arrivingSquare)) {
					moves[moveCount++] = generateMove(leavingSquare, arrivingSquare);
				}
			}

			if ( (piece&PIECE_MASK) == KING ) { // If the piece is a king
				if (canCastleKingside(position, color)) {
					moves[moveCount++] = generateMove(leavingSquare, leavingSquare+2);
				}
				if (canCastleQueenside(position, color)) {
					moves[moveCount++] = generateMove(leavingSquare, leavingSquare-2);
				}
			}
		}
	}

	return moveCount;
}

// ==== Get a list of possible attacks for each piece ==== 
Bitboard getAttacks(Bitboard movingPiece, int board[], char color) {
	int piece = board[bb2index(movingPiece)] & PIECE_MASK;

	switch (piece) {
	case PAWN:
		return pawnAttacks(movingPiece, board, color); // Get the pawns information
	case KNIGHT:
		return knightAttacks(movingPiece); // Get the knights information
	case BISHOP:
		return bishopAttacks(movingPiece, board, color); // Get the bishops information
	case ROOK:
		return rookAttacks(movingPiece, board, color); // Get the rooks information
	case QUEEN:
		return queenAttacks(movingPiece, board, color); // Get the queens information
	case KING:
		return kingAttacks(movingPiece); // Get the kings information
	}
	return 0;
}

// ==== Get the number of attacks ====
int countAttacks(Bitboard target, int board[], char color) {
	int i, attackCount = 0;

	for (i=0; i<NUM_SQUARES; i++)
		if (board[i] != EMPTY && (board[i]&COLOR_MASK) == color)
			if ( getAttacks(index2bb(i), board, color) & target )
				attackCount += 1; // Add one to the attack count

	return attackCount;
}

// ==== Was the piece attacked ====
BOOL isAttacked(Bitboard target, int board[], char color) {
	if (countAttacks(target, board, color) > 0) // Get the pieces attack information
		return TRUE; // If it was attacked return true
	else
		return FALSE; // If it wasn't attacked return false
}

// ==== Checks if it's check ====
BOOL isCheck(int board[], char color) {
 	return isAttacked(getKing(board, color), board, opponent(color)); // Return if the king can be attacked?
}

// ==== Checks if the move is the legal ====
BOOL isLegalMove(Position * position, Move move) {
	Position newPosition;
	updatePosition(&newPosition, position, move); // Gets selected position
	if ( isCheck(newPosition.board, position->toMove) ) // Did the user move the king into a spot where it will be attacked.
		return FALSE;
	return TRUE;
}

// ==== Returns how many legal moves there are ====
int legalMoves(Move * legalMoves, Position * position, char color) {
	int i, legalCount = 0;

	Move pseudoMoves[MAX_BRANCHING_FACTOR];
	int pseudoCount = pseudoLegalMoves(pseudoMoves, position, color); // Get the number of pseudo legal moves

	for (i=0; i<pseudoCount; i++) { 
		if (isLegalMove(position, pseudoMoves[i])) { //For all of the moves, if the move is legal, increase the count by one
			legalMoves[legalCount++] = pseudoMoves[i];
		}
	}

	return legalCount;
}

// ==== Count of legal moves ====
int legalMovesCount(Position * position, char color) {
	int i, legalCount = 0;

	Move pseudoMoves[MAX_BRANCHING_FACTOR];
	int pseudoCount = pseudoLegalMoves(pseudoMoves, position, color); // Get list of psuedo legal moves 

	for (i=0; i<pseudoCount; i++) {
		if (isLegalMove(position, pseudoMoves[i])) { // For that number of moves, if it is a legal move, increment by one
			legalCount++;
		}
	}

	return legalCount;
}

// ==== List the legal moves in static order ==== 
int staticOrderLegalMoves(Move * orderedLegalMoves, Position * position, char color) {
	Move moves[MAX_BRANCHING_FACTOR];
	int legalCount = legalMoves(moves, position, color); // Get the count of moves from a piece

	Position newPosition;
	Node nodes[legalCount], orderedNodes[legalCount];

	int i;
	for (i=0; i<legalCount; i++) {
		updatePosition(&newPosition, position, moves[i]); // Make the move
		nodes[i] = (Node) { .move = moves[i], .score = staticEvaluation(&newPosition) }; // Staticaly evaluate the new move
	}

	sortNodes(orderedNodes, nodes, legalCount, color); // Sort the nodes (moves) in the list

	for (i=0; i<legalCount; i++) {
		orderedLegalMoves[i] = orderedNodes[i].move; // Reorder the legal moves into a sorted list
	}

	return legalCount;
}

// ==== Let a list of legal captures ====
int legalCaptures(Move * legalCaptures, Position * position, char color) {
	int i, captureCount = 0;

	Move moves[MAX_BRANCHING_FACTOR];
	int legalCount = legalMoves(moves, position, color); // Get the number of legal moves

	for (i=0; i<legalCount; i++) {
		int arrivingSquare = getTo(moves[i]); // Get information for selected position
		if ( index2bb(arrivingSquare) & getColoredPieces(position->board, opponent(color)) ) {
			legalCaptures[captureCount++] = moves[i]; // If the piece on the board is a valid piece to take (oposite colour) then increase the count of legal caputres by one
		}
	}

	return captureCount;
}

// ====== GAME CONTROL =======

// ==== Determines if it is checkmate ====
BOOL isCheckmate(Position * position) {
	// if is in check and has no legal moves
	if (isCheck(position->board, position->toMove) && legalMovesCount(position, position->toMove) == 0)
		return TRUE;
	else
		return FALSE;
}

// ==== Determines if it is a stalemate ====
BOOL isStalemate(Position * position) {
	// if no legal moves but not in check
	if (!isCheck(position->board, position->toMove) && legalMovesCount(position, position->toMove) == 0)
		return TRUE;
	else
		return FALSE;
}

// ==== Determines if there is insufficient material (neither player can checkmate) ====
BOOL hasInsufficientMaterial(int board[]) {
	int pieceCount = countBits(getOccupiedSquares(board));

	if ( pieceCount <= 3 ) {
		if ( pieceCount == 2 || getKnights(board) != 0 || getBishops(board) != 0 )
			return TRUE;
	}

	return FALSE;
}

// ==== Determines if it is the endgame ====
BOOL isEndgame(int board[]) {
	if (countBits(getOccupiedSquares(board)) <= ENDGAME_PIECE_COUNT)
		return TRUE;
	return FALSE;
}

// ==== Determines if the move limit has been reached ====
BOOL isOver75MovesRule(Position * position) {
	if (position->halfmoveClock >= 150)
		return TRUE;
	else
		return FALSE;
}

// ==== Determines if the game has ended ====
BOOL hasGameEnded(Position * position) {
	// checkmate, stalemate, insufficient material, or move limit reached
	if ( isCheckmate(position) ||
		 isStalemate(position) ||
		 hasInsufficientMaterial(position->board) ||
		 isOver75MovesRule(position) )
		return TRUE;
	else
		return FALSE;
}

// ==== Prints the outcome of the game ====
void printOutcome(Position * position) {
	// Black in checkmate
	if (isCheckmate(position) && position->toMove == BLACK)
		printf("WHITE wins!\n");
	// White in checkmate
	if (isCheckmate(position) && position->toMove == WHITE)
		printf("BLACK wins!\n");
	// Draw: Stalemate
	if (isStalemate(position))
		printf("Draw by stalemate!\n");
	// Draw: Insufficient Material
	if (hasInsufficientMaterial(position->board))
		printf("Draw by insufficient material!\n");
	// Draw: Move limit reached
	if ( isOver75MovesRule(position) )
		printf("Draw by 75 move rule!\n");
	fflush(stdout);
}

// ========== EVAL ===========

// ==== Returns the win score for specified color ====
int winScore(char color) {
	if (color == WHITE)
		return 10*PIECE_VALUES[KING];
	if (color == BLACK)
		return -10*PIECE_VALUES[KING];
	return 0;
}

// ==== Returns the material total for specified color ====
int materialSum(int board[], char color) {
	int i, material = 0;

	// goes through board
	for (i=0; i<NUM_SQUARES; i++) {
		// if space is occupied by a piece of specified color
		if (board[i] != EMPTY && (board[i]&COLOR_MASK) == color) {
			// add piece value to material
			material += PIECE_VALUES[board[i]&PIECE_MASK];
		}
	}

	return material;
}

// ==== Returns the difference of the colors' material sum ====
int materialBalance(int board[]) {
	return materialSum(board, WHITE) - materialSum(board, BLACK);
}

// ==== Returns positional bonus for specified color
int positionalBonus(int board[], char color) {
	int bonus = 0;

	int i;
	for (i=0; i<NUM_SQUARES; i++) {
		int piece = board[i];

		// if space is occupied by piece of specified color
		if (piece != EMPTY && (piece&COLOR_MASK) == color) {
			int pieceType = piece&PIECE_MASK;

			// Switch statement to handle each piece and their corresponding bonuses
			switch(pieceType) {
			case PAWN:
				if (color == WHITE) {
					bonus += PAWN_BONUS[i];
				} else {
					bonus += PAWN_BONUS[FLIP_VERTICAL[i]];
				}

				if (isDoubledPawn(index2bb(i), board)) {
					bonus -= DOUBLED_PAWN_PENALTY/2;
				}
				if (isPassedPawn(index2bb(i), board)) {
					bonus += PASSED_PAWN_BONUS;
				}

				if (isIsolatedPawn(index2bb(i), board)) {
					bonus -= ISOLATED_PAWN_PENALTY;
				} else if (isBackwardsPawn(index2bb(i), board)) {
					bonus -= BACKWARDS_PAWN_PENALTY;
				}

				break;

			case KNIGHT:
				if (color == WHITE) {
					bonus += KNIGHT_BONUS[i];
				} else {
					bonus += KNIGHT_BONUS[FLIP_VERTICAL[i]];
				}
				break;

			case BISHOP:
				if (color == WHITE) {
					bonus += BISHOP_BONUS[i];
				} else {
					bonus += BISHOP_BONUS[FLIP_VERTICAL[i]];
				}
				break;

			case ROOK:
                		if (isOpenFile(index2bb(i), board)) {
                    			bonus += ROOK_OPEN_FILE_BONUS;
                		} else if (isSemiOpenFile(index2bb(i), board)) {
                    			bonus += ROOK_SEMI_OPEN_FILE_BONUS;
                		}

				if (color == WHITE) {
					if (index2bb(i) & RANK_7) {
						bonus += ROOK_ON_SEVENTH_BONUS;
					}
				} else {
					if (index2bb(i) & RANK_2) {
						bonus += ROOK_ON_SEVENTH_BONUS;
					}
				}
				break;

			case KING:
				if (isEndgame(board)) {
					if (color == WHITE) {
						bonus += KING_ENDGAME_BONUS[i];
					} else {
						bonus += KING_ENDGAME_BONUS[FLIP_VERTICAL[i]];
					}
				} else {
					if (color == WHITE) {
						bonus += KING_BONUS[i];
					} else {
						bonus += KING_BONUS[FLIP_VERTICAL[i]];
					}
				}
			}
		}
	}

	return bonus;
}

// ==== Returns the difference of each colors' positional bonus ====
int positionalBalance(int board[]) {
	return positionalBonus(board, WHITE) - positionalBonus(board, BLACK);
}

// ==== Returns the value of the position at the end of the game ==== 
int endNodeEvaluation(Position * position) {
	// If it is checkmate, return the winning score
	if (isCheckmate(position)) {
		return winScore(opponent(position->toMove));
	}
	// If it is in stalemate or will likely be a stalemate, return 0
	if (isStalemate(position) || hasInsufficientMaterial(position->board) || isOver75MovesRule(position)) {
		return 0;
	}
	// Else, return 0
	return 0;
}

// ==== Returns the value of the current position ====
int staticEvaluation(Position * position) {
	// If the game is over, end node evaluation
	if (hasGameEnded(position))
		return endNodeEvaluation(position);
	// Else return a combination of the material and positional balances
	// to figure out who has the upper hand
	else
		return materialBalance(position->board) + positionalBalance(position->board);
}

// ==== Returns count of moves that end in capture from position to target square ====
// ==== Stores them in captures ====
int getCaptureSequence(Move * captures, Position * position, int targetSquare) {
	Move allCaptures[MAX_BRANCHING_FACTOR], targetCaptures[MAX_ATTACKING_PIECES];
	int captureCount = legalCaptures(allCaptures, position, position->toMove);	// Gets all legal captures from position
	int i, j, targetCount = 0;

	// For all legal captures
	for (i=0; i<captureCount; i++) {
		// If the end position is the target square
		if ( getTo(allCaptures[i]) == targetSquare ) {
			// Add capture to target captures and increase target count
			targetCaptures[targetCount++] = allCaptures[i];
		}
	}

	Move captureBuffer[targetCount];

	// Order the moves in order of increasing value
	BOOL sorted;
	// Insertion sort of targetCount
	for (i=0; i<targetCount; i++) {
		sorted = FALSE;
		// Get integer representation of piece from targetcaptures[i]
		int piece = position->board[getFrom(targetCaptures[i])] & PIECE_MASK;

		for (j=0; j<i; j++) {
			// Get integer representation of piece from captures[j]
			int sortedPiece = position->board[getFrom(captures[j])] & PIECE_MASK;

			// If the piece at target captures is less than the piece at captures
			if ( PIECE_VALUES[piece] < PIECE_VALUES[sortedPiece] ) {
				// Move captures[j] over 1 spot
				sorted = TRUE;
				memcpy(captureBuffer, &captures[j], (i-j)*sizeof(Move));
				memcpy(&captures[j+1], captureBuffer, (i-j)*sizeof(Move));
				// Move targetCaptures[i] to captures[j]
				captures[j] = targetCaptures[i];
				break;
			}
		}

		// Else, insert at end of iteration
		if ( sorted == FALSE ) {
			captures[i] = targetCaptures[i];
		}
	}

	return targetCount;
}

// Evaluates the long-term cost of taking a piece
// Used for deciding if a trade is bad in the long term
int staticExchangeEvaluation(Position * position, int targetSquare) {
	Move captures[MAX_ATTACKING_PIECES];
	// Get sorted list of possible captures
	int attackCount = getCaptureSequence(captures, position, targetSquare);
	int value = 0;

	// If the amount of possible captures is greater than 0
	if ( attackCount > 0 ) {
		Position newPosition;
		// Simulate move
		updatePosition(&newPosition, position, captures[0]);
		// Get value of the captured piece
		int capturedPiece = position->board[targetSquare] & PIECE_MASK;
		int pieceValue = PIECE_VALUES[capturedPiece];
		// Get overall value by subtracting the capture evaluation of the updated position from the value of the capture
		value = pieceValue - staticExchangeEvaluation(&newPosition, targetSquare);
	}

	return value>0?value:0;
}

// More extensive value evaluation, done by checking all possible captures after a move
// and seeing if any of the captures will cause the move to be a poor trade
int quiescenceEvaluation(Position * position) {
	// Evaluate current position's value statically
	int staticScore = staticEvaluation(position);

	// If the game is over in this position, return the current score
	if (hasGameEnded(position))
		return staticScore;

	// Get number of possible captures, and fill captures with all possible captures
	Move captures[MAX_BRANCHING_FACTOR];
	int captureCount = legalCaptures(captures, position, position->toMove);

	// If there are no possible captures, return the static score
	if (captureCount == 0) {
		return staticScore;
	} else {
		Position newPosition;
		int i, bestScore = staticScore;

		for (i=0; i<captureCount; i++) {
			// If the current position is a poor exchange, break the loop
			if (staticExchangeEvaluation(position, getTo(captures[i])) <= 0)
				break;

			// Simulate capture
			updatePosition(&newPosition, position, captures[i]);
			// Recursively calculate value of this position
			int score = quiescenceEvaluation(&newPosition);

			// If score is better than the current best, update the best score
			if ( (position->toMove == WHITE && score > bestScore) ||
				 (position->toMove == BLACK && score < bestScore) ) {
				bestScore = score;
			}
		}

		return bestScore;
	}
}

// ==============================================================
// 					========= SEARCH ==========
// ==============================================================

// ==== Perfroms a static search ====
Node staticSearch(Position * position) {
	// Initialize best score to max values before iteration
	int bestScore = position->toMove==WHITE?INT32_MIN:INT32_MAX;
	Move bestMove = 0;

	// Create array of possible moves
	Move moves[MAX_BRANCHING_FACTOR];
	// Finds the count of possible moves and fills move array with all possible moves
	int moveCount = legalMoves(moves, position, position->toMove);

	Position newPosition;
	int i;
	//For all possible moves
	for (i=0; i<moveCount; i++) {
		// Simulate move
		updatePosition(&newPosition, position, moves[i]);
		// Calculate the value of the board after the move
		int score = staticEvaluation(&newPosition);

		// If move wins the game, return the move
		if (score == winScore(position->toMove)) {
			return (Node) { .move = moves[i], .score = score };
		}

		//Otherwise, if the move is the best move so far, update the best move
		if ( (position->toMove == WHITE && score > bestScore) ||
			 (position->toMove == BLACK && score < bestScore) ) {
			bestScore = score;
			bestMove = moves[i];
		}
	}

	return (Node) { .move = bestMove, .score = bestScore };
}

// Search utilizing the quiescence evaluation to get a more accurate value estimate
Node quiescenceSearch(Position * position) {
	// Initialize best score to max values before iteration
	int bestScore = position->toMove==WHITE?INT32_MIN:INT32_MAX;
	Move bestMove = 0;

	Move moves[MAX_BRANCHING_FACTOR];
	int moveCount = legalMoves(moves, position, position->toMove);	// Gets legal moves

	Position newPosition;
	int i;
	// Create array of possible moves
	for (i=0; i<moveCount; i++) {
		// Simulate move
		updatePosition(&newPosition, position, moves[i]);
		// Calculate the value of the board after the move, checking to see
		// if the value will be invalidated by an opponents move
		int score = quiescenceEvaluation(&newPosition);

		// If move wins the game, return the move
		if (score == winScore(position->toMove)) {
			return (Node) { .move = moves[i], .score = score };
		}

		//Otherwise, if the move is the best move so far, update the best move
		if ( (position->toMove == WHITE && score > bestScore) ||
			 (position->toMove == BLACK && score < bestScore) ) {
			bestScore = score;
			bestMove = moves[i];
		}
	}

	return (Node) { .move = bestMove, .score = bestScore };
}

// Basic recursive search, which evaluates each position recursively up to a given depth
Node alphaBeta(Position * position, char depth, int alpha, int beta) {
	// If the game has ended after this move, return
	if (hasGameEnded(position))
		return (Node) { .score = endNodeEvaluation(position) };

	// If the depth has reached 1, then return the static evaluation of the position
	// As static evaluation is naturally an evaluation of depth 1
	if (depth == 1)
		return staticSearch(position);

	// Shortcut check to see if a simple 1-depth search will yield a winning move
	Node staticNode = staticSearch(position);
	if (staticNode.score == winScore(position->toMove))
		return staticNode;
	// Otherwise, proceed with the recursive search

	Move bestMove = 0;

	// Find list of possible moves, ordered by the move's value
	Move moves[MAX_BRANCHING_FACTOR];
	int moveCount = staticOrderLegalMoves(moves, position, position->toMove);

	Position newPosition;
	int i;
	// For all possible moves
	for (i=0; i<moveCount; i++) {
		// Simulate move
		updatePosition(&newPosition, position, moves[i]);

		// Evaluate node's score recursively 
		// (Makes move, then simulates making more moves after that move, and returns best score)
		int score = alphaBeta(&newPosition, depth-1, alpha, beta).score;

		// If the returned score wins the game, return that node	
		if (score == winScore(position->toMove)) {
			return (Node) { .move = moves[i], .score = score };
		}

		// Otherwise, update the best move and upper/lower bound (alpha/beta)
		if (position->toMove == WHITE && score > alpha) {
			alpha = score;
			bestMove = moves[i];
		} else if (position->toMove == BLACK && score < beta) {
			beta = score;
			bestMove = moves[i];
		}
		// If the lower bound exceeds the upper bound, terminate the loop
		if (alpha > beta) {
			break;
		}
	}

	return (Node) { .move = bestMove, .score = position->toMove==WHITE?alpha:beta };
}

// Generates sorted list of best possible moves for a certain position using alphaBeta
int alphaBetaNodes(Node * sortedNodes, Position * position, char depth) {
	Node nodes[MAX_BRANCHING_FACTOR];
	Move moves[MAX_BRANCHING_FACTOR];
	// Get list of legal moves
	int moveCount = legalMoves(moves, position, position->toMove);

	Position newPosition;
	int i;
	// For all possible moves
	for (i=0; i<moveCount; i++) {
		// Simulate move
		updatePosition(&newPosition, position, moves[i]);
		
		// Add move to list of nodes
		nodes[i].move = moves[i];
		// Use alphaBeta to calculate the score of the move, and add to list of nodes
		nodes[i].score = depth>1?alphaBeta(&newPosition, depth-1, INT32_MIN, INT32_MAX).score:staticEvaluation(&newPosition);
	}

	// Sort list of nodes into sortedNodes
	sortNodes(sortedNodes, nodes, moveCount, position->toMove);

	return moveCount;
}

// A version of alphaBeta that uses a list of nodes sorted by value using alphaBeta (why?)
// It also uses quiescence search to confirm the validity of the move
Node iterativeDeepeningAlphaBeta(Position * position, char depth, int alpha, int beta, BOOL verbose) {
	// If the game has ended after this move, return
	if (hasGameEnded(position))
		return (Node) { .score = endNodeEvaluation(position) };

	// If the depth has reached 1, then return the quiescence evaluation of the position
	if (depth == 1)
		return quiescenceSearch(position);
//		return staticSearch(position);

	// Shortcut check to see if a simple 1-depth search will yield a winning move
	Node staticNode = staticSearch(position);
	if (staticNode.score == winScore(position->toMove))
		return staticNode;
	// Otherwise, proceed with the recursive search

	Move bestMove = 0;

	if (verbose) {
		printf("Ordering moves...\n");
		fflush(stdout);
	}

	// Find list of possible moves, ordered by the move's value using alphaBeta to calculate the score
	Node nodes[MAX_BRANCHING_FACTOR];
	int moveCount = alphaBetaNodes(nodes, position, depth-1);

	Position newPosition;
	int i;
	// For all possible moves
	for (i=0; i<moveCount; i++) {
		// Simulate move
		updatePosition(&newPosition, position, nodes[i].move);

		if (verbose) {
			printf("(Move %2d/%d) ", i+1, moveCount);
			printFullMove(nodes[i].move, position->board);
			printf(" = ");
			fflush(stdout);
		}
		// Evaluate node's score recursively 
		// (Makes move, then simulates making more moves after that move, and returns best score)
		int score = iterativeDeepeningAlphaBeta(&newPosition, depth-1, alpha, beta, FALSE).score;

		if (verbose) {
			printf("%.2f\n", score/100.0);
			fflush(stdout);
		}

		// If the returned score wins the game, return that node
		if (score == winScore(position->toMove)) {
			return (Node) { .move = nodes[i].move, .score = score };
		}

		// Otherwise, update the best move and upper/lower bound (alpha/beta)
		if (position->toMove == WHITE && score > alpha) {
			alpha = score;
			bestMove = nodes[i].move;
		} else if (position->toMove == BLACK && score < beta) {
			beta = score;
			bestMove = nodes[i].move;
		}

		// If the lower bound exceeds the upper bound, terminate the loop
		if (alpha > beta) {
			break;
		}
	}

	return (Node) { .move = bestMove, .score = position->toMove==WHITE?alpha:beta };
}

// A version of iterativeDeepeningAlphaBeta with static upper and lower bounds
// Used for limiting the number of iterations when evaluating all possible moves
// by restricting the upper and lower bounds to the current bestupper and lower bounds
// (used when threading)
Node pIDAB(Position * position, char depth, int * p_alpha, int * p_beta) {
	// If the game has ended after this move, return
	if (hasGameEnded(position))
		return (Node) { .score = endNodeEvaluation(position) };

	// If the depth has reached 1, then return the quiescence evaluation of the position
	if (depth == 1)
		return quiescenceSearch(position);

	// Shortcut check to see if a simple 1-depth search will yield a winning move
	Node staticNode = staticSearch(position);
	if (staticNode.score == winScore(position->toMove))
		return staticNode;
	// Otherwise, proceed with the recursive search

	Move bestMove = 0;

	// Find list of possible moves, ordered by the move's value using alphaBeta to calculate the score
	Node nodes[MAX_BRANCHING_FACTOR];
	int moveCount = alphaBetaNodes(nodes, position, depth-1);

	Position newPosition;
	int i;
	int alpha = *p_alpha;
	int beta = *p_beta;
	// For all possible moves
	for (i=0; i<moveCount; i++) {
		// Simulate move
		updatePosition(&newPosition, position, nodes[i].move);

		// Evaluate node's score recursively 
		// (Makes move, then simulates making more moves after that move, and returns best score)
		int score = iterativeDeepeningAlphaBeta(&newPosition, depth-1, alpha, beta, FALSE).score;

		// If the returned score wins the game, return that node
		if (score == winScore(position->toMove)) {
			return (Node) { .move = nodes[i].move, .score = score };
		}

		// Otherwise, update the best move and upper/lower bound (alpha/beta)
		if (position->toMove == WHITE && score > alpha) {
			alpha = score;
			bestMove = nodes[i].move;
		} else if (position->toMove == BLACK && score < beta) {
			beta = score;
			bestMove = nodes[i].move;
		}

		// If the lower bounds exceed any of the upper bounds, terminate the loop
		if (alpha > beta || alpha > *p_beta || *p_alpha > beta) {
			break;
		}
	}

	return (Node) { .move = bestMove, .score = position->toMove==WHITE?alpha:beta };

}

// A version of pIDAB that writes the search data to a hash file
Node pIDABhashed(Position * position, char depth, int * p_alpha, int * p_beta) {
	// If the game has ended after this move, return
	if (hasGameEnded(position)) {
		int score = endNodeEvaluation(position);
		writeToHashFile(position, score, 0);
		return (Node) { .score = score };
	}

	// If the depth has reached 1, then return the quiescence evaluation of the position
	if (depth <= 1) {
		Node quie = quiescenceSearch(position);
		writeToHashFile(position, quie.score, depth);
		return quie;
	}

	// Shortcut check to see if a simple 1-depth search will yield a winning move
	Node staticNode = staticSearch(position);
	if (staticNode.score == winScore(position->toMove)) {
		writeToHashFile(position, staticNode.score, 1);
		return staticNode;
	}
	// Otherwise, proceed with the recursive search

	Move bestMove = 0;

	Node nodes[MAX_BRANCHING_FACTOR];
	int moveCount = alphaBetaNodes(nodes, position, depth-1);
	// Find list of possible moves, ordered by the move's value using alphaBeta to calculate the score

	Position newPosition;
	int i;
	int alpha = *p_alpha;
	int beta = *p_beta;
	// For all possible moves
	for (i=0; i<moveCount; i++) {
		// Simulate move
		updatePosition(&newPosition, position, nodes[i].move);

		// Evaluate node's score recursively 
		// (Makes move, then simulates making more moves after that move, and returns best score)
		int score = iterativeDeepeningAlphaBeta(&newPosition, depth-1, alpha, beta, FALSE).score;
		writeToHashFile(&newPosition, score, depth-1);

		// If the returned score wins the game, return that node
		if (score == winScore(position->toMove)) {
			return (Node) { .move = nodes[i].move, .score = score };
		}

		// Otherwise, update the best move and upper/lower bound (alpha/beta)
		if (position->toMove == WHITE && score > alpha) {
			alpha = score;
			bestMove = nodes[i].move;
		} else if (position->toMove == BLACK && score < beta) {
			beta = score;
			bestMove = nodes[i].move;
		}

		// If the lower bounds exceed any of the upper bounds, terminate the loop
		if (alpha > beta || alpha > *p_beta || *p_alpha > beta) {
			break;
		}
	}

	writeToHashFile(position, position->toMove==WHITE?alpha:beta, depth);
	return (Node) { .move = bestMove, .score = position->toMove==WHITE?alpha:beta };
}


// Parallel processing currently only implemented for Windows
// ifdef block to define windows-only features
#ifdef _WIN32

// Threaded position evaluation function for windows
DWORD WINAPI evaluatePositionThreadFunction(LPVOID lpParam) {
	// Get thread info
	ThreadInfo * tInfo = (ThreadInfo *) lpParam;
	// Get position to evaluate from the thread info
	Position * pos = &tInfo->pos;

	// Evalute the value of the node using pIDAB
	Node node = pIDAB(pos, tInfo->depth, tInfo->alpha, tInfo->beta);

	// If the new node score is better than the old node score
	// Update the global upper and lower bounds accordingly
	if ( pos->toMove == BLACK && node.score > *tInfo->alpha ) {
		*tInfo->alpha = node.score;
	} else if ( pos->toMove == WHITE && node.score < *tInfo->beta ) {
		*tInfo->beta = node.score;
	}

	if (tInfo->verbose) {
		printf("-");
		fflush(stdout);
	}

	// Return the score of the node
	return node.score;
}

// Threaded position evaluation function that calls pIDABhashed instead of pIDAB
DWORD WINAPI evaluatePositionThreadFunctionHashed(LPVOID lpParam) {
	// Get thread info
	ThreadInfo * tInfo = (ThreadInfo *) lpParam;
	// Get position to evaluate from the thread info
	Position * pos = &tInfo->pos;

	// Evalute the value of the node using pIDABhashed
	Node node = pIDABhashed(pos, tInfo->depth, tInfo->alpha, tInfo->beta);

	// If the new node score is better than the old node score
	// Update the global upper and lower bounds accordingly
	if ( pos->toMove == BLACK && node.score > *tInfo->alpha ) {
		*tInfo->alpha = node.score;
	} else if ( pos->toMove == WHITE && node.score < *tInfo->beta ) {
		*tInfo->beta = node.score;
	}

	if (tInfo->verbose) {
		printf("-");
		fflush(stdout);
	}

	// Return the score of the node
	return node.score;
}

// Function which initiates threaded move search
Node idabThreaded(Position * position, int depth, BOOL verbose) {
	// If the game has ended after this move, return
	if (hasGameEnded(position))
		return (Node) { .score = endNodeEvaluation(position) };

	// If the depth has reached 1, then return the quiescence evaluation of the position
	if (depth <= 1)
		return quiescenceSearch(position);

	int i;
	Node nodes[MAX_BRANCHING_FACTOR];
	// Find list of possible moves, ordered by the move's value using alphaBeta to calculate the score
	int moveCount = alphaBetaNodes(nodes, position, depth-1);

	// If there is only 1 possible move, return that move
	if (moveCount == 1) {
		return nodes[0];
	}

	if (verbose) {
		printf("Analyzing %d possible moves with base depth %d:\n[", moveCount, depth);
		for (i=0; i<moveCount; i++)
			printf(" ");
		printf("]\r[");
		fflush(stdout);
	}

	HANDLE threadHandles[MAX_BRANCHING_FACTOR];
	ThreadInfo threadInfo[MAX_BRANCHING_FACTOR];
	// Initialize upper and lower bounds to maximum values
	int alpha = INT32_MIN;
	int beta = INT32_MAX;

	// For all possible moves
	for (i=0; i<moveCount; i++) {
		// Initiallize thread struct 
		threadInfo[i].depth = depth-1;
		updatePosition(&threadInfo[i].pos, position, nodes[i].move);
		threadInfo[i].alpha = &alpha;
		threadInfo[i].beta = &beta;
		threadInfo[i].verbose = verbose;

		// Start threaded evaluation of the current move
		threadHandles[i] = CreateThread(NULL, 0, evaluatePositionThreadFunction, (LPVOID) &threadInfo[i], 0, NULL);

		// If the thread failed to create, print an error message
		if ( threadHandles[i] == NULL ) {
//			printf("Error launching process on move #%d!\n", i);
			printf("!");
			fflush(stdout);
		}
	}

	// Wait for threads to finish (windows command)
	WaitForMultipleObjects((DWORD) moveCount, threadHandles, TRUE, INFINITE);
	if (verbose) {
		printf("]\n");
		fflush(stdout);
	}

	// Retrieve best move

	Move bestMove = 0;
	int bestMoveScore = position->toMove==WHITE?INT32_MIN:INT32_MAX;
	long unsigned int retVal;
	int score;
	// For all of the possible moves
	for (i=0; i<moveCount; i++) {
		// Retrieve the exit value of the thread, which is the score of the position
		GetExitCodeThread(threadHandles[i], &retVal);
		score = (int) retVal;

		// If the score is better than the current best score, update the best move and best score
		if ( (position->toMove == WHITE && score > bestMoveScore) || (position->toMove == BLACK && score < bestMoveScore) ) {
			bestMove = nodes[i].move;
			bestMoveScore = score;
		}

		// Deallocate thread
		if (CloseHandle(threadHandles[i]) == 0) {
//			printf("Error on closing thread #%d!\n", i);
			printf("x");
			fflush(stdout);
		}
	}

	return (Node) { .move = bestMove, .score = bestMoveScore };
}

// Function which initiates threaded move search with initial upper and lower bounds
// Evaluates value of first position before performing threading, and restricts
// the upper and lower bounds of the threads to the upper and lower bounds of the
// first position
Node idabThreadedBestFirst(Position * position, int depth, BOOL verbose) {
	// If the game has ended after this move, return
	if (hasGameEnded(position))
		return (Node) { .score = endNodeEvaluation(position) };

	// If the depth has reached 1, then return the quiescence evaluation of the position
	if (depth <= 1)
		return quiescenceSearch(position);

	int i;
	Node nodes[MAX_BRANCHING_FACTOR];
	// Find list of possible moves, ordered by the move's value using alphaBeta to calculate the score
	int moveCount = alphaBetaNodes(nodes, position, depth-1);

	// If there is only 1 possible move, return that move
	if (moveCount == 1) {
		return nodes[0];
	}


	Position firstPos;
	// Simulate first move
	updatePosition(&firstPos, position, nodes[0].move);
	// Evaluate score of first move
	Node firstReply = idabThreaded(&firstPos, depth-1, FALSE);

	// If the first move wins the game, return that move
	if ( firstReply.score == winScore(position->toMove) ) {
		if (verbose) {
				printf("Playing checkmate move: ");
				printFullMove(nodes[0].move, position->board);
				printf(".\n");
		}
		return (Node) { .move = nodes[0].move, .score = firstReply.score };
	}

	if (verbose) {
		printf("Move ");
		printFullMove(nodes[0].move, position->board);
		printf(" had score of %+.2f.\n", firstReply.score/100.0);
		printf("Analyzing other %d possible moves with minimum depth of %d plies:\n[", moveCount-1, depth);
		for (i=0; i<moveCount-1; i++)
			printf(" ");
		printf("]\r[");
		fflush(stdout);
	}

	HANDLE threadHandles[MAX_BRANCHING_FACTOR];
	ThreadInfo threadInfo[MAX_BRANCHING_FACTOR];
	// Initialize upper and lower bounds to maximum values
	int alpha = INT32_MIN;
	int beta = INT32_MAX;

	// Set upper and lower bounds
	if (position->toMove == WHITE) {
		alpha = firstReply.score;
	} else {
		beta = firstReply.score;
	}

	// For all possible moves
	for (i=0; i<moveCount-1; i++) {
		// Initiallize thread struct 
		threadInfo[i].depth = depth-1;
		updatePosition(&threadInfo[i].pos, position, nodes[i+1].move);
		threadInfo[i].alpha = &alpha;
		threadInfo[i].beta = &beta;
		threadInfo[i].verbose = verbose;

		// Start threaded evaluation of the current move
		threadHandles[i] = CreateThread(NULL, 0, evaluatePositionThreadFunction, (LPVOID) &threadInfo[i], 0, NULL);

		// If the thread failed to create, print an error message
		if ( threadHandles[i] == NULL ) {
//			printf("Error launching process on move #%d!\n", i);
			printf("!");
			fflush(stdout);
		}
	}

	// Wait for threads to finish (windows command)
	WaitForMultipleObjects((DWORD) moveCount-1, threadHandles, TRUE, INFINITE);
	if (verbose) {
		printf("] Done!\n");
		fflush(stdout);
	}

	// Retrieve best move

	Move bestMove = nodes[0].move;
	int bestMoveScore = firstReply.score;
	long unsigned int retVal;
	int score;
	for (i=0; i<moveCount-1; i++) {
		// Retrieve the exit value of the thread, which is the score of the position
		GetExitCodeThread(threadHandles[i], &retVal);
		score = (int) retVal;

		// If the score is better than the current best score, update the best move and best score
		if ( (position->toMove == WHITE && score > bestMoveScore) || (position->toMove == BLACK && score < bestMoveScore) ) {
			bestMove = nodes[i+1].move;
			bestMoveScore = score;
		}

		// Deallocate thread
		if (CloseHandle(threadHandles[i]) == 0) {
//			printf("Error on closing thread #%d!\n", i);
			printf("x");
			fflush(stdout);
		}
	}

	return (Node) { .move = bestMove, .score = bestMoveScore };
}

// Same as idabThreadedBestFirst, but calls the hashed version of evaluatePositionThreadFunction
Node idabThreadedBestFirstHashed(Position * position, int depth, BOOL verbose) {
	// If the game has ended after this move, return
	if (hasGameEnded(position)) {
		int score = endNodeEvaluation(position);
		writeToHashFile(position, score, 0);
		return (Node) { .score = score };
	}

	// If the depth has reached 1, then return the quiescence evaluation of the position
	if (depth <= 1) {
		Node quie = quiescenceSearch(position);
		writeToHashFile(position, quie.score, depth);
		return quie;
	}

	int i;
	Node nodes[MAX_BRANCHING_FACTOR];
	// Find list of possible moves, ordered by the move's value using alphaBeta to calculate the score
	int moveCount = alphaBetaNodes(nodes, position, depth-1);

	// If there is only 1 possible move, return that move
	if (moveCount == 1) {
		return nodes[0];
	}

	Position firstPos;
	// Simulate first move
	updatePosition(&firstPos, position, nodes[0].move);
	// Evaluate score of first move
	Node firstReply = idabThreaded(&firstPos, depth-1, FALSE);

	// If the first move wins the game, return that move
	if ( firstReply.score == winScore(position->toMove) ) {
		if (verbose) {
				printf("Playing checkmate move: ");
				printFullMove(nodes[0].move, position->board);
				printf(".\n");
		}
		writeToHashFile(position, firstReply.score, depth);
		return (Node) { .move = nodes[0].move, .score = firstReply.score };
	}

	if (verbose) {
		printf("Move ");
		printFullMove(nodes[0].move, position->board);
		printf(" had score of %+.2f.\n", firstReply.score/100.0);
		printf("Analyzing other %d possible moves with minimum depth of %d plies:\n[", moveCount-1, depth);
		for (i=0; i<moveCount-1; i++)
			printf(" ");
		printf("]\r[");
		fflush(stdout);
	}

	HANDLE threadHandles[MAX_BRANCHING_FACTOR];
	ThreadInfo threadInfo[MAX_BRANCHING_FACTOR];
	// Initialize upper and lower bounds to maximum values
	int alpha = INT32_MIN;
	int beta = INT32_MAX;

	// Set upper and lower bounds
	if (position->toMove == WHITE) {
		alpha = firstReply.score;
	} else {
		beta = firstReply.score;
	}

	// For all possible moves
	for (i=0; i<moveCount-1; i++) {
		// Initiallize thread struct 
		threadInfo[i].depth = depth-1;
		updatePosition(&threadInfo[i].pos, position, nodes[i+1].move);
		threadInfo[i].alpha = &alpha;
		threadInfo[i].beta = &beta;
		threadInfo[i].verbose = verbose;

		// Start threaded evaluation of the current move
		threadHandles[i] = CreateThread(NULL, 0, evaluatePositionThreadFunctionHashed, (LPVOID) &threadInfo[i], 0, NULL);

		if ( threadHandles[i] == NULL ) {
//			printf("Error launching process on move #%d!\n", i);
			printf("!");
			fflush(stdout);
		}
	}

	// Wait for threads to finish (windows command)
	WaitForMultipleObjects((DWORD) moveCount-1, threadHandles, TRUE, INFINITE);
	if (verbose) {
		printf("] Done!\n");
		fflush(stdout);
	}

	// Retrieve best move

	Move bestMove = nodes[0].move;
	int bestMoveScore = firstReply.score;
	long unsigned int retVal;
	int score;
	for (i=0; i<moveCount-1; i++) {
		// Retrieve the exit value of the thread, which is the score of the position
		GetExitCodeThread(threadHandles[i], &retVal);
		score = (int) retVal;

		writeToHashFile(&threadInfo[i].pos, score, depth-1);

		// If the score is better than the current best score, update the best move and best score
		if ( (position->toMove == WHITE && score > bestMoveScore) || (position->toMove == BLACK && score < bestMoveScore) ) {
			bestMove = nodes[i+1].move;
			bestMoveScore = score;
		}

		// Deallocate thread
		if (CloseHandle(threadHandles[i]) == 0) {
//			printf("Error on closing thread #%d!\n", i);
			printf("x");
			fflush(stdout);
		}
	}

	writeToHashFile(position, bestMoveScore, depth);
	return (Node) { .move = bestMove, .score = bestMoveScore };
}

#endif /* _WIN32 */

// Gets a random move
Move getRandomMove(Position * position) {
	Move moves[MAX_BRANCHING_FACTOR];
	// Find all legal moves
	int totalMoves = legalMoves(moves, position, position->toMove);
	// Choose random move and return
	int chosenMove = rand() % totalMoves;
	return moves[chosenMove];
}

// Initial searching for AI move
Move getAIMove(Game * game, int depth) {
	printf("--- AI ---\n");
	fflush(stdout);

	// If it is the first move, chooses initial game move using hardcoded initial moves from a .txt file
	if ( fromInitial(game) && countBookOccurrences(game) > 0 ) {
		printf("There are %d available book continuations.\n", countBookOccurrences(game));
		fflush(stdout);
		Move bookMove = getBookMove(game);
		printf("CHOSEN book move: ");
		printFullMove(bookMove, game->position.board);
		printf(".\n");
		fflush(stdout);
		return bookMove;
	}

	// Start timer for search
	time_t startTime, endTime;
	startTime = time(NULL);

//	Move move = getRandomMove(&game->position);
//	Move move = simpleEvaluation(&game->position).move;
//	Move move = minimax(&game->position, AI_DEPTH).move;
//	Node node = alphaBeta(&game->position, depth, INT32_MIN, INT32_MAX, TRUE);
//	Node node = iterativeDeepeningAlphaBeta(&game->position, depth, INT32_MIN, INT32_MAX, TRUE);
//	Node node = idabThreaded(&game->position, depth, TRUE);
//	Node node = idabThreadedBestFirst(&game->position, depth, TRUE);
//	Node node = idabThreadedBestFirstHashed(&game->position, depth, TRUE);

	// Search for best move using current position
#ifdef _WIN32
	Node node = idabThreadedBestFirst(&game->position, depth, TRUE);
#else
	Node node = iterativeDeepeningAlphaBeta(&game->position, depth, INT32_MIN, INT32_MAX, TRUE);
#endif

	// End timer for search
	endTime = time(NULL);

	// Print which move was chosen, and how long it took
	printf("CHOSEN move: ");
	printFullMove(node.move, game->position.board);
	printf(" in %d seconds [%+.2f, %+.2f]\n", (int) (endTime-startTime), staticEvaluation(&game->position)/100.0, node.score/100.0);
	fflush(stdout);

	return node.move;
}

// Parses a string for a move into a move struct
Move parseMove(char * move) {
	// Converts first couple into a location
	int pos1 = str2index(&move[0]);
	// Converts second couple into a location
	int pos2 = str2index(&move[2]);
	// Generates move from 2 locations and returns
	return generateMove(pos1, pos2);
}

// Gets player input for a move, parses into string and returns as move
Move getPlayerMove() {
	char input[100];
	gets( input );
	return parseMove(input);
}

// Suggests a move by calling the AI move generator
Move suggestMove(char fen[], int depth) {
	Game game;
	getFenGame(&game, fen);
	return getAIMove(&game, depth);
}

// ==============================================================
// 		        ========= PLAY LOOP (TEXT) ==========
// ==============================================================

// Prints out that the user is playing as white, then starts the game
void playTextWhite(int depth) {
	printf("Playing as WHITE!\n");
	fflush(stdout);

	Game game;
	// Initialize game
	getInitialGame(&game);

	// While the game has not ended
	while(TRUE) {
		// Print the game board
		printBoard(game.position.board);
		// Check game end state
        if (hasGameEnded(&game.position))
        	break;

		// Player moves
        makeMove(&game, getPlayerMove());

		// Print the game board
        printBoard(game.position.board);
		// Check game end state
        if (hasGameEnded(&game.position))
			break;

		// AI moves
        makeMove(&game, getAIMove(&game, depth));
	}

	// Prints the outcome of the game
	printOutcome(&game.position);
}

// Prints out that the user is playing as black, then starts the game
void playTextBlack(int depth) {
	printf("Playing as BLACK!\n");
	fflush(stdout);

	Game game;
	// Initialize game
	getInitialGame(&game);

	// While the game has not ended
	while(TRUE) {
		// Print the game board
		printBoard(game.position.board);
		// Check game end state
        if (hasGameEnded(&game.position))
        	break;

		// Player moves
        makeMove(&game, getPlayerMove());

		// Print the game board
        printBoard(game.position.board);
		// Check game end state
        if (hasGameEnded(&game.position))
			break;

		// AI moves
        makeMove(&game, getAIMove(&game, depth));
	}

	// Prints the outcome of the game
	printOutcome(&game.position);
}

// Decides how to start the game based on the value of WHITE and BLACK
void playTextAs(char color, int depth) {
	if (color == WHITE)
		playTextWhite(depth);
	if (color == BLACK)
		playTextBlack(depth);
}

// Randomizes whether or not the first player is black or white, and calls playText As
void playTextRandomColor(int depth) {
	char colors[] = {WHITE, BLACK};
	char color = colors[rand()%2];
	playTextAs(color, depth);
}

// ===========================

 /*
int main(int argc, char *argv[]) {
	srand(time(NULL));

	playTextRandomColor(DEFAULT_AI_DEPTH);

	return EXIT_SUCCESS;
}
// */
