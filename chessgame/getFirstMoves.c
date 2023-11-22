#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "chessgame.h"

// Assume ChessGame struct and other necessary structures and functions are defined

// Function to copy ChessGame state from 'src' to 'dest'
static void copy_game_state(ChessGame *dest, const ChessGame *src) {
    // Assuming a simple shallow copy of the game state (board, current_turn, etc.)
    *dest = *src;
}

// Function to truncate ChessGame to its first N half-moves
ChessGame getFirstMoves(const ChessGame *game, int n) {
    ChessGame truncated_game;
    copy_game_state(&truncated_game, game); // Initialize truncated game state as the original game

    int half_move_count = 0;
    bool is_white_turn = true; // Assuming white starts the game

    // Perform the moves up to N half-moves
    // This assumes that chessgame_make_move updates the current_turn after each move
    while (half_move_count < n) {
        // Execute the next move based on the current turn
        char color = (is_white_turn) ? 'w' : 'b';
        char move[5]; // Assuming the move is represented as a string ("e2e4" format)
        // Get the next move from the game or any other means based on your implementation
        // Assuming the move is obtained and stored in the 'move' variable

        // Make the move in the truncated game
        if (!chessgame_make_move(&truncated_game, move, color)) {
            printf("Invalid move or end of game reached.\n");
            break; // Break the loop on an invalid move or end of the game
        }

        // Update counters and flags for the next move
        half_move_count++;
        is_white_turn = !is_white_turn; // Alternate turns
    }

    return truncated_game;
}
