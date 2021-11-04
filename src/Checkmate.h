#ifndef CHECKMATE_H
#define CHECKMATE_H

//check if check 
//return 1 if yes
//return 0 if no
//color = 1: check if black king is in check
//color = 2: check if white king is in check

int ifCheck(char board[8][8], const char positions[8][8][3], int color);

//check if a piece in a given position is threatened
//return 1 if yes
//return 0 if no
//error if empty
//color = 1: check if a position is threatened by white pieces
//color = 2: check if a position is threatened by black pieces
int isThreatened(char board[8][8], const char positions[8][8][3], char location[3], int color);

//get a position string and translate it to a piece character on the board
char position_to_piece(char board[8][8], const char positions[8][8][3], char current_position[3]);

//save one side's all positions in an array of string
//need caller to declare: char opponent[16][3] array first and pass it in
//the array has a fixed size
//color = 1: white pieces
//color = 2: black pieces
void opponent_positions(char board[8][8], const char positions[8][8][3], char opponent[16][3], int color);

//get the length of the all_moves string array
//length is also the number of legal moves for a piece at the initial_position
int getLength(char board[8][8], const char positions[8][8][3], char initial_position[3]);

//given a position of a piece and save all possible legal moves of that piece in a string array
//need caller call getLength and pass the length into the function
//return the array
//the array is dynamic so need to use void free_all_moves(char** all_moves, int length) when done
char** get_all_moves(char board[8][8], const char positions[8][8][3], char initial_position[3], int length);

//free the dynamic array
void free_all_moves(char** all_moves, int length);

int getLength2(char board[8][8], const char positions[8][8][3], char initial_position[3]);

char** get_all_legal_moves(char board[8][8], const char positions[8][8][3], char initial_position[3], int length);

//check if checkmate
//return 1 if yes
//return 0 if no
//color = 1: check black king
//color = 2: check white king 
int isCheckmate(char board[8][8], const char positions[8][8][3], int color);

//see if check condition is ignored
//return 1 if yes(illegal)
//return 0 if no(legal)
//color: 1 for white
//2 for black
int ignore_check(char board[8][8], const char positions[8][8][3], char old_position[3], char new_position[3], int color);

int draw_Checkmate(char board[8][8], const char positions[8][8][3], int color);

//restricts king from putting itself in checkmate
int restrict_king(char board[8][8], const char positions[8][8][3], int color, char current_position[3], char location[3]);

//returns 1 if there is a draw otherwise returns 0
int check_draw(char board[8][8], const char positions [8][8][3], int color);

#endif
