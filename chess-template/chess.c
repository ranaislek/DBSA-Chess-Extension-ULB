/*
 * chess.c 
 *
 * PostgreSQL Chess Type:
 *
 * chess
 *
 * Author: Rana Islek <rana.islek@ulb.be>
 */

/*

Your extension will implement data types, functions and indexes as follows:
Data types: chessgame, chessboard. The input/output for your types shall
use SAN and FEN notation for chess-game and chess-board respectively.

Functions:
- getBoard(chessgame, integer) -> chessboard: Return the board state
at a given half-move (A full move is counted only when both players
have played). The integer parameter indicates the count of half
moves since the beginning of the game. A 0 value of this parameter
means the initial board state, i.e.,(rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1).

- getFirstMoves(chessgame, integer) -> chessgame: Returns the
chessgame truncated to its first N half-moves. This function may also
be called getOpening(...). Again the integer parameter is zero based.

- hasOpening(chessgame, chessgame) -> bool: Returns true if the first
chess game starts with the exact same set of moves as the second
chess game. The second parameter should not be a full game, but
should only contain the opening moves that we want to check for,
which can be of any length, i.e., m half-moves.

- hasBoard(chessgame, chessboard, integer) -> bool: Returns true if
the chessgame contains the given board state in its first N
half-moves. Only compare the state of the pieces and not compare
the move count, castling right, en passant pieces, ...

*/
#include "chess.h"
/*****************************************************************************/

/* Structure to represent chess game */
//Data types: chessgame, chessboard. The input/output for your types shall
//use SAN and FEN notation for chess-game and chess-board respectively.

//dummy functions needs to change ofc
// Convert ChessGame to string representation
static char *chessgame_to_str(const ChessGame *c) {
    char *result = palloc(strlen(c->san) + 1);
    strcpy(result, c->san);
    return result;
}

// Convert ChessBoard to string representation
static char *chessboard_to_str(const ChessBoard *c) {
    char *result = palloc(strlen(c->fen) + 1);
    strcpy(result, c->fen);
    return result;
}

//Convert string representation to ChessBoard
static ChessBoard *str_to_chessboard(const char *str) {
    ChessBoard *result = (ChessBoard *)palloc(sizeof(ChessBoard));
    
    // Assuming FEN string is directly copied
    strncpy(result->fen, str, sizeof(result->fen) - 1);
    result->fen[sizeof(result->fen) - 1] = '\0'; // Ensure null-terminated

    // You might want to parse the FEN string and set the SCL_Board accordingly

    return result;
}

// Convert string representation to ChessGame
static ChessGame *str_to_chessgame(const char *str) {
    ChessGame *result = (ChessGame *)palloc(sizeof(ChessGame));
    
    // Assuming SAN string is directly copied
    strncpy(result->san, str, sizeof(result->san) - 1);
    result->san[sizeof(result->san) - 1] = '\0'; // Ensure null-terminated

    // You might want to parse the SAN string and set the SCL_Game accordingly

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

// Output function for ChessGame
PG_FUNCTION_INFO_V1(chessgame_out);

Datum chessgame_out(PG_FUNCTION_ARGS) {
    ChessGame *game = PG_GETARG_CHESSGAME_P(0);
    char *result;

    result = chessgame_to_str(game);

    PG_RETURN_CSTRING(result);
}

// Input function for ChessBoard
PG_FUNCTION_INFO_V1(chessboard_in);
Datum chessboard_in(PG_FUNCTION_ARGS) {
    char *str = PG_GETARG_CSTRING(0);
    ChessBoard *result;

    result = str_to_chessboard(str);

    PG_RETURN_CHESSBOARD_P(result);
}

// Output function for ChessBoard
PG_FUNCTION_INFO_V1(chessboard_out);

Datum chessboard_out(PG_FUNCTION_ARGS) {
    ChessBoard *board = PG_GETARG_CHESSBOARD_P(0);
    char *result;

    result = chessboard_to_str(board);

    PG_RETURN_CSTRING(result);
}
/*****************************************************************************/

PG_FUNCTION_INFO_V1(getBoard);
Datum
getBoard(PG_FUNCTION_ARGS)
{
  ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
  int halfmove = PG_GETARG_INT32(1);
  char *result;
  result = psprintf("(%s,%d)", chessgame->game, chessgame->turn);
  PG_RETURN_CSTRING(result);
}

/*****************************************************************************/

PG_FUNCTION_INFO_V1(getFirstMoves);
Datum
getFirstMoves(PG_FUNCTION_ARGS)
{
  ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
  int halfmove = PG_GETARG_INT32(1);
  char *result;
  result = psprintf("(%s,%d)", chessgame->game, chessgame->turn);
  PG_RETURN_CSTRING(result);
}

/*****************************************************************************/

PG_FUNCTION_INFO_V1(hasOpening);
Datum
hasOpening(PG_FUNCTION_ARGS)
{
  ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
  ChessGame *chessgame2 = PG_GETARG_CHESSGAME_P(1);
  bool result;
  result = true;
  PG_RETURN_BOOL(result);
}

/*****************************************************************************/

PG_FUNCTION_INFO_V1(hasBoard);
Datum
hasBoard(PG_FUNCTION_ARGS)
{
  ChessGame *chessgame = PG_GETARG_CHESSGAME_P(0);
  ChessBoard *chessboard = PG_GETARG_CHESSGAME_P(1);
  int halfmove = PG_GETARG_INT32(2);
  bool result;
  result = true;
  PG_RETURN_BOOL(result);
}

/*****************************************************************************/

