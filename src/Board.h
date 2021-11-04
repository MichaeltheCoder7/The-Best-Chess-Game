#ifndef BOARD__H
#define BOARD__H

//displaying the board
void displayboard(char board[8][8]);

//displayed updated board after the players make moves
void updateboard(char current_position[3], char new_position[3], char board[8][8], const char positions[8][8][3]);

#endif
