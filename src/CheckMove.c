#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "CheckMove.h"
#include "Board.h"
#include "IsOccupied.h"
#include "Checkmate.h"

int castlingFLAG = 0;
//return 1-12 for each piece
int CheckPiece(const char positions[8][8][3], char board[8][8], char current_position[3], char new_position[3])
{
        castlingFLAG = 0;
	int index_x, index_y;
	int piece_number;
	for(int x = 0; x < 8; x++)
	{
		for(int y = 0; y < 8; y++)
		{
			if(strcmp(current_position, positions[x][y]) == 0)
			{
				index_x = x;
				index_y = y;
			}
		}
	}

	if(board[index_x][index_y] == 'P')
	{
		piece_number = 1;
	}
	else if(board[index_x][index_y] == 'p')
	{
		piece_number = 2;
	}
	else if(board[index_x][index_y] == 'R')
	{
		piece_number = 3;
	}
	else if(board[index_x][index_y] == 'r')
	{
		piece_number = 4;
	}
	else if(board[index_x][index_y] == 'N')
	{
		piece_number = 5;
	}
	else if(board[index_x][index_y] == 'n')
	{
		piece_number = 6;
	}
	else if(board[index_x][index_y] == 'B')
	{
		piece_number = 7;
	}
	else if(board[index_x][index_y] == 'b')
	{
		piece_number = 8;
	}
	else if(board[index_x][index_y] == 'Q')
	{
		piece_number = 9;
	}
	else if(board[index_x][index_y] == 'q')
	{
		piece_number = 10;
	}
	else if(board[index_x][index_y] == 'K')
	{
		piece_number = 11;
	}
	else if(board[index_x][index_y] == 'k')
	{
		piece_number = 12;
	}
	else if(board[index_x][index_y] == ' ')
	{
		piece_number = 13;
	}



/***************************************************************Checking moves for pieces******************************************************/

	int compare_new = 0;
	int new_x = 0;
	int new_y = 0;
	int ifLegal = 0; /*ifLegal = 0 for legal move, ifLegal = 1 for illegal move, ifLegal = 2 for invalid input, 3 for capture*/
	for(int i = 0; i<8; i++)
        {
        	for(int j = 0; j<8; j++)
        	{
                	compare_new = strcmp(new_position, positions[i][j]);
                        if(compare_new == 0)
                        {
                        	/*position in board where new piece will go*/
                                new_x = i;
                                new_y = j;
                        }
                }
       	}
	


	switch(piece_number)
	{
		case 1:		/*White Pawn*/
		{
                        /*Checking for legal move (both beginning of the game and afterwards)*/
                        if(index_x != new_x && index_y == new_y && board[new_x][new_y] == ' ')
                        {
                                if(index_x == 6 && new_x >= 4 && new_x <= 5)
                                {
                                        /*If a piece is blocking the way*/
                                        if(IsOccupied(board, index_x, index_y, new_x, new_y))
                                        {
                                                ifLegal = 1;
                                        }
                                        else
                                        {
                                                ifLegal = 0;
                                        }
                                }
                                else if(index_x != 6 && (index_x - new_x) < 2 && (index_x - new_x) > 0)
                                {
                                        ifLegal = 0;
                                }
                                else
                                {
                                        ifLegal = 1;
                                }
                        }
                        /*Capturing*/
                        else if((new_x == index_x - 1 && new_y == index_y + 1) || (new_x == index_x - 1 && new_y == index_y - 1))
                        {
                                if(board[new_x][new_y] != ' ')
                                {
                                        if((board[new_x][new_y] == 'p' || board[new_x][new_y] == 'r' || board[new_x][new_y] == 'n' || board[new_x][new_y] == 'b' || board[new_x][new_y] == 'q' || board[new_x][new_y] == 'k'))
                                        {
                                                ifLegal = 0;
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                }
                                else
                                {
                                        ifLegal = 1;
                                }

                        }                                                        

                        /* If starting position is the same as new position */
                        else if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
                        /*If the move is backwards*/
                        else if((index_x - new_x) < 0)
                        {
                                ifLegal = 1;
                        }
                        /*If a piece is occupying the new space*/
                        else
                        {
                                ifLegal = 1;
                        }


                break;
                }

		case 2:		/* Black Pawn */
		{
                        /*Checking for legal move (both beginning of the game and afterwards)*/
			if(index_x != new_x && index_y == new_y && board[new_x][new_y] == ' ')
			{
				if(index_x == 1 && new_x >= 2 && new_x <= 3)
				{
					/*If a piece is blocking the way*/
                        		if(IsOccupied(board, index_x, index_y, new_x, new_y))
                        		{
                                		ifLegal = 1;
                        		}
					else
					{
						ifLegal = 0;
					}
				}
				else if(index_x != 1 && (new_x - index_x) < 2 && (new_x - index_x) > 0)
				{
					ifLegal = 0;
				}
				else
				{
					ifLegal = 1;
				}
			}
			/*Capturing*/
			else if((new_x == index_x + 1 && new_y == index_y + 1) || (new_x == index_x + 1 && new_y == index_y - 1)) 
			{
				if(board[new_x][new_y] != ' ')
				{
					if((board[new_x][new_y] == 'P' || board[new_x][new_y] == 'R' || board[new_x][new_y] == 'N' || board[new_x][new_y] == 'B' || board[new_x][new_y] == 'Q' || board[new_x][new_y] == 'K'))
					{
						ifLegal = 0;
					}
					else
					{
						ifLegal = 1;
					}
				}
				else
				{
					ifLegal = 1;
				}
				
			}
				
                        /* If starting position is the same as new position */
                        else if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
			/*If the move is backwards*/
                        else if((new_x - index_x) < 0)
                        {
                                ifLegal = 1;
                        }
                        /*If a piece is occupying the new space*/
                        else
                        {
                                ifLegal = 1;
                        }

		
		break;
		}


		case 3:		/*White Rook*/
		{
			/*If a piece is blocking the way*/
			if(IsOccupied(board, index_x, index_y, new_x, new_y))
                        {
                        	ifLegal = 1;
                        }
                        /*If starting position = new position*/
                        else if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
			/*For legal moves, capturing*/
			else if((index_x == new_x && index_y != new_y) || (index_x != new_x && index_y == new_y))
			{
				if(board[new_x][new_y] != ' ')
				{
					if(board[new_x][new_y] == 'p' || board[new_x][new_y] == 'r' || board[new_x][new_y] == 'n' || board[new_x][new_y] == 'b' || board[new_x][new_y] == 'q' || board[new_x][new_y] == 'k')
					{
						ifLegal = 0;
					}	
					else
					{
						ifLegal = 1;
					}
				}				
				else
				{
					ifLegal = 0;
				}

			}
			else
			{
				ifLegal = 1;
			}
					                                                                                                                                                                                                                                                                                                                                                                       			                                                                                                                                                                                                                              
		break;
		}

		case 4:		/*Black Rook*/
		{
			/*If a piece is blocking the way*/
                        if(IsOccupied(board, index_x, index_y, new_x, new_y))
                        {
                        	ifLegal = 1;
                        }	
                        /*If starting position = new position*/
                        else if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 1;
                        }
			/*For legal moves and capturing*/
                        else if((index_x == new_x && index_y != new_y) || (index_x != new_x && index_y == new_y))
                        {
				if(board[new_x][new_y] != ' ')
				{
                                	if(board[new_x][new_y] == 'P' || board[new_x][new_y] == 'R' || board[new_x][new_y] == 'N' || board[new_x][new_y] == 'B' || board[new_x][new_y] == 'Q' || board[new_x][new_y] == 'K')
                                	{
						ifLegal = 0;
                                	}
                                	else
                                	{
						ifLegal = 1;
                                	}
				}
				else
				{
					ifLegal = 0;
				}
                        }
                        else
                        {
                                //printf("Illegal move! Try again\n");
                                ifLegal = 1;
                        }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
		break;
		}


		case 5:		/*White Knight*/
		{
			/*Checking for L shape move*/
			if((new_x == index_x-2 && new_y == index_y+1) || (new_x == index_x-2 && new_y == index_y-1) || (new_x == index_x-1 && new_y == index_y+2) || (new_x == index_x-1 && new_y == index_y-2) || (new_x == index_x+2 && new_y == index_y+1) || (new_x == index_x+2 && new_y == index_y-1) || (new_x == index_x+1 && new_y == index_y+2) || (new_x == index_x+1 && new_y == index_y-2))
			{
				if(board[new_x][new_y] != ' ')
				{
					/*Capturing*/
					if(board[new_x][new_y] == 'p' || board[new_x][new_y] == 'r' || board[new_x][new_y] == 'n' || board[new_x][new_y] == 'b' || board[new_x][new_y] == 'q' || board[new_x][new_y] == 'k')
					{
						ifLegal = 0;
					}
					else
					{
						ifLegal = 1;
					}
				}
				else
				{
					ifLegal = 0;
				}
			}
			/*If starting position = new position*/
                        else if(index_x == new_x && index_y == new_y)
                        {
                        	ifLegal = 2;
                        }	
			else
			{
				ifLegal = 1;
			}
		break;
		}

		case 6:		/*Black Knight*/
		{
			/*Checking for L shape move*/
                        if((new_x == index_x-2 && new_y == index_y+1) || (new_x == index_x-2 && new_y == index_y-1) || (new_x == index_x-1 && new_y == index_y+2) || (new_x == index_x-1 && new_y == index_y-2) || (new_x == index_x+2 && new_y == index_y+1) || (new_x == index_x+2 && new_y == index_y-1) || (new_x == index_x+1 && new_y == index_y+2) || (new_x == index_x+1 && new_y == index_y-2))
                        {
                                if(board[new_x][new_y] != ' ')
                                {
                                        /*Capturing*/
                                        if(board[new_x][new_y] == 'P' || board[new_x][new_y] == 'R' || board[new_x][new_y] == 'N' || board[new_x][new_y] == 'B' || board[new_x][new_y] == 'Q' || board[new_x][new_y] == 'K')
                                        {
                                                ifLegal = 0;
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                }
                                else
                                {
                                        ifLegal = 0;
                                }
                        }
                        /*If starting position = new position*/
                        else if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
                        else
                        {
                                ifLegal = 1;
                        }
		break;
		}

		case 7:		/*White Bishop*/
		{
			if(IsOccupied(board, index_x, index_y, new_x, new_y))
                        {
                                ifLegal = 1;
                        }
			else if(index_x == new_x && index_y == new_y)
                        {
                        	ifLegal = 2;
			}
			else if(abs(index_x - new_x) == abs(index_y - new_y))
			{
				if(board[new_x][new_y] != ' ')
				{
					if((board[new_x][new_y] == 'p' || board[new_x][new_y] == 'r' || board[new_x][new_y] == 'n' || board[new_x][new_y] == 'b' || board[new_x][new_y] == 'q' || board[new_x][new_y] == 'k'))
                                	{
                                        	ifLegal = 0;
                                	}
					else
					{
						ifLegal = 1;
					}
				}				
                                else
                                {
                                        ifLegal = 0;
                                }
			}
			else
			{
				ifLegal = 1;
			}
                                                                                       
		break;
		}

		case 8:		/*Black Bishop*/
		{
			if(IsOccupied(board, index_x, index_y, new_x, new_y))
                        {
                                ifLegal = 1;
                        }
			else if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
                        else if(abs(index_x - new_x) == abs(index_y - new_y))
                        {
                                if(board[new_x][new_y] != ' ')
                                {
                                        if((board[new_x][new_y] == 'P' || board[new_x][new_y] == 'R' || board[new_x][new_y] == 'N' || board[new_x][new_y] == 'B' || board[new_x][new_y] == 'Q' || board[new_x][new_y] == 'K'))
                                        {
                                                ifLegal = 0;
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                }
				else		                         		                                                                                                             
                                {
                                        ifLegal = 0;
                                }
                        }
                        else
                        {
                                ifLegal = 1;
                        }

                break;
                }

		case 9:		/*White Queen*/
		{
			if(IsOccupied(board, index_x, index_y, new_x, new_y))
                        {
                        	ifLegal = 1;
                        }
			else if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
                        else if((abs(index_x - new_x) == abs(index_y - new_y)) || (index_x == new_x && index_y != new_y) || (index_x != new_x && index_y == new_y))
                        {
                                if(board[new_x][new_y] != ' ')
                                {
                                        if((board[new_x][new_y] == 'p' || board[new_x][new_y] == 'r' || board[new_x][new_y] == 'n' || board[new_x][new_y] == 'b' || board[new_x][new_y] == 'q' || board[new_x][new_y] == 'k'))
                                        {
                                                ifLegal = 0;
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                }	                                                                                                
                                else
                                {
                                        ifLegal = 0;
                                }
                        }
                        else
                        {
                                ifLegal = 1;
                        }

                break;
                }	
			
		
		case 10:	/*Black Queen*/
		{
			if(IsOccupied(board, index_x, index_y, new_x, new_y))
                        {
                                ifLegal = 1;
                        }			                                                	
			else if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
                        else if((abs(index_x - new_x) == abs(index_y - new_y)) || (index_x == new_x && index_y != new_y) || (index_x != new_x && index_y == new_y))
                        {
                                if(board[new_x][new_y] != ' ')
                                {
                                        if((board[new_x][new_y] == 'P' || board[new_x][new_y] == 'R' || board[new_x][new_y] == 'N' || board[new_x][new_y] == 'B' || board[new_x][new_y] == 'Q' || board[new_x][new_y] == 'K'))
                                        {
                                                ifLegal = 0;
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                }                                                                                                      
                                else
                                {
                                        ifLegal = 0;
                                }
                        }
                        else
                        {
                                ifLegal = 1;
                        }
		break;	
		}

		case 11:	/*White King*/
		{
			if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
                        //castling
                        //king side
                        else if (index_y == 4 && index_x == 7 && new_y == 6 && new_x == 7)
                        {
                                //check if king and rook have been moved
                                if( WhiteKing == 0 && WRook2 == 0)
                                {
                                        //check if positions between king and rook are empty
                                        if( board[7][5] == ' ' && board[7][6] == ' ' && board[7][7] == 'R')
                                        {
                                                //check if king is in check and the two other spots are threatened
                                                if((ifCheck(board, positions, 2)) == 0 && (isThreatened(board, positions, "F1", 2)) == 0 && (isThreatened(board, positions, "G1", 2)) == 0)
                                                {
                                                        ifLegal = 0;
                                                        castlingFLAG = 1;
                                                }
                                                else
                                                {
                                                        ifLegal = 1;
                                                }
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                        
                                }
                                else
                                {
                                        ifLegal = 1;
                                }
                                
                        }
                        //queen side
                        else if (index_y == 4 && index_x == 7 && new_y == 2 && new_x == 7)
                        {
                                //check if king and rook have been moved
                                if(WhiteKing == 0 && WRook1 == 0)
                                {
                                        //check if positions between king and rook are empty
                                        if( board[7][1] == ' ' && board[7][2] == ' ' && board[7][3] == ' ' && board[7][0] == 'R')
                                        {
                                                //check if king is in check and the two other spots are threatened
                                                if((ifCheck(board, positions, 2)) == 0 && (isThreatened(board, positions, "C1", 2)) == 0 && (isThreatened(board, positions, "D1", 2)) == 0)
                                                {
                                                        ifLegal = 0;
                                                        castlingFLAG = 2;
                                                }
                                                else
                                                {
                                                        ifLegal = 1;
                                                }
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                        
                                }
                                else
                                {
                                        ifLegal = 1;
                                }
                        }
                        //end of castling
                        else if((abs(index_x - new_x) == 1 && abs(index_y - new_y) == 1) || (abs(index_x - new_x) == 1 && index_y == new_y) || (index_x == new_x && abs(index_y - new_y) == 1))
                        {
                                if(board[new_x][new_y] != ' ')
                                {
                                        if((board[new_x][new_y] == 'p' || board[new_x][new_y] == 'r' || board[new_x][new_y] == 'n' || board[new_x][new_y] == 'b' || board[new_x][new_y] == 'q' || board[new_x][new_y] == 'k'))
                                        {
                                                ifLegal = 0;
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                }
                                else
                                {
                                        ifLegal = 0;
                                }
                        }
                        else
                        {
                                ifLegal = 1;
                        }
		break;
		}

		case 12:        /*Black King*/
                {
                        if(index_x == new_x && index_y == new_y)
                        {
                                ifLegal = 2;
                        }
                        //castling
                        //king side
                        else if (index_y == 4 && index_x == 0 && new_y == 6 && new_x == 0)
                        {
                                //check if king and rook have been moved
                                if(BlackKing == 0 && BRook2 == 0)
                                {
                                        //check if positions between king and rook are empty
                                        if( board[0][5] == ' ' && board[0][6] == ' ' && board[0][7] == 'r')
                                        {
                                                //check if king is in check and the two other spots are threatened
                                                if((ifCheck(board, positions, 1)) == 0 && (isThreatened(board, positions, "F8", 1)) == 0 && (isThreatened(board, positions, "G8", 1)) == 0)
                                                {
                                                        ifLegal = 0;
                                                        castlingFLAG = 3;
                                                }
                                                else
                                                {
                                                        ifLegal = 1;
                                                }
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                        
                                }
                                else
                                {
                                        ifLegal = 1;
                                } 
                        }
                        //queen side
                        else if (index_y == 4 && index_x == 0 && new_y == 2 && new_x == 0)
                        {
                                //check if king and rook have been moved
                                if(BlackKing == 0 && BRook1 == 0)
                                {
                                        //check if positions between king and rook are empty
                                        if( board[0][1] == ' ' && board[0][2] == ' ' && board[0][3] == ' ' && board[0][0] == 'r')
                                        {
                                                //check if king is in check and the two other spots are threatened
                                                if((ifCheck(board, positions, 1)) == 0 && (isThreatened(board, positions, "C8", 1)) == 0 && (isThreatened(board, positions, "D8", 1)) == 0)
                                                {
                                                        ifLegal = 0;
                                                        castlingFLAG = 4;
                                                }
                                                else
                                                {
                                                        ifLegal = 1;
                                                }
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                        
                                }
                                else
                                {
                                        ifLegal = 1;
                                }
                        }
                        //end of castling
                        else if((abs(index_x - new_x) == 1 && abs(index_y - new_y) == 1) || (abs(index_x - new_x) == 1 && index_y == new_y) || (index_x == new_x && abs(index_y - new_y) == 1))
                        {
                                if(board[new_x][new_y] != ' ')
                                {
                                        if((board[new_x][new_y] == 'P' || board[new_x][new_y] == 'R' || board[new_x][new_y] == 'N' || board[new_x][new_y] == 'B' || board[new_x][new_y] == 'Q' || board[new_x][new_y] == 'K'))
                                        {
                                                ifLegal = 0;
                                        }
                                        else
                                        {
                                                ifLegal = 1;
                                        }
                                }
                                else
                                {
                                        ifLegal = 0;
                                }
                        }
                        else
                        {
                                ifLegal = 1;
                        }
                break;
                }

		case 13:	/*Empty Space*/
		{
			ifLegal = 2;
		}	
			
			
	}
	
	return ifLegal;

}

void PromotionBlack(char board[8][8], int x)
{
    char newPiece;
    printf("\nPawn has reached the other end!\n");
    printf("What would you like to promote your Pawn to?\n");
    board[7][x] = ' ';

    do
    {
	printf("'r' = Rook    \n");
    	printf("'n' = Knight  \n");
    	printf("'b' = Bishop  \n");
        printf("'q' = Queen   \n");

	printf("Selection: ");
        scanf("%c", &newPiece);
	getchar();
       
	if (newPiece == 'r')
                board[7][x] = 'r';
        else if (newPiece == 'n')
                board[7][x] = 'n';
        else if (newPiece == 'b')
                board[7][x] = 'b';
        else if (newPiece == 'q')
                board[7][x] = 'q';
        else
                printf("Invalid selection!\n");

	

    } while (newPiece != 'r' && newPiece != 'n' && newPiece != 'b' && newPiece != 'q');
	
}

void PromotionWhite(char board[8][8], int x)
{
    char newPiece;
    printf("\nPawn has reached the other end!\n");
    printf("What would you like to promote your Pawn to?\n");
    board[0][x] = ' ';

    do
    {
    	printf("'R' = Rook    \n");
    	printf("'N' = Knight  \n");
    	printf("'B' = Bishop  \n");
	printf("'Q' = Queen   \n\n");

        printf("Selection: ");
        scanf("%c", &newPiece);
	getchar();
	
        if (newPiece == 'R')
		board[0][x] = 'R';
	else if (newPiece == 'N')
		board[0][x] = 'N';
	else if (newPiece == 'B')
		board[0][x] = 'B';
	else if (newPiece == 'Q')
		board[0][x] = 'Q';
	else
		printf("Invalid selection!\n");
	
	
    } while (newPiece != 'R' && newPiece != 'N' && newPiece != 'B' && newPiece != 'Q');

}


int BlackEnpassant(char opponentNP[3], char opponentCP[3], char opponentPiece, char new_position[3], char current_position[3], char piece, char board[8][8], 
		   const char positions[8][8][3])
{	
	int ifLegal = 1;

	if (opponentNP[0] == opponentCP[0] && opponentCP[1] == '7' && opponentNP[1] == '5' && opponentPiece == 'p')
	{
		if (opponentNP[0] == new_position[0] && abs(new_position[0] - current_position[0]) == 1 && 
		    current_position[1] == '5' && new_position[1] == '6' && piece == 'P')
		{
			ifLegal = 0;
			
			for(int c = 0; c < 8; c++)
			{
                        	for(int d = 0; d < 8; d++)
				{
                                        if(!strcmp(opponentNP, positions[c][d]))
					{
                                        	board[c][d] = ' ';
                                        }
                                }
                       	}

		}
		else
		{
			ifLegal = 1;
		}
		
	}
	
	return ifLegal;	
}



int WhiteEnpassant(char opponentNP[3], char opponentCP[3], char opponentPiece, char new_position[3], char current_position[3], char piece, char board[8][8],
                   const char positions[8][8][3])
{
        int ifLegal = 1;

        if (opponentNP[0] == opponentCP[0] && opponentCP[1] == '2' && opponentNP[1] == '4' && opponentPiece == 'P')
        {
                if (opponentNP[0] == new_position[0] && abs(new_position[0] - current_position[0]) == 1 &&
                    current_position[1] == '4' && new_position[1] == '3' && piece == 'p')
                {
                        ifLegal = 0;

                        for(int c = 0; c < 8; c++)
                        {
                                for(int d = 0; d < 8; d++)
                                {
                                        if(!strcmp(opponentNP, positions[c][d]))
                                        {
                                                board[c][d] = ' ';
                                        }
                                }
                        }

                }
                else
                {
                        ifLegal = 1;
                }

        }

        return ifLegal;
}

