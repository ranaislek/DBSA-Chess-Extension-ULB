/*
 * chess_extension.c
 *
 * PostgreSQL Chess Extension:
 *
 * chessgame 'PGN'
 * chessboard 'FEN'
 *
 * Author: Your Name
 */

#include <stdio.h>
#include <postgres.h>
#include <stdlib.h>

#include "utils/builtins.h"
#include "libpq/pqformat.h"

/* Include smallchesslib */
#include "smallchesslib.h"

PG_MODULE_MAGIC;

/* Structure to represent chess games */
typedef struct
{
  char *pgn; /* Portable Game Notation (PGN) */
} ChessGame;

/* Structure to represent chess boards */
typedef struct
{
  char *fen; /* Forsyth–Edwards Notation (FEN) */
} ChessBoard;

/* fmgr macros for chess types */
#define DatumGetChessGameP(X) ((ChessGame *)DatumGetPointer(X))
#define ChessGamePGetDatum(X) PointerGetDatum(X)
#define PG_GETARG_CHESSGAME_P(n) DatumGetChessGameP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSGAME_P(x) return ChessGamePGetDatum(x)

#define DatumGetChessBoardP(X) ((ChessBoard *)DatumGetPointer(X))
#define ChessBoardPGetDatum(X) PointerGetDatum(X)
#define PG_GETARG_CHESSBOARD_P(n) DatumGetChessBoardP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSBOARD_P(x) return ChessBoardPGetDatum(x)

/* Function prototypes */
static ChessGame *chessgame_make(char *pgn);
static ChessBoard *chessboard_make(char *fen);
static char *chessgame_to_str(const ChessGame *game);
static char *chessboard_to_str(const ChessBoard *board);

PG_FUNCTION_INFO_V1(chessgame_in);
Datum chessgame_in(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(chessgame_out);
Datum chessgame_out(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(chessboard_in);
Datum chessboard_in(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(chessboard_out);
Datum chessboard_out(PG_FUNCTION_ARGS);

/* Add more functions as needed */

/* Implement the functions */
static ChessGame *chessgame_make(char *pgn)
{
  ChessGame *game = palloc0(sizeof(ChessGame));
  game->pgn = pstrdup(pgn);
  return game;
}

static ChessBoard *chessboard_make(char *fen)
{
  ChessBoard *board = palloc0(sizeof(ChessBoard));
  board->fen = pstrdup(fen);
  return board;
}

static char *chessgame_to_str(const ChessGame *game)
{
  return psprintf("Chess Game: %s", game->pgn);
}

static char *chessboard_to_str(const ChessBoard *board)
{
  return psprintf("Chess Board: %s", board->fen);
}

Datum chessgame_in(PG_FUNCTION_ARGS)
{
  char *pgn = PG_GETARG_CSTRING(0);
  PG_RETURN_CHESSGAME_P(chessgame_make(pgn));
}

Datum chessgame_out(PG_FUNCTION_ARGS)
{
  ChessGame *game = PG_GETARG_CHESSGAME_P(0);
  char *result = chessgame_to_str(game);
  PG_FREE_IF_COPY(game, 0);
  PG_RETURN_CSTRING(result);
}

Datum chessboard_in(PG_FUNCTION_ARGS)
{
  char *fen = PG_GETARG_CSTRING(0);
  PG_RETURN_CHESSBOARD_P(chessboard_make(fen));
}

Datum chessboard_out(PG_FUNCTION_ARGS)
{
  ChessBoard *board = PG_GETARG_CHESSBOARD_P(0);
  char *result = chessboard_to_str(board);
  PG_FREE_IF_COPY(board, 0);
  PG_RETURN_CSTRING(result);
}

/* Add more functions as needed */
