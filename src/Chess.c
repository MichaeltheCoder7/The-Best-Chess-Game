/* Chess.c */
/*removing the AI functionality from the chess program*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "Menu.h"
#include "Board.h"
#include "Help.h"
#include "CheckMove.h"
#include "Checkmate.h"
#include "Chess.h"

/*
char board[8][8] = {

						
							{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
							{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},			
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
							{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},


						};
*/
						
	
const char positions[8][8][3] = 	{
								{"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"},
								{"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7"},
								{"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6"},
								{"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5"},
								{"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4"},
								{"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3"},
								{"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2"},
								{"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"},
						   		};

//global varibles as castling flags
int WhiteKing = 0;
int WRook1 = 0;
int WRook2 = 0;
int BlackKing = 0;
int BRook1 = 0;
int BRook2 = 0;

int chess(int colorOption, char movement[9], char board[8][8], int PlayerNum)
{
	int repeat = 0;
	int y = 0;
	char opponentCP[3], opponentNP[3];
	char current_position[3], new_position[3];
	int player = 0;
	int round = 0;
	FILE *file;
	int legalFLAG = 0;
	int checkFLAG = 0, checkmateFLAG = 0;
	char white_piece;
	char black_piece;
	int color = 0;
	int color_checkx = 0;
	int color_checky = 0;
	int xx = 0, yy = 0;
	int suicide_king = 0;
	int CASTLINGFLAG = 0;
	char check_capture;
	int value = 0;
	char temp2 = ' ';
	char opponentPiece = ' ';;
	
	
	system("clear");
	printf("\nWelcome to 'The Best Chess Game Ever!'\n\n");
  	//if(start == 1){
		do
		{
			if (colorOption == 1) 
			{	player = 1;   /* White Piece */

			}
			else 
			{
				player = 2;   /*Black Piece */
			}
			if (colorOption != 1 && colorOption != 2)
				printf("Invalid Input!\n");

			printf("\n");

		} while (colorOption != 1 && colorOption != 2);
	
		system("clear");

		/* Displays Selections */
		printf("\n****************** Selections ******************\n");
		printf("Player vs Player\n");
		if (colorOption == 1) {
			printf("Player 1: White Piece (Upper Case)\n");
			printf("Player 2: Black Piece (Lower Case)\n");
		}
		else if (colorOption == 2) {
                        printf("Player 1: Black Piece (Lower Case)\n");
                        printf("Player 2: White Piece (Upper Case)\n");
        }
		printf("************************************************\n\n");

	
		file = fopen("./bin/ChessHistory.txt", "w"); /* Makes txt file */	
	
		system("clear");
		help(); /* Displays the rules of the game */
		definitions();
		round = 1;
	//}//end if
	displayboard(board); /* Displays the current Board in play */
	
	if (repeat == 1)
	{
		//printf("         ~ Round %d ~\n", round);
		if (PlayerNum == 1) 
			printf("            White \n");
		else if (PlayerNum == 2) 
			printf("            Black \n");

		//PrintMenu(); //Displays menu options for the game
	}
		

	//check for a draw/stalemate in PVP
	if(check_draw(board, positions, PlayerNum) == 1){
		printf("There is a stalemate! The game will end in a draw.\n");
		value = 4;
		return value;
		exit(0);
	}
	//get the user input
	//fgets(movement, large, stdin);
	printf("movement: %s\n", movement);
	//printf("length: %d\n", strlen(movement));
				
	if((strlen(movement) - 1) > 8){
		printf("Incorrent Format! Please try again\n");
		displayboard(board);
		printf("Check1\n");
		value = 1;
		return value;
	}
	/* Checks for valid input for MOVEMENT STRING */
	else if ((strlen(movement)) != 8 || movement[2] != ' ' || movement[3] != 't' || movement[4] != 'o' ||
			movement[5] != ' ')
	{
		printf("Incorrect Format! Please try again\n");
		displayboard(board);
		printf("check2\n");
		value = 1;
		return value;
	}
	else if ((movement[0] < 'A' || movement[0] > 'H') || (movement[1] < '1' || movement[1] > '8') ||
					 (movement[6] < 'A' || movement[6] > 'H') || (movement[7] < '1' || movement[7] > '8'))
	{
		printf("Incorrect Format! Please try again\n");
        displayboard(board);
		printf("check3\n");
		value = 1;
        return value;
	}
			
	//parse the string
	sscanf(movement, "%s to %s", current_position, new_position);
	//printf("new position: %s\nlength new position: %d\n", new_position, strlen(new_position));
	//printf("current position: %s\nlength current position: %d\n", current_position, strlen(current_position));
				
	//check to make sure players are only moving their pieces
	if (PlayerNum == 1) {
		for(int c = 0; c<8; c++){
			for(int d = 0; d<8; d++){
				color = strcmp(current_position, positions[c][d]);
				if(color == 0){
					color_checkx = c;
					color_checky = d;
				}
			}
		}
		white_piece = board[color_checkx][color_checky];
		if (white_piece != 'P' &&  white_piece != 'R' && white_piece != 'Q' && white_piece != 'K' &&  
			white_piece != 'B' && white_piece != 'N' && white_piece != ' ')
		{
			printf("Invalid move! You cannot move opponents pieces!\n");
			value = 2;
			displayboard(board);
			return value;
		}
	}
			
	if (PlayerNum == 2) { //meaning colorOption is 2 so the player is black
		for(int c = 0; c<8; c++){
			for(int d = 0; d<8; d++){
				color = strcmp(current_position, positions[c][d]);
				if(color == 0){
					color_checkx = c;
					color_checky = d;
				}
			}
		}
		black_piece = board[color_checkx][color_checky];
		if (black_piece != 'p' && black_piece != 'r' && black_piece != 'q' && black_piece != 'k' && 
			black_piece != 'b' && black_piece != 'n' && black_piece != ' ')
		{
			printf("Invalid move! You cannot move opponents' pieces!\n");
			displayboard(board);
			value = 2;
			return value;
		}
					
	}

	//check if a player ignores check condition			
	if(checkFLAG == 1)
	{
		if(ignore_check(board, positions, current_position, new_position, PlayerNum) == 1)
		{
			legalFLAG = 1;
		}
		else
		{
			legalFLAG = CheckPiece(positions, board, current_position, new_position);
			if (legalFLAG == 1)
			{
				if(PlayerNum == 1)
				{
					legalFLAG = BlackEnpassant(opponentNP, opponentCP, opponentPiece, new_position, current_position, temp2, board, positions);
				}
				else if(PlayerNum == 2)
				{
					legalFLAG = WhiteEnpassant(opponentNP, opponentCP, opponentPiece, new_position, current_position, temp2, board, positions);
				}
			}
																		
		}
						
	}

	//Check if the moves valid 
	else
	{
		//legalFLAG is equal to the ifLegal flag returned from CheckPiece
		legalFLAG = CheckPiece(positions, board, current_position, new_position);
		if (legalFLAG == 1)
		{
			if(PlayerNum == 1)
			{
				legalFLAG = BlackEnpassant(opponentNP, opponentCP, opponentPiece, new_position, current_position, temp2, board, positions);
			}
			else if(PlayerNum == 2)
			{
				legalFLAG = WhiteEnpassant(opponentNP, opponentCP, opponentPiece, new_position, current_position, temp2, board, positions);
			}
		}
	}
	//see if the king is trying to place himself in checkmate
	suicide_king = restrict_king(board, positions, PlayerNum, current_position, new_position);
			
	//changing position to a piece
	check_capture = position_to_piece(board, positions, new_position);

	//update the board only when it is legal
	//0 for legal and 1 for illegal
	if(legalFLAG == 0)
	{
		if(suicide_king == 1){
			printf("King cannot sacrifice itself! Please try again.\n");
			displayboard(board);
			value = 2;
			return value;
		}
		printf("Legal move!\n");
		if(check_capture != ' '){
			printf("An enemy piece has been captured!\n");
		}
		updateboard(current_position, new_position, board, positions);
					
		//move the rook for player castling
		if(strcmp(current_position, "E1") == 0 &&  strcmp(new_position, "G1") == 0)
		{
			updateboard("H1", "F1", board, positions);
			CASTLINGFLAG = 1;
		}
		else if(strcmp(current_position, "E1") == 0 && strcmp(new_position, "C1") == 0)
		{
			updateboard("A1", "D1", board, positions);
			CASTLINGFLAG = 2;
		}
		else if(strcmp(current_position, "E8") == 0 && strcmp(new_position, "G8") == 0)
		{
			updateboard("H8", "F8", board, positions);
			CASTLINGFLAG = 3;
		}
		else if(strcmp(current_position, "E8") == 0 && strcmp(new_position, "C8") == 0)
		{
			updateboard("A8", "D8", board, positions);
			CASTLINGFLAG = 4;
		}
          
		/* White Pawn Promotion */
		for (y = 0; y < 8; y++)
		{
  			if (new_position[1] == '8' && board[0][y] == 'P')
			{
                PromotionWhite(board, y);
            }
		}

	  	/* Black Pawn Promotion */
        for (y = 0; y < 8; y++)
        {
            if (new_position[1] == '1' && board[7][y] == 'p')
            {
                PromotionBlack(board, y);
            }
              				
        }
				
        displayboard(board);

	}

	else if(legalFLAG == 1) /*ifLegal is 1*/
	{	
		printf("Illegal move! Please try again\n");
		displayboard(board);
		value = 2;
		return value;
	}
	else if(legalFLAG == 2) //ifLegal is 2
	{
		printf("Invalid input! Please try again\n");
		displayboard(board);
		value = 2;
		return value;
	}
					
	//Check if kings and rooks have moved for castling
	if(board[7][4] == ' ')
	{
		WhiteKing = 1;
	}
	if(board[7][0] == ' ')
	{
		WRook1 = 1;
	}
	if(board[7][7] == ' ')
	{
		WRook2 = 1;
	}
	if(board[0][4] == ' ')
	{
		BlackKing = 1;
	}
	if(board[0][0] == ' ')
	{
		BRook1 = 1;
	}
	if(board[0][7] == ' ')
	{
		BRook2 = 1;
	}

	//check if opponent's king is in check after a move and set the checkFLAG
	checkFLAG = ifCheck(board, positions, PlayerNum);
	checkmateFLAG = isCheckmate(board, positions, PlayerNum);

	//tell player if they are in check
	if(checkFLAG == 1 && checkmateFLAG == 0)
	{
		printf("Check!\n");
	}
					
	// Player vs Player 
	if (PlayerNum == 1)
	{
		fputs("***** Round ", file);
		fprintf(file, "%d *****\n\n", round);
		fputs("White: ", file);
		fputs(movement, file);
		fputs("\n", file);
		PlayerNum++;       
    }
					
	else if (PlayerNum == 2)
	{
		/* New movement for Black Piece */
		fputs("Black: ", file);
		fputs(movement, file);
		fputs("\n\n\n", file);
		round++;
		PlayerNum = 1;	
	}

	// Opponents piece 
	strcpy(opponentNP, new_position);
    strcpy(opponentCP, current_position);

	for (int c = 0; c<8; c++){
		for (int d = 0; d<8; d++){
            if (!strcmp(opponentNP, positions[c][d]))
            {
                xx = c;
                yy = d;
            }
        }
    }

    opponentPiece = board[xx][yy];

    strcpy(current_position, "");
    strcpy(new_position, "");
    strcpy(movement, "");


	//checkmate code:
	//PlayerNum is alreay swapped at this point
	//returning 3 means exit
	if(checkmateFLAG == 1 && PlayerNum == 2)
	{
		printf("Checkmate!\n");
		printf("White wins!\n\n");
		fputs("Checkmate!\nWhite wins!\n\n", file);
		value = 3;
		return value;
		exit(0);
	}
	else if(checkmateFLAG == 1 && PlayerNum == 1)
	{
		printf("Checkmate!\n");
		printf("Black wins!\n\n");
		fputs("Checkmate!\nBlack wins!\n\n", file);
		value = 3;
		return value;
		exit(0);
	}

	//break;
         
			/*case 2: // Help
				printf("\n");
				help();
				definitions();
				displayboard(board);
				value = 2;
				return value;
				break;

			case 3:
				value = 3;
				return value;
				break;

			default:
				if (repeat == 1)
				{
					printf("Invalid Selection\n\n");
					displayboard(board);
				}		
				break;
		}*/
	
	repeat = 1;
	
	//} while (selection != 3);
	
    fclose(file);
	return value;
}
