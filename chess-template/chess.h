/*
 * chess.h 
 *
 * PostgreSQL Chess Type:
 *
 * chess header file
 *
 * Author: Rana Islek <rana.islek@ulb.be>
 */

#ifndef CHESS_H
#define CHESS_H

#include "smallchesslib.h"
#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/memutils.h"

PG_MODULE_MAGIC;

/* DATA TYPES: FOR GAME AND BOARD */
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

// FMGR macros for chess type 
#define DatumGetChessGameP(X) ((ChessGame *)DatumGetPointer(X))
#define ChessGamePGetDatum(X) PointerGetDatum(X)
#define PG_GETARG_CHESSGAME_P(n) DatumGetChessGameP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSGAME_P(x) return ChessGamePGetDatum(x)

#define DatumGetChessBoardP(X) ((ChessBoard *)DatumGetPointer(X))
#define ChessBoardPGetDatum(X) PointerGetDatum(X)
#define PG_GETARG_CHESSBOARD_P(n) DatumGetChessBoardP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSBOARD_P(x) return ChessBoardPGetDatum(x)

// Function Declarations 
Datum chessgame_in(PG_FUNCTION_ARGS);
Datum chessgame_out(PG_FUNCTION_ARGS);
Datum chessboard_in(PG_FUNCTION_ARGS);
Datum chessboard_out(PG_FUNCTION_ARGS);

Datum getBoard(PG_FUNCTION_ARGS);
Datum getFirstMoves(PG_FUNCTION_ARGS);
Datum hasOpening(PG_FUNCTION_ARGS);
Datum hasBoard(PG_FUNCTION_ARGS);

static ChessGame *str_to_chessgame(const char *str);
static ChessBoard *str_to_chessboard(char *str);
#endif
