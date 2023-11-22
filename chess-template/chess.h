/*
 * chess.h 
 *
 * PostgreSQL Chess Type:
 *
 * chess header file
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

//header file for chess.c
#ifndef CHESS_H
#define CHESS_H

//include the provided smallchesslib.h file
#include "smallchesslib.h"
#include "stdio.h"
#include "postgres.h"
#include "stdlib.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;


/*****************************************************************************/

/* Structure to represent chess game */
//Data types: chessgame, chessboard. The input/output for your types shall
//use SAN and FEN notation for chess-game and chess-board respectively.

typedef struct
{
  char *game;
  int turn;
} ChessGame;

typedef struct
{
  char *board;
  int turn;
} ChessBoard;

/* fmgr macros chess type */
#define DatumGetChessGameP(X)  ((ChessGame *) DatumGetPointer(X))
#define ChessGamePGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_CHESSGAME_P(n) DatumGetChessGameP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSGAME_P(x) return ChessGamePGetDatum(x)

/*****************************************************************************/
Datum chessgame_in(PG_FUNCTION_ARGS);
Datum chessgame_out(PG_FUNCTION_ARGS);
Datum chessboard_in(PG_FUNCTION_ARGS);
Datum chessboard_out(PG_FUNCTION_ARGS);
Datum getBoard(PG_FUNCTION_ARGS);
Datum getFirstMoves(PG_FUNCTION_ARGS);
Datum hasOpening(PG_FUNCTION_ARGS);
Datum hasBoard(PG_FUNCTION_ARGS);

static char *chessgame_to_str(const ChessGame *c);
static char *chessboard_to_str(const ChessBoard *c);

/*****************************************************************************/

#endif

