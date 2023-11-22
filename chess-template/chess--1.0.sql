/*
 * chess--1.0.sql 
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

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION chess" to load this file. \quit

/******************************************************************************
 * Input/Output
 ******************************************************************************/

 CREATE OR REPLACE FUNCTION chess_in(cstring)
   RETURNS chessgame
   AS 'MODULE_PATHNAME'
   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chess_out(chessgame)
    RETURNS cstring
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chess_recv(internal)
    RETURNS chessgame
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chess_send(chessgame)
    RETURNS bytea
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE TYPE chessgame (
    internallength = 16,
    input          = chess_in,
    output         = chess_out,
    receive        = chess_recv,
    send           = chess_send,
    alignment      = double
);

CREATE OR REPLACE FUNCTION chess(text)
    RETURNS chessgame
    AS 'MODULE_PATHNAME', 'chess_cast_from_text'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION text(chessgame)
    RETURNS text
    AS 'MODULE_PATHNAME', 'chess_cast_to_text'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE CAST (text as chessgame) WITH FUNCTION chess(text) AS IMPLICIT;
CREATE CAST (chessgame as text) WITH FUNCTION text(chessgame);

/******************************************************************************
 * Constructor
 ******************************************************************************/

CREATE FUNCTION chess(text)
    RETURNS chessgame
    AS 'MODULE_PATHNAME', 'chess_constructor'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/******************************************************************************
    * Accessing values
    ******************************************************************************/ 

CREATE FUNCTION getBoard(chessgame, integer)
    RETURNS chessboard
    AS 'MODULE_PATHNAME', 'getBoard'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION getFirstMoves(chessgame, integer)
    RETURNS chessgame
    AS 'MODULE_PATHNAME', 'getFirstMoves'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION hasOpening(chessgame, chessgame)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'hasOpening'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION hasBoard(chessgame, chessboard, integer)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'hasBoard'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/******************************************************************************
    * Indexing
    ******************************************************************************/