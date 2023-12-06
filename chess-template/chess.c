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
    //ChessGame *result = (ChessGame *)palloc(sizeof(ChessGame));
    ChessGame *result = palloc0(sizeof(ChessGame));
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
    char *result = palloc0(sizeof(char)*SCL_RECORD_MAX_LENGTH);
    SCL_printPGN(game->game, result, 0);
    PG_FREE_IF_COPY(game, 0);
    PG_RETURN_CSTRING(result);
}

// Output function for ChessBoard
PG_FUNCTION_INFO_V1(chessboard_out);
Datum chessboard_out(PG_FUNCTION_ARGS) {
    ChessBoard *board = PG_GETARG_CHESSBOARD_P(0);
    char *result = palloc0(sizeof(char)*SCL_FEN_MAX_LENGTH);
    SCL_boardToFEN(board->board, result);
    PG_FREE_IF_COPY(board, 0);
    PG_RETURN_CSTRING(result);
}

/*****************************************************************************/
// 1)
// - getBoard(chessgame, integer) -> chessboard: Return the board state
// at a given half-move (A full move is counted only when both players
// have played). The integer parameter indicates the count of half
// moves since the beginning of the game. A 0 value of this parameter
// means the initial board state, i.e.,(rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1).

PG_FUNCTION_INFO_V1(getBoard);
Datum
getBoard(PG_FUNCTION_ARGS)
{
      //smallchesslib.h get board position
    ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
    ChessBoard *chessboard = palloc0(sizeof(ChessBoard));
    int half_moves = PG_GETARG_INT32(1);
    SCL_recordApply(chessgame->game, chessboard, half_moves);
    PG_FREE_IF_COPY(chessgame,0);
    PG_RETURN_CHESSBOARD_P(chessboard);
}

/*****************************************************************************/
// 2)
// - getFirstMoves(chessgame, integer) -> chessgame: Returns the
// chessgame truncated to its first N half-moves. This function may also
// be called getOpening(...). Again the integer parameter is zero based.

PG_FUNCTION_INFO_V1(getFirstMoves);
Datum 
getFirstMoves(PG_FUNCTION_ARGS)
{
    // have the input chessgame and int half_moves
    ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
    int half_moves = PG_GETARG_INT32(1);
    // get the string from the chessgame
    char *str = palloc0(sizeof(char) * SCL_RECORD_MAX_LENGTH);
    SCL_printPGN(chessgame->game, str, 0);

    // Calculate the number of full moves
    int full_moves = (int)ceil(half_moves / 2.0);
    bool even = half_moves % 2 == 0;

    // Build the search string
    StringInfoData search_str;
    initStringInfo(&search_str);
    appendStringInfo(&search_str, "%d.", full_moves);

    // Find the starting position
    int counter = (int)strstr(str, search_str.data) - (int)str;
    bool found = true;
    
    // Set the threshold
    int threshold = even ? 3 : 2;
    int nb_spaces = 0;

    // Iterate until the specified threshold is reached
    while (found) {
        counter++;
        if (str[counter] == ' ') {
            nb_spaces++;
        }
        if (nb_spaces == threshold) {
            found = false;
        }
    }

    // Return the truncated string
    char *result = palloc0(counter + 1);
    strncpy(result, str, counter);
    result[counter] = '\0';

    //convert str to chessgame
    ChessGame *result_chessgame = str_to_chessgame(result);

    // Cleanup and return the result
    PG_FREE_IF_COPY(chessgame, 0);
    pfree(str);
    pfree(result);
    PG_RETURN_CHESSGAME_P(result_chessgame);
}

/*****************************************************************************/
// 3)
// - hasOpening(chessgame, chessgame) -> bool: Returns true if the first
// chess game starts with the exact same set of moves as the second
// chess game. The second parameter should not be a full game, but
// should only contain the opening moves that we want to check for,
// which can be of any length, i.e., m half-moves.

PG_FUNCTION_INFO_V1(hasOpening);
Datum 
hasOpening(PG_FUNCTION_ARGS) {
    ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
    ChessGame *chessgame2 = PG_GETARG_CHESSGAME_P(1);

    int half_moves = SCL_recordLength(chessgame2->game);
    elog(NOTICE, "half_moves: %d", half_moves);

    char *str = palloc0(sizeof(char) * SCL_RECORD_MAX_LENGTH);
    SCL_printPGN(chessgame->game, str, 0);

    char *str2 = palloc0(sizeof(char) * SCL_RECORD_MAX_LENGTH);
    SCL_printPGN(chessgame2->game, str2, 0);

    elog(NOTICE, "str: %s", str);
    elog(NOTICE, "str2: %s", str2);

    // Get first n half moves from the first chessgame
    int full_moves = (int)ceil(half_moves / 2.0);
    bool even = half_moves % 2 == 0;

    StringInfoData search_str;
    initStringInfo(&search_str);
    appendStringInfo(&search_str, "%d.", full_moves);

    // Find the starting position
    int counter = (int)strstr(str, search_str.data) - (int)str;
    bool found = true;

    // Set the threshold
    int threshold = even ? 3 : 2;
    int nb_spaces = 0;

    // Iterate until the specified threshold is reached or the string ends
    while (found && str[counter] != '\0') {
        counter++;
        if (str[counter] == ' ') {
            nb_spaces++;
        }
        if (nb_spaces == threshold) {
            found = false;
        }
    }
    
    char *result = palloc0(counter + 1);
    strncpy(result, str, counter);
    result[counter] = '\0';

    elog(NOTICE, "result of str1 after trimming: %s", result);

    int length = strlen(result);
    bool has_opening = strncmp(result, str2, length) == 0;

    // Cleanup and return the result
    PG_FREE_IF_COPY(chessgame, 0);
    PG_FREE_IF_COPY(chessgame2, 1);
    pfree(str);
    pfree(str2);
    pfree(result);
    PG_RETURN_BOOL(has_opening);
}

/*****************************************************************************/
// 4)
// - hasBoard(chessgame, chessboard, integer) -> bool: Returns true if
// the chessgame contains the given board state in its first N
// half-moves. Only compare the state of the pieces and not compare
// the move count, castling right, en passant pieces, ...

PG_FUNCTION_INFO_V1(hasBoard);
Datum 
hasBoard(PG_FUNCTION_ARGS) 
{
    ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
    ChessBoard *chessboard = palloc0(sizeof(ChessBoard));
    ChessBoard *chessboard2 = PG_GETARG_CHESSBOARD_P(1);
    int half_moves = PG_GETARG_INT32(2);
    
    SCL_recordApply(chessgame->game, chessboard, half_moves);
    PG_FREE_IF_COPY(chessgame,0);

    uint32_t hash = SCL_boardHash32(chessboard);
    uint32_t hash2 = SCL_boardHash32(chessboard2);

    PG_RETURN_BOOL(hash == hash2);
}
/*****************************************************************************/

