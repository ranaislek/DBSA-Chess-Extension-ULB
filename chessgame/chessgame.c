#include <stdio.h>
//#include <postgres.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

//#include "utils/builtins.h"
//#include "libpq/pqformat.h"
#include "chessgame.h"

PG_MODULE_MAGIC;

// Initialize the chess board with starting positions
static void 
chessgame_initialize_board(ChessGame *game) {
    // Initial positions for white pieces
    game->board[0][0] = (Piece){'R', 'w'};
    game->board[0][1] = (Piece){'N', 'w'};
    game->board[0][2] = (Piece){'B', 'w'};
    game->board[0][3] = (Piece){'Q', 'w'};
    game->board[0][4] = (Piece){'K', 'w'};
    game->board[0][5] = (Piece){'B', 'w'};
    game->board[0][6] = (Piece){'N', 'w'};
    game->board[0][7] = (Piece){'R', 'w'};
    for (int i = 0; i < BOARD_SIZE; i++) {
        game->board[1][i] = (Piece){'P', 'w'};
    }

    // Initial positions for black pieces
    game->board[7][0] = (Piece){'r', 'b'};
    game->board[7][1] = (Piece){'n', 'b'};
    game->board[7][2] = (Piece){'b', 'b'};
    game->board[7][3] = (Piece){'q', 'b'};
    game->board[7][4] = (Piece){'k', 'b'};
    game->board[7][5] = (Piece){'b', 'b'};
    game->board[7][6] = (Piece){'n', 'b'};
    game->board[7][7] = (Piece){'r', 'b'};
    for (int i = 0; i < BOARD_SIZE; i++) {
        game->board[6][i] = (Piece){'p', 'b'};
    }

    // Empty squares on the board
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = (Piece){' ', ' '};
        }
    }
}


// Function to print the current board
static void 
chessgame_print_board(const ChessGame *game) {
    printf("  a b c d e f g h\n");
    printf("  -----------------\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d|", BOARD_SIZE - i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j].type == ' ') {
                printf("  ");
            } else {
                printf("%c%c", game->board[i][j].type, game->board[i][j].color);
            }
            printf("|");
        }
        printf("\n");
        printf(" |\n");
    }
    printf("  -----------------\n");
    printf("  a b c d e f g h\n");
}


// Function to check if a move is valid for a rook
static bool 
chessgame_is_valid_rook_move(int from_rank, int from_file, int to_rank, int to_file) {
    return (from_rank == to_rank || from_file == to_file);
}

// Function to check if a move is valid for a bishop
static bool 
chessgame_is_valid_bishop_move(int from_rank, int from_file, int to_rank, int to_file) {
    return (abs(from_rank - to_rank) == abs(from_file - to_file));
}

// Function to check if a move is valid for a queen
static bool 
chessgame_is_valid_queen_move(int from_rank, int from_file, int to_rank, int to_file) {
    return (is_valid_rook_move(from_rank, from_file, to_rank, to_file) ||
            is_valid_bishop_move(from_rank, from_file, to_rank, to_file));
}

// Function to check if a move is valid for a knight
static bool 
chessgame_is_valid_knight_move(int from_rank, int from_file, int to_rank, int to_file) {
    int rank_diff = abs(from_rank - to_rank);
    int file_diff = abs(from_file - to_file);
    return ((rank_diff == 2 && file_diff == 1) || (rank_diff == 1 && file_diff == 2));
}

// Function to check if a move is valid for a pawn
static bool 
chessgame_is_valid_pawn_move(ChessGame *game, int from_rank, int from_file, int to_rank, int to_file, char color) {
    int direction = (color == 'w') ? -1 : 1;
    int start_rank = (color == 'w') ? 6 : 1;

    // Pawn moves one square forward
    if (from_file == to_file && from_file == to_file + direction) {
        // Normal one-step move
        return (game->board[to_rank][to_file].type == ' ' && to_rank == from_rank + direction);
    }

    // Pawn double-step move from initial position
    if (from_file == to_file && from_rank == start_rank && to_rank == from_rank + 2 * direction) {
        // Check if the path is clear
        return (game->board[to_rank][to_file].type == ' ' &&
                game->board[to_rank - direction][to_file].type == ' ');
    }

    // Pawn captures
    if (abs(to_file - from_file) == 1 && to_rank == from_rank + direction) {
        // Check if the destination has an opponent's piece
        return (game->board[to_rank][to_file].type != ' ' && game->board[to_rank][to_file].color != color);
    }

    return false;
}

// Function to check if a move is valid for the king
static bool 
chessgame_is_valid_king_move(int from_rank, int from_file, int to_rank, int to_file) {
    return (abs(from_rank - to_rank) <= 1 && abs(from_file - to_file) <= 1);
}

// Function to check if a move is valid for a piece based on its type
static bool 
chessgame_is_valid_move(int from_rank, int from_file, int to_rank, int to_file, char type, char color) {
    switch (type) {
        case 'R':
        case 'r':
            return is_valid_rook_move(from_rank, from_file, to_rank, to_file);
        case 'N':
        case 'n':
            return is_valid_knight_move(from_rank, from_file, to_rank, to_file);
        case 'B':
        case 'b':
            return is_valid_bishop_move(from_rank, from_file, to_rank, to_file);
        case 'Q':
        case 'q':
            return is_valid_queen_move(from_rank, from_file, to_rank, to_file);
        case 'P':
        case 'p':
            return is_valid_pawn_move(from_rank, from_file, to_rank, to_file, color);
        case 'K':
        case 'k':
            return is_valid_king_move(from_rank, from_file, to_rank, to_file);
        default:
            return false;
    }
}

// Function to validate a move and execute it
static bool 
chessgame_make_move(ChessGame *game, char* move, char color) {
    if (strlen(move) != 4) {
        return false; // Move should be in the format "e2e4"
    }

    int from_file = file_to_index(move[0]);
    int from_rank = rank_to_index(move[1]);
    int to_file = file_to_index(move[2]);
    int to_rank = rank_to_index(move[3]);

    // Check if the indices are within the board bounds
    if (from_file < 0 || from_file >= BOARD_SIZE || from_rank < 0 || from_rank >= BOARD_SIZE ||
        to_file < 0 || to_file >= BOARD_SIZE || to_rank < 0 || to_rank >= BOARD_SIZE) {
        return false;
    }

    char piece_type = game->board[from_rank][from_file].type;
    char piece_color = game->board[from_rank][from_file].color;

    // Validate the move for the specific piece
    if (!is_valid_move(from_rank, from_file, to_rank, to_file, piece_type, piece_color)) {
        return false;
    }

    // Implement move execution here
    // Update the board state based on the move
    game->board[to_rank][to_file] = game->board[from_rank][from_file];
    Piece empty_piece = {' ', ' '};
    game->board[from_rank][from_file] = empty_piece;

    // Basic check for check or checkmate (needs refinement)
    // Check if the opponent's king is in a vulnerable position
    // Placeholder logic:
    bool is_opponent_king_vulnerable = is_king_vulnerable(game->board, (color == 'w' ? 'b' : 'w'));

    if (is_opponent_king_vulnerable) {
        //Check for checkmate or check
        bool is_check = is_checkmate(game->board, (color == 'w' ? 'b' : 'w'));
        if (is_checkmate) {
            printf("Checkmate!\n");
        } else {
            printf("Check!\n");
        }
    }

    return true; // Placeholder return value
}