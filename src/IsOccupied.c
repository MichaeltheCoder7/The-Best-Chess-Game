#include<stdio.h>
#include<stdlib.h>
#include "IsOccupied.h"

int IsOccupied(char board[8][8], int index_x, int index_y, int new_x, int new_y)
{
	int ifLegal = 0;
	/*Horizontal Left*/
	if(index_x == new_x && index_y > new_y)
        {
        	for(int i = index_y-1; i > new_y; i--)
                {
                	if(board[index_x][i] != ' ')
                        {
                        
                                ifLegal = 1;
                        }
                }
        }
	/*Vertical Up*/
        else if(index_x < new_x && index_y == new_y)
        {
                for(int i = index_x+1; i < new_x; i++)
                {
                        if(board[i][index_y] != ' ')
                        {
                        	
                                ifLegal = 1;
                        }
                }
        }
	/*Horizontal Right*/
        else if(index_x == new_x && index_y < new_y)
        {
                for(int i = index_y+1; i < new_y; i++)
                {
                	if(board[index_x][i] != ' ')
                        {
                        	
                                ifLegal = 1;
                        }
                }
        }
	/*Vertical Down*/
        else if(index_x > new_x && index_y == new_y)
        {
                for(int i = index_x-1; i > new_x; i--)
                {
                        if(board[i][index_y] != ' ')
                        {
                                
                        	ifLegal = 1;
                        }
                }
        }
	/*Diagonal Down Left*/
	else if(index_x < new_x && index_y > new_y)
	{
		for(int i = new_x - index_x - 1; i > 0; i--)
		{
			if(board[index_x + i][index_y - i] != ' ')
			{
				ifLegal = 1;
				
			}
		}
	}
	/*Diagonal Down Right*/
        else if(index_x < new_x && index_y < new_y)
        {
                for(int i = new_x - index_x - 1; i > 0; i--)
                {
                        if(board[index_x + i][index_y + i] != ' ')
                        {
                                ifLegal = 1;
                        	
			}
                }
        }
	/*Diagonal Up Left*/
        else if(index_x > new_x && index_y > new_y)
        {
                for(int i = index_x - new_x - 1; i > 0; i--)
                {     
			if(board[index_x - i][index_y - i] != ' ')
                        {
                                ifLegal = 1;
	
                        }
                }
        }
	/*Diagonal Up Right*/
        else if(index_x > new_x && index_y < new_y)
        {
                for(int i = index_x - new_x - 1; i > 0; i--)
                {
                        if(board[index_x - i][index_y + i] != ' ')
                        {
                                ifLegal = 1;
                        	
			}
                }
        }
	

	
	return ifLegal;

}


















