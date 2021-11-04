#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Board.h"

/*displays the original board*/	
void displayboard(char board[8][8]){
	
	int j = 8; /*row numbers*/
	printf("\n    A   B   C   D   E   F   G   H\n");
	/*nested loops so I can print the board*/
	for(int i = 0; i < 8; i++){
		printf("  +---+---+---+---+---+---+---+---+\n");
		printf(" %d", j);
		for(int k = 0; k < 8; k++){
			if(k == 7){
				if(isupper(board[i][k])){
					printf("| %c |", board[i][k]);
				}
				else{
					printf("| ");
					printf("\033[0;31m");
					printf("%c", board[i][k]);
					printf("\033[0m");
					printf(" |");
				}
			}
			else{
				if(isupper(board[i][k])){
					printf("| %c ", board[i][k]);
				}
				else{
					printf("| ");
					printf("\033[0;31m");
					printf("%c ", board[i][k]);
					printf("\033[0m");
				}
			}
		}
		printf("%d", j);
		printf("\n");
		j--;
	}
	printf("  +---+---+---+---+---+---+---+---+\n");
	printf("    A   B   C   D   E   F   G   H\n\n");
}
/*displays the board as the players make moves*/
void updateboard(char current_position[3], char new_position[3], char board[8][8], const char positions[8][8][3]){
	int compare_current = 0;
	int current_x = 0;
	int current_y = 0;
	int compare_new = 0;
	int new_x = 0;
	int new_y = 0;
	/*finding the location of the current position*/
	for(int a = 0; a<8; a++){
		for(int b = 0; b<8; b++){
			compare_current = strcmp(current_position, positions[a][b]);
			if(compare_current == 0){
				/*position in board where new piece will go*/
				current_x = a;
				current_y = b;
			}
		}
	}
	/*finding the location of the new position*/
	for(int i = 0; i<8; i++){
		for(int j = 0; j<8; j++){
			compare_new = strcmp(new_position, positions[i][j]);
			if(compare_new == 0){
				/*position in board where new piece will go*/
				new_x = i;
				new_y = j;
			}
		}
	}
	/*make the new position equal to the current position, then make current position blank*/
	board[new_x][new_y] = board[current_x][current_y]; /*new is now the current*/
	board[current_x][current_y] = ' ';
	

}

