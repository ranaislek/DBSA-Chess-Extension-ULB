/*
 * chess.c 
 *
 * PostgreSQL Chess Type:
 *
 * chess
 *
 * Authors: 
 * Rana Islek <rana.islek@ulb.be>
 */

/*

Your extension will implement data types, functions and indexes as follows:
Data types: chessgame, chessboard. The input/output for your types shall
use SAN and FEN notation for chess-game and chess-board respectively.

*/
#include "chess.h"
#include "smallchesslib.h"
#include "postgres.h"

//SAN and FEN notation for chess-game and chess-board respectively.

//Convert string representation to ChessBoard - FEN
static ChessBoard *str_to_chessboard(char *str) {
    ChessBoard *result = palloc0(sizeof(ChessBoard));
    SCL_boardFromFEN(result->board, str);
    return result;
}

//Convert string representation to ChessGame - SAN
static ChessGame *str_to_chessgame(const char *str) {
    ChessGame *result = (ChessGame *)palloc(sizeof(ChessGame));
    //SCL_boardToFEN(result->game, str);
    SCL_recordFromPGN(result->game, str);
    return result;
}

/*****************************************************************************/

// Input function for ChessGame
PG_FUNCTION_INFO_V1(chessgame_in);
Datum chessgame_in(PG_FUNCTION_ARGS) {
    char *str = PG_GETARG_CSTRING(0);
    ChessGame *result;
    result = str_to_chessgame(str);
    PG_RETURN_CHESSGAME_P(result);
}

// Input function for ChessBoard
PG_FUNCTION_INFO_V1(chessboard_in);
Datum chessboard_in(PG_FUNCTION_ARGS) {
    char *str = PG_GETARG_CSTRING(0);
    ChessBoard *result;
    result = str_to_chessboard(str);
    PG_RETURN_CHESSBOARD_P(result);
}

/*****************************************************************************/

// Output function for ChessGame
PG_FUNCTION_INFO_V1(chessgame_out);
Datum chessgame_out(PG_FUNCTION_ARGS) {
    ChessGame *game = PG_GETARG_CHESSGAME_P(0);
    char *result = palloc0(100);
    SCL_boardToFEN(game->game, result);
    PG_FREE_IF_COPY(game, 0);
    PG_RETURN_CSTRING(result);
}

// Output function for ChessBoard
PG_FUNCTION_INFO_V1(chessboard_out);
Datum chessboard_out(PG_FUNCTION_ARGS) {
    ChessBoard *board = PG_GETARG_CHESSBOARD_P(0);
    char *result = palloc0(100);
    // SCL_boardToFEN(board->board, result); // print pgn . test file
    // print pgn . test file
    SCL_printPGN(board->board, result, 0);
    PG_FREE_IF_COPY(board, 0);
    PG_RETURN_CSTRING(result);
}

/*****************************************************************************/

// - getBoard(chessgame, integer) -> chessboard: Return the board state
// at a given half-move (A full move is counted only when both players
// have played). The integer parameter indicates the count of half
// moves since the beginning of the game. A 0 value of this parameter
// means the initial board state, i.e.,(rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1).

PG_FUNCTION_INFO_V1(getBoard);
Datum
getBoard(PG_FUNCTION_ARGS)
{
    /*
    to-do:
       get pgn string and put it in the record
       use recordapply (record, board, halfmove)
       get pgn string to the number of half moves, and get the state of the board
       use boardtofen to convert the board state to fen
       return fen
    */


    //smallchesslib.h get board position
    ChessGame *chessgame = PG_GETARG_CHESSBOARD_P(0);
    SCL_boardGetPosition(chessgame->game);
    PG_RETURN_CHESSGAME_P(chessgame);

    // ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
    // int half_moves = PG_GETARG_INT32(1);

    // // Calculate the full moves from half moves
    // int full_moves = half_moves / 2;

    // // Apply the specified number of half-moves
    // for (int i = 0; i < full_moves * 2; i++) {
    //     SCL_recordApplyMove(chessgame->game, i); //SCL_recordApply  use printf to check log in potgres folder
    // }

    // PG_RETURN_CHESSGAME_P(chessgame);
}

/*****************************************************************************/
// - getFirstMoves(chessgame, integer) -> chessgame: Returns the
// chessgame truncated to its first N half-moves. This function may also
// be called getOpening(...). Again the integer parameter is zero based.

