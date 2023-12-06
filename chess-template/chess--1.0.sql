/*
 * chess--1.0.sql 
 *
 * PostgreSQL Chess Type:
 *
 * chess
 *
 * Authors: 
 Rana Islek <rana.islek@ulb.be>
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

CREATE FUNCTION hasOpening(chessgame, chessgame)
    RETURNS boolean
    AS '$libdir/chess', 'hasOpening'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION hasBoard(chessgame, chessboard, int)
    RETURNS boolean
    AS '$libdir/chess', 'hasBoard'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/******************************************************************************
 * Indexing
 ******************************************************************************/

-- Add your indexing definitions here, if needed.
