#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smallchesslib.h"


//SAN and FEN notation for chess-game and chess-board respectively.

// Structure to represent chess game -SAN/PGN
typedef struct
{
    SCL_Record game; 
} ChessGame;

// Structure to represent chess board -FEN
typedef struct
{
    SCL_Board board;
} ChessBoard;

//*****************************************************************************
// 1- Function to get the board state at a given half-move
// - getBoard(chessgame, integer) -> chessboard: Return the board state
// at a given half-move (A full move is counted only when both players
// have played). The integer parameter indicates the count of half
// moves since the beginning of the game. A 0 value of this parameter
// means the initial board state, i.e.,(rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1).

/*
    to-do:
       get pgn (chessgame?) string and put it in the record
       use recordapply (record, board, halfmove)
       get pgn string to the number of half moves, and get the state of the board
       use boardtofen to convert the board state to fen (chessboard?)
       return fen
    */

static ChessGame* getBoard(ChessGame* chessgame, int half_moves) {
    ChessGame *result = (ChessGame*)malloc(sizeof(ChessGame));
    ChessBoard *board = (ChessBoard*)malloc(sizeof(ChessBoard));

    // Apply moves to the board
    SCL_recordApply(result->game, board->board, half_moves);

    // Convert the board state to FEN
    char fen[100]; // Adjust the size as needed
    SCL_boardToFEN(board->board, fen);

    // Return the fen
    return fen;
}


//*****************************************************************************
// 2- getFirstMoves(chessgame, integer) -> chessgame: Returns the
// chessgame truncated to its first N half-moves. This function may also
// be called getOpening(...). Again the integer parameter is zero based.
/*
to-do:
    use the library called strtok to split the string
    then you will have the number of tokens
    then trim the string to the number of half moves
    then return the string 
*/

// Function to get the first N half-moves of a chess game
static ChessGame* getFirstMoves(const ChessGame* game, int num_half_moves) {
    ChessGame* result = (ChessGame*)malloc(sizeof(ChessGame));
    // Implement the logic to get the first N half-moves
    // Replace the following line with your actual logic
    strncpy(result->game, game->game, sizeof(result->game));
    return result;
}

//*****************************************************************************
// 3- hasOpening(chessgame, chessgame) -> bool: Returns true if the first
// chess game starts with the exact same set of moves as the second
// chess game. The second parameter should not be a full game, but
// should only contain the opening moves that we want to check for,
// which can be of any length, i.e., m half-moves.
/*
to-do:
    1- SCL_recordFromPGN to convert the string to record
       use recordLength to get the number of half moves and compare

    2-use getfirstmoves to get the first n half moves
    then compare the two strings
*/

// Function to check if a chess game starts with the exact same set of moves as another chess game
static int hasOpening(const ChessGame* game1, const ChessGame* game2) {
    // Implement the logic to check if the games start with the same moves
    // Return 1 if true, 0 otherwise
    // Replace the following line with your actual logic
    return strcmp(game1->game, game2->game) == 0 ? 1 : 0;
}

//*****************************************************************************
// - hasBoard(chessgame, chessboard, integer) -> bool: Returns true if
// the chessgame contains the given board state in its first N
// half-moves. Only compare the state of the pieces and not compare
// the move count, castling right, en passant pieces, ...
/*
to-do:
    use getboard and apply the requested amount of moves and check if the state of the bard gives the right fen
*/

// Function to check if a chess game contains a given board state in its first N half-moves
static int hasBoard(const ChessGame* game, const ChessBoard* board, int nHalfMoves) {
    // Implement the logic to check if the game contains the board state
    // Return 1 if true, 0 otherwise
    // Replace the following line with your actual logic
    return 0;
}

//*****************************************************************************


int main() {

    //example pgn - game of the century
    ChessGame pgn = "1. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 6. Qb3 dxc4 7.
                    Qxc4 c6 8. e4 Nbd7 9. Rd1 Nb6 10. Qc5 Bg4 11. Bg5 Na4 12. Qa3
                    Nxc3 13. bxc3 Nxe4 14. Bxe7 Qb6 15. Bc4 Nxc3 16. Bc5 Rfe8+ 17.
                    Kf1 Be6 18. Bxb6 Bxc4+ 19. Kg1 Ne2+ 20. Kf1 Nxd4+ 21. Kg1 Ne2+
                    22. Kf1 Nc3+ 23. Kg1 axb6 24. Qb4 Ra4 25. Qxb6 Nxd1 26. h3 Rxa2
                    27. Kh2 Nxf2 28. Re1 Rxe1 29. Qd8+ Bf8 30. Nxe1 Bd5 31. Nf3 Ne4
                    32. Qb8 b5 33. h4 h5 34. Ne5 Kg7 35. Kg1 Bc5+ 36. Kf1 Ng3+ 37.
                    Ke1 Bb4+ 38. Kd1 Bb3+ 39. Kc1 Ne2+ 40. Kb1 Nc3+ 41. Kc1 Rc2#"; 
    // Test getBoard function
    ChessGame *game = "1. e4 e5 2. Nf3 Nc6 3. Bb5";
    //call getboard
    ChessGame *boardState = getBoard(game, 3);
    printf("Board state: %s\n", boardState->game);
    free(boardState);

    return 0;
}