/*
to-do:
    use the library called strtok to split the string
    then you will have the number of tokens
    then trim the string to the number of half moves
    then return the string 
*/
// narmina
// PG_FUNCTION_INFO_V1(getFirstMoves);
// Datum getFirstMoves(PG_FUNCTION_ARGS){
//     ChessGame *game = PG_GETARG_CHESSGAME_P(0);
//     int num_half_moves=PG_GETARG_INT32(1);
//     char *state = palloc0(sizeof(char) * (num_half_moves + 1)); // Allocate memory for the state
//     SCL_SquareSet moves;
//     for(int i = 0; i < num_half_moves; i++){
//         SCL_boardGetMoves(game, i, &moves);
//         state[i] = moves[0]; // Assuming moves is an array and you want the first character
//     }   
//     ChessGame *result; // no need for string if u have chessgames
//     result=str_to_chessgame(state);
//     PG_RETURN_CHESSGAME_P(result);
// }

/*****************************************************************************/
// - hasOpening(chessgame, chessgame) -> bool: Returns true if the first
// chess game starts with the exact same set of moves as the second
// chess game. The second parameter should not be a full game, but
// should only contain the opening moves that we want to check for,
// which can be of any length, i.e., m half-moves.

// herma

// PG_FUNCTION_INFO_V1(hasOpening);

/*
to-do:
    1- SCL_recordFromPGN to convert the string to record
       use recordLength to get the number of half moves and compare

    2-use getfirstmoves to get the first n half moves
    then compare the two strings
*/

// Datum hasOpening(PG_FUNCTION_ARGS) {
//     // Retrieve pointers to ChessGame structures from the function arguments
//     ChessGame *game1 = (ChessGame *)PG_GETARG_POINTER(0);
//     ChessGame *game2 = (ChessGame *)PG_GETARG_POINTER(1);

//     // Extract the opening moves from the second chess game
//     SCL_Board board1 = game1->board;
//     SCL_Board board2 = game2->board;

//     // Estimate the phase of the opening for the specified moves
//     SCL_boardEstimatePhase boardEstimatePhase = SCL_boardEstimatePhase(board1, board2);

//     // Check if the phase estimation indicates an opening
//     bool isOpening = (phaseEstimation == SCL_PHASE_OPENING);

//     // Return the result indicating if the first chess game matches the specified opening moves
//     PG_RETURN_BOOL(isOpening);
// }

/*****************************************************************************/

// - hasBoard(chessgame, chessboard, integer) -> bool: Returns true if
// the chessgame contains the given board state in its first N
// half-moves. Only compare the state of the pieces and not compare
// the move count, castling right, en passant pieces, ...

/*
to-do:
    use getboard and apply the requested amount of moves and check if the state of the bard gives the right fen
*/
// shofi
//SCL_recordGetMove + SCL_recordAdd 
// int hasboardchess(const ChessGame *chess_game, const ChessBoard *chess_board, const int nHalfMoves) {
//   uint32_t result;
//   uint32_t toFEN;

//   // Initialize the smallchesslib board
//   SCL_Board board = SCL_BOARD_START_STATE;
//   SCL_boardInit(board);

//   // convert board into FEN (chess chess_game)
//   SCL_boardToFEN(board, chess_board->board);
//   toFEN = SCL_boardHash32(board);

//   SCL_Board board2 = SCL_BOARD_START_STATE;
//   SCL_boardInit(board2);

//   // Apply moves from the chesschess_game to the smallchesslib board
//   for (int i = 0; i < nHalfMoves; ++i) {
//     SCL_boardFromFEN(board2, chess_game->game);
//     result = SCL_boardHash32(board2);
//   }

//   // Compare the FEN strings
//   //return strcmp(result, toFEN);
//   return (result == toFEN) ? 0 : 1;
// }


// // Function to check if the chessgame contains the given board state in its first N half-moves

// PG_FUNCTION_INFO_V1(hasboard); // correct
// Datum hasboard(PG_FUNCTION_ARGS) {
//   ChessGame *game = (ChessGame *)PG_GETARG_POINTER(0);
//   ChessBoard *board = (ChessBoard *)PG_GETARG_POINTER(1);
//   int nHalfMoves = PG_GETARG_INT32(2);

//   PG_RETURN_BOOL(hasboardchess(game, board, nHalfMoves));
// }

/*****************************************************************************/

