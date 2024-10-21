/*
 * chess--1.0.sql 
 *
 * PostgreSQL Chess Type:
 *
 * chess
 *
 * Authors: 
 * Rana Islek <rana.islek@ulb.be>
 * Shofiyyah Nadhiroh <shofiyyah.nadhiroh@ulb.be>
 * Narmina Mahmudova <narmina.mahmudova@ulb.be>
 * Herma Elezi <herma.elezi@ulb.be>
 */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION chess" to load this file. \quit

CREATE TYPE chessgame;
CREATE TYPE chessboard;

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

CREATE TYPE chessgame (
    internallength = 512, -- SCL_Record size
    input          = chessgame_in,
    output         = chessgame_out
);

CREATE OR REPLACE FUNCTION chessboard_in(cstring)
    RETURNS chessboard
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessboard_out(chessboard)
    RETURNS cstring
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE TYPE chessboard (
    internallength = 69, -- SCL_Board size
    input          = chessboard_in,
    output         = chessboard_out
);


/******************************************************************************
 * Accessing values
 ******************************************************************************/

CREATE FUNCTION getBoard(chessgame, int)
    RETURNS chessboard
    AS 'MODULE_PATHNAME', 'getBoard'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
    
CREATE FUNCTION getFirstMoves(chessgame, int)
    RETURNS chessgame
    AS 'MODULE_PATHNAME', 'getFirstMoves'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION hasOpening(game chessgame, opening_game chessgame)
  RETURNS boolean
  AS $$
     SELECT game = opening_game;  -- calls chess_eq function
  $$
  LANGUAGE SQL;

CREATE FUNCTION hasBoard(chessgame, chessboard, int)
    RETURNS boolean
    AS '$libdir/chess', 'hasBoard'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/******************************************************************************
 * Indexing
 ******************************************************************************/

/* B-Tree comparison functions */

CREATE OR REPLACE FUNCTION chess_eq(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chess_eq'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chess_lt(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chess_lt'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chess_le(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chess_le'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chess_gt(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chess_gt'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chess_ge(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chess_ge'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;


/**************************/

/* B-Tree comparison operators */

CREATE OPERATOR = (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chess_eq,
  COMMUTATOR = =, NEGATOR = <>
);
CREATE OPERATOR < (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chess_lt,
  COMMUTATOR = >, NEGATOR = >=
);
CREATE OPERATOR <= (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chess_le,
  COMMUTATOR = >=, NEGATOR = >
);
CREATE OPERATOR >= (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chess_ge,
  COMMUTATOR = <=, NEGATOR = <
);
CREATE OPERATOR > (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chess_gt,
  COMMUTATOR = <, NEGATOR = <=
);

/**************************/

/* B-Tree support function */

CREATE OR REPLACE FUNCTION chess_cmp(chessgame, chessgame)
  RETURNS integer
  AS 'MODULE_PATHNAME', 'chess_cmp'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/**************************/

/* B-Tree operator class */

CREATE OPERATOR CLASS chess_ops
DEFAULT FOR TYPE chessgame USING btree
AS
        OPERATOR        1       <  ,
        OPERATOR        2       <= ,
        OPERATOR        3       =  ,
        OPERATOR        4       >= ,
        OPERATOR        5       >  ,
        FUNCTION        1       chess_cmp(chessgame, chessgame);

/**************************/

/******************************************************************************
 * GIN Indexing
 ******************************************************************************/
 -- GIN Index
-- CREATE INDEX idx_chessgame_boards
-- ON chessgame USING GIN (getBoard(chessgame, 0));

CREATE FUNCTION compare(chessboard, chessboard)
    RETURNS int4
    AS 'MODULE_PATHNAME', 'compare'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION gin_extract_value(chessgame, internal)
    RETURNS internal
    AS 'MODULE_PATHNAME', 'gin_extract_value'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION gin_extract_query(chessgame, internal, int2, internal, internal, internal, internal)
    RETURNS internal
    AS 'MODULE_PATHNAME', 'gin_extract_query'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION gin_consistent(internal, int2, text, int4, internal, internal, internal, internal)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hasboardcheck(game chessgame, board chessboard)
    RETURNS BOOLEAN
    LANGUAGE plpgsql
    AS $$
BEGIN
    RETURN true;
END;
$$;

CREATE OPERATOR @> (
    PROCEDURE = hasboardcheck,
    LEFTARG = chessgame,
    RIGHTARG = chessboard,
    COMMUTATOR = =,
    NEGATOR = <>
);


CREATE OPERATOR CLASS chess_gin_ops
FOR TYPE chessgame USING gin AS
  OPERATOR  1    @>(chessgame, chessboard),
  FUNCTION	1	 compare(chessboard, chessboard),
  FUNCTION  2    gin_extract_value(chessgame, internal),
  FUNCTION  3    gin_extract_query(chessgame, internal, int2, internal, internal, internal, internal),
  FUNCTION	4	 gin_consistent (internal, int2, text, int4, internal, internal, internal, internal);
