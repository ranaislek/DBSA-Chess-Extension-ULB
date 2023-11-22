#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <stdbool.h>

#define BOARD_SIZE 8

typedef struct {
    char type; // Piece type: 'P', 'R', 'N', 'B', 'Q', 'K' or ' '
    char color; // Piece color: 'w' for white, 'b' for black or ' ' for empty square
} Piece;

typedef struct {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    bool is_white_turn; // To indicate whose turn it is - true for white, false for black
    // Other necessary game state variables can be added here
} ChessGame;

// Function Declarations
void chessgame_initialize_board(ChessGame *game);
void chessgame_print_board(const ChessGame *game);
bool chessgame_is_valid_move(int from_rank, int from_file, int to_rank, int to_file, char type, char color);
bool chessgame_make_move(ChessGame *game, char* move, char color);

#endif /* CHESSGAME_H */
