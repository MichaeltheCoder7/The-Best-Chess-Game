#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

//declare global varibles for castling
extern int WhiteKing;
extern int BlackKing;
extern int WRook1;
extern int WRook2;
extern int BRook1;
extern int BRook2;
extern int castlingFLAG;

int CheckPiece(const char positions[8][8][3], char board[8][8], char current_position[3], char new_position[3]);

void PromotionBlack(char board[8][8], int x);

void PromotionWhite(char board[8][8], int x);

int BlackEnpassant(char opponentNP[3], char opponentCP[3], char opponentPiece, char new_position[3], char current_position[3], char piece, char board[8][8],const char positions[8][8][3]);

int WhiteEnpassant(char opponentNP[3], char opponentCP[3], char opponentPiece, char new_position[3], char current_position[3], char piece, char board[8][8],const char positions[8][8][3]);


#endif /*CHESS_PIECES_H */ 
