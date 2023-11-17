-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION chess_extension" to load this file. \quit

/******************************************************************************
 * Input/Output
 ******************************************************************************/

CREATE OR REPLACE FUNCTION chessgame_in(cstring)
  RETURNS chessgame
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessgame_out(chessgame)
  RETURNS cstring
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessgame_recv(internal)
  RETURNS chessgame
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessgame_send(chessgame)
  RETURNS bytea
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE TYPE chessgame (
  internallength = 128,
  input          = chessgame_in,
  output         = chessgame_out,
  receive        = chessgame_recv,
  send           = chessgame_send,
  alignment      = double
);

CREATE OR REPLACE FUNCTION chessgame(text)
  RETURNS chessgame
  AS 'MODULE_PATHNAME', 'chessgame_cast_from_text'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION text(chessgame)
  RETURNS text
  AS 'MODULE_PATHNAME', 'chessgame_cast_to_text'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE CAST (text AS chessgame) WITH FUNCTION chessgame(text) AS IMPLICIT;
CREATE CAST (chessgame AS text) WITH FUNCTION text(chessgame);

/******************************************************************************
 * Constructor
 ******************************************************************************/

CREATE FUNCTION chessgame(integer)
  RETURNS chessgame
  AS 'MODULE_PATHNAME', 'chessgame_constructor'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/*****************************************************************************
 * Accessing values
 *****************************************************************************/

CREATE FUNCTION getBoard(chessgame, integer)
  RETURNS chessboard
  AS 'MODULE_PATHNAME', 'getBoard'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION getFirstMoves(chessgame, integer)
  RETURNS chessgame
  AS 'MODULE_PATHNAME', 'getFirstMoves'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION hasOpening(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'hasOpening'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION hasBoard(chessgame, chessboard, integer)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'hasBoard'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/*****************************************************************************
 * Operators
 *****************************************************************************/

CREATE FUNCTION chessgame_eq(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_eq'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_ne(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_ne'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_lt(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_lt'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_gt(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_gt'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_le(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_le'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_ge(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_ge'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OPERATOR = (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_eq,
  COMMUTATOR = =, NEGATOR = <>
);

CREATE OPERATOR <> (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_ne,
  COMMUTATOR = <>, NEGATOR = =
);

CREATE OPERATOR < (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_lt,
  COMMUTATOR = >, NEGATOR = >=
);

CREATE OPERATOR > (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_gt,
  COMMUTATOR = <, NEGATOR = <=
);

CREATE OPERATOR <= (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_le,
  COMMUTATOR = >=, NEGATOR = >
);

CREATE OPERATOR >= (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_ge,
  COMMUTATOR = <=, NEGATOR = <
);

/******************************************************************************/

CREATE FUNCTION chessboard_in(cstring)
  RETURNS chessboard
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessboard_out(chessboard)
  RETURNS cstring
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessboard_recv(internal)
  RETURNS chessboard
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessboard_send(chessboard)
  RETURNS bytea
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE TYPE chessboard (
  internallength = 64,
  input          = chessboard_in,
  output         = chessboard_out,
  receive        = chessboard_recv,
  send           = chessboard_send,
  alignment      = double
);

CREATE FUNCTION chessboard(text)
  RETURNS chessboard
  AS 'MODULE_PATHNAME', 'chessboard_cast_from_text'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION text(chessboard)
  RETURNS text
  AS 'MODULE_PATHNAME', 'chessboard_cast_to_text'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE CAST (text AS chessboard) WITH FUNCTION chessboard(text) AS IMPLICIT;
CREATE CAST (chessboard AS text) WITH FUNCTION text(chessboard);

/******************************************************************************/

CREATE FUNCTION complex_add(chessboard, chessboard)
  RETURNS chessboard
  AS 'MODULE_PATHNAME', 'chessboard_add'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION complex_sub(chessboard, chessboard)
  RETURNS chessboard
  AS 'MODULE_PATHNAME', 'chessboard_sub'
  LANGUAGE C IMMUTABLE STRICT PAR
