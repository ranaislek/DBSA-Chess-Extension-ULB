#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smallchesslib.h"

// Structure to represent chess game -SAN/PGN
typedef struct 
{
    /* data */
    SCL_Record game;
} ChessGame;

// Structure to represent chess board -FEN
typedef struct 
{
    /* data */
    SCL_Board board;
} ChessBoard;

//*****************************************************************************
// 1- Function to get the board state at a given half-move
// - getBoard(chessgame, integer) -> chessboard: Return the board state
// at a given half-move (A full move is counted only when both players
// have played). The integer parameter indicates the count of half
// moves since the beginning of the game. A 0 value of this parameter
// means the initial board state, i.e.,(rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1).

/*
    to-do:
       get pgn (chessgame?) string and put it in the record
       use recordapply (record, board, halfmove)
       get pgn string to the number of half moves, and get the state of the board
       use boardtofen to convert the board state to fen (chessboard?)
       return fen
    */

static ChessGame *getBoard(ChessGame* chessgame, int half_moves)
{
    ChessGame *result = (ChessGame*)malloc(sizeof(ChessGame));
    ChessBoard *board = (ChessBoard*)malloc(sizeof(ChessBoard));
    SCL_recordApply(chessgame->game, board->board, half_moves);
    char fen[100];
    SCL_boardToFEN(board->board, fen);
    return fen;
}

//*****************************************************************************

int main()
{
    ChessGame *chessgame = (ChessGame*)malloc(sizeof(ChessGame));
    char pgn[100] = "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6";
    SCL_recordFromPGN(chessgame->game, pgn);

    //print the state of the board
    printf("The state of the board is: \n");
    printf("%s\n", getBoard(chessgame, 3));
    return 0;
}
