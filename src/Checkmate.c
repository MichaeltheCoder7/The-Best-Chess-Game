#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Checkmate.h"
#include "CheckMove.h"
#include "Board.h"


//check if check 
//return 1 if yes
//return 0 if no
//color = 1: check if black king is in check
//color = 2: check if white king is in check
int ifCheck(char board[8][8], const char positions[8][8][3], int color)
{
    char king_position [3];
    int king_x, king_y;
    char opponent[16][3];

    
    if(color == 1)
    {
        for(int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                if(board[x][y] == 'k')
                {
                    king_x = x;
                    king_y = y;
                }

            }
        }
        strcpy(king_position, positions[king_x][king_y]);//get black king's position
    }
    else if(color == 2)
    {
        for(int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                if(board[x][y] == 'K')
                {
                    king_x = x;
                    king_y = y;
                }

            }
        }
        strcpy(king_position, positions[king_x][king_y]);//get white king's position
    }
    
    if(color == 1)
    {
        opponent_positions(board, positions, opponent, color);
        for(int x = 0; x < 16; x++)
        {
            if(strcmp(opponent[x], "") != 0)//make sure it is not empty
            {
                if(CheckPiece(positions, board, opponent[x], king_position) == 0)
                {
                    return 1;
                }
            }
        }
    }
    else if(color == 2)
    {
        opponent_positions(board, positions, opponent, color);
        for(int x = 0; x < 16; x++)
        {
            if(strcmp(opponent[x], "") != 0)
            {
                if(CheckPiece(positions, board, opponent[x], king_position) == 0)
                {
                    return 1;
                }
            }
        }
    }
    //check if it is legal for the current move to capture the king
    
    return 0;

}

//save one side's all positions in an array of string
//need caller to declare: char opponent[16][3] array first and pass it in
//the array has a fixed size
//color = 1: white pieces
//color = 2: black pieces
void opponent_positions(char board[8][8], const char positions[8][8][3], char opponent[16][3], int color)
{
    //Clear opponent array
    for(int x = 0; x < 16; x++)
    {
        strcpy(opponent[x], "");
    }
    int temp = 0;
    //get all white pieces locations
    if(color == 1)
    {
        for(int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                if(isupper(board[x][y]))
                {
                    strcpy(opponent[temp], positions[x][y]);
                    temp++;
                }

            }
        }
    }
    //get all black pieces locations
    else if(color == 2)
    {
        for(int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                if(islower(board[x][y]))
                {
                    strcpy(opponent[temp], positions[x][y]);
                    temp++;
                }

            }
        }
    }

}

//get a position string and translate it to a piece character on the board
char position_to_piece(char board[8][8], const char positions[8][8][3], char current_position[3])
{
    int index_x, index_y;
	char piece;
    
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
    piece = board[index_x][index_y];
    return piece;
}

//check if a piece in a given position is threatened
//return 1 if yes
//return 0 if no
//error if empty
//color = 1: check if a position is threatened by white pieces
//color = 2: check if a position is threatened by black pieces

int isThreatened(char board[8][8], const char positions[8][8][3], char location[3], int color)
{

    char opponent[16][3];
    int index_x, index_y;
    int new_x = 0;
	int new_y = 0;
      
    if(color == 1)
    {
        opponent_positions(board, positions, opponent, color);
        for(int i = 0; i < 16; i++)
        {
            if(strcmp(opponent[i], "") != 0)//make sure it is not empty
            {

                    //in case it is pawn
                    if(position_to_piece(board, positions, opponent[i]) == 'P')
                    {

                        for(int x = 0; x < 8; x++)
                        {
                            for(int y = 0; y < 8; y++)
                            {
                                if(strcmp(opponent[i], positions[x][y]) == 0)
                                {
                                    index_x = x;
                                    index_y = y;
                                }
                            }
                        }
                        for(int x = 0; x < 8; x++)
                        {
                            for(int y = 0; y < 8; y++)
                            {
                                if(strcmp(location, positions[x][y]) == 0)
                                {
                                    new_x = x;
                                    new_y = y;
                                }
                            }
                        }
                        if((new_x == index_x - 1 && new_y == index_y + 1) || (new_x == index_x - 1 && new_y == index_y - 1))
                        {
                            return 1;
                        }
                    }
                    else if(CheckPiece(positions, board, opponent[i], location) == 0)
                    {
                    return 1;
                    }
            }
        }
    }
    
    else if(color == 2)
    {
        opponent_positions(board, positions, opponent, color);
        for(int i = 0; i < 16; i++)
        {
            if(strcmp(opponent[i], "") != 0)//make sure it is not empty
            {
                    if(position_to_piece(board, positions, opponent[i]) == 'p')
                    {

                        for(int x = 0; x < 8; x++)
                        {
                            for(int y = 0; y < 8; y++)
                            {
                                if(strcmp(opponent[i], positions[x][y]) == 0)
                                {
                                    index_x = x;
                                    index_y = y;
                                }
                            }
                        }
                        for(int x = 0; x < 8; x++)
                        {
                            for(int y = 0; y < 8; y++)
                            {
                                if(strcmp(location, positions[x][y]) == 0)
                                {
                                    new_x = x;
                                    new_y = y;
                                }
                            }
                        }
                        if((new_x == index_x + 1 && new_y == index_y + 1) || (new_x == index_x + 1 && new_y == index_y - 1)) 
                        {
                            return 1;
                        }
                    }
                    else if(CheckPiece(positions, board, opponent[i], location) == 0)
                    {
                        return 1;
                    }
                
            }
        }
    }
   
    
    return 0;

}



//given a position of a piece and save all possible legal moves of that piece in a string array
//need caller call getLength and pass the length into the function
//return the array
//the array is dynamic so need to use void free_all_moves(char** all_moves, int length) when done
char** get_all_moves(char board[8][8], const char positions[8][8][3], char initial_position[3], int length)
{
    //list all the positions on the board
    char coordinates[8][8][3] = 	{
								{"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"},
								{"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7"},
								{"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6"},
								{"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5"},
								{"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4"},
								{"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3"},
								{"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2"},
								{"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"},
						   		};
    
    int index = 0;
    //make an dynamic string array to store the positions of all possible moves
    char** all_moves = malloc(length * sizeof(char*));
    if (!all_moves)
    {
        perror("Out of memory! Aborting...");
        exit(0);
    }
    for(int i = 0; i < length; i++)
    {
        all_moves[i] = malloc(sizeof(char) * 3);
        if (!all_moves[i])
        {
            perror("Out of memory! Aborting...");
            exit(0);
        }
    }
    

    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if(CheckPiece(positions, board, initial_position, coordinates[x][y]) == 0)//or 3 
            {
                //save all ilegal positions a piece can move to into the all_moves array
                strcpy(all_moves[index], positions[x][y]);
                index++;
            }
        }
    }

    return all_moves;
}

//free the dynamic array
void free_all_moves(char** all_moves, int length)
{
    if(length > 0)
    {
        for(int i = 0; i < length; i++)
        {
            free(all_moves[i]);
        }
        free(all_moves);
    }
}

//made for AI
int getLength2(char board[8][8], const char positions[8][8][3], char initial_position[3])
{   
    //if AI failed to choose a position, it returns 0
    if(strcmp(initial_position, "") == 0)
    {
        return 0;
    }
    
    char piece = position_to_piece(board, positions, initial_position);
    int checkFLAG_black = ifCheck(board, positions, 1);
    int checkFLAG_white = ifCheck(board, positions, 2);
    //list all the positions on the board
    char coordinates[8][8][3] = 	{
								{"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"},
								{"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7"},
								{"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6"},
								{"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5"},
								{"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4"},
								{"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3"},
								{"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2"},
								{"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"},
						   		};
    int length = 0;
    //get the length of the array
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if(CheckPiece(positions, board, initial_position, coordinates[x][y]) == 0)//or 3 
            {   
                
                if(checkFLAG_white) //make sure AI does not ignore check
                {
                    if(ignore_check(board, positions, initial_position, coordinates[x][y], 1) == 0)
                    {
                        length++;
                    }
                }
                else if(checkFLAG_black)
                {
                    if(ignore_check(board, positions, initial_position, coordinates[x][y], 2) == 0)
                    {
                        length++;
                    }
                }    
                else if(piece == 'K') //make sure king does not move to attacked spots
                {
                    if(isThreatened(board, positions, coordinates[x][y], 2) == 0)
                    {
                        length++;
                    }
                }
                else if(piece == 'k')
                {
                    if(isThreatened(board, positions, coordinates[x][y], 1) == 0)
                    {
                        length++;
                    }
                }
                else
                {
                    length++;
                }
            }
        }
    }
    return length;
}

//made for AI
char** get_all_legal_moves(char board[8][8], const char positions[8][8][3], char initial_position[3], int length)
{
    char piece = position_to_piece(board, positions, initial_position);
    int checkFLAG_black = ifCheck(board, positions, 1);
    int checkFLAG_white = ifCheck(board, positions, 2);
    //list all the positions on the board
    char coordinates[8][8][3] = 	{
								{"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"},
								{"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7"},
								{"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6"},
								{"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5"},
								{"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4"},
								{"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3"},
								{"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2"},
								{"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"},
						   		};
    
    int index = 0;
    //make an dynamic string array to store the positions of all possible moves
    char** all_moves = malloc(length * sizeof(char*));
    if (!all_moves)
    {
        perror("Out of memory! Aborting...");
        exit(0);
    }
    for(int i = 0; i < length; i++)
    {
        all_moves[i] = malloc(sizeof(char) * 3);
        if (!all_moves[i])
        {
            perror("Out of memory! Aborting...");
            exit(0);
        }
    }
    

    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if(CheckPiece(positions, board, initial_position, coordinates[x][y]) == 0)//or 3 
            {

                if(checkFLAG_white) //make sure AI does not ignore check
                {
                    if(ignore_check(board, positions, initial_position, coordinates[x][y], 1) == 0)
                    {
                        strcpy(all_moves[index], positions[x][y]);
                        index++;
                    }
                }
                else if(checkFLAG_black)
                {
                    if(ignore_check(board, positions, initial_position, coordinates[x][y], 2) == 0)
                    {
                        strcpy(all_moves[index], positions[x][y]);
                        index++;
                    }
                }    
                else if(piece == 'K') //make sure king does not move to attacked spots
                {
                    if(isThreatened(board, positions, coordinates[x][y], 2) == 0)
                    {
                        strcpy(all_moves[index], positions[x][y]);
                        index++;
                    }
                }
                else if(piece == 'k')
                {
                    if(isThreatened(board, positions, coordinates[x][y], 1) == 0)
                    {
                        strcpy(all_moves[index], positions[x][y]);
                        index++;
                    }
                }
                else
                {
                    strcpy(all_moves[index], positions[x][y]);
                    index++;
                }
                //save all ilegal positions a piece can move to into the all_moves array

            }
        }
    }

    return all_moves;
}


//check if checkmate
//return 1 if yes
//return 0 if no
//color = 1: check black king
//color = 2: check white king 
int isCheckmate(char board[8][8], const char positions[8][8][3], int color)
{
    char piece_positions[16][3];
    char board_copy[8][8];
    int length = 0;
    int checkmate_flag = 1;

    //check checkmate condition for black king
    if(color == 1)
    {
        //check if the black king is in check
        //return 0 if no
        if(ifCheck(board, positions, 1) == 0)
        {
            return 0;
        }
        //get all black pieces' positions
        opponent_positions(board, positions, piece_positions, 2);
        for(int x = 0; x < 16; x++)
        {
            if(strcmp(piece_positions[x], "") != 0)//make sure it is not empty
            {
                //get all legal moves of a black piece
                length = getLength(board, positions, piece_positions[x]);
                char** all_moves = get_all_moves(board, positions, piece_positions[x], length);
                for(int i = 0; i < length; i++)
                {
                    //make a copy of the board
                    for(int a = 0; a < 8; a++)
                    {
                        for(int b = 0; b < 8; b++)
                        {
                            board_copy[a][b] = board[a][b];
                        }
                    }
                    //test that move and see if king is still in check
                    updateboard(piece_positions[x], all_moves[i], board_copy, positions);
                    if(ifCheck(board_copy, positions, 1) == 0)
                    {
                        //if it's not in check in any case, set the flag to 0
                        checkmate_flag = 0;
                    }
                }
                free_all_moves(all_moves, length);

                
            }
        }

    }
    
    //check checkmate condition for white king
    if(color == 2)
    {
        //check if the white king is in check
        //return 0 if no
        if(ifCheck(board, positions, 2) == 0)
        {
            return 0;
        }
        //get all white pieces' positions
        opponent_positions(board, positions, piece_positions, 1);
        for(int x = 0; x < 16; x++)
        {
            if(strcmp(piece_positions[x], "") != 0)//make sure it is not empty
            {
                //get all legal moves of a white piece
                length = getLength(board, positions, piece_positions[x]);
                char** all_moves = get_all_moves(board, positions, piece_positions[x], length);
                for(int i = 0; i < length; i++)
                {
                    //make a copy of the board
                    for(int a = 0; a < 8; a++)
                    {
                        for(int b = 0; b < 8; b++)
                        {
                            board_copy[a][b] = board[a][b];
                        }
                    }
                    //test that move and see if king is still in check
                    updateboard(piece_positions[x], all_moves[i], board_copy, positions);
                    if(ifCheck(board_copy, positions, 2) == 0)
                    {
                        //if it's not in check in any case, set the flag to 0
                        checkmate_flag = 0;
                    }
                }
                free_all_moves(all_moves, length);
                
            }
        }

    }
    
    return checkmate_flag;
}

//get the length of the all_moves string array
//length is also the number of legal moves for a piece at the initial_position
int getLength(char board[8][8], const char positions[8][8][3], char initial_position[3])
{
    //list all the positions on the board
    char coordinates[8][8][3] = 	{
								{"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"},
								{"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7"},
								{"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6"},
								{"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5"},
								{"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4"},
								{"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3"},
								{"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2"},
								{"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"},
						   		};
    int length = 0;
    //get the length of the array
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if(CheckPiece(positions, board, initial_position, coordinates[x][y]) == 0)//or 3 
            {
                length++;
            }
        }
    }
    return length;
}

//see if check condition is ignored
//return 1 if yes(illegal)
//return 0 if no(legal)
//color: 1 for white
//2 for black
int ignore_check(char board[8][8], const char positions[8][8][3], char old_position[3], char new_position[3], int color)
{
    char board_copy[8][8];
    //make a copy of the board
    for(int a = 0; a < 8; a++)
    {
        for(int b = 0; b < 8; b++)
        {
            board_copy[a][b] = board[a][b];
        }
    }
    if(color == 1)
    {
        //test that move and see if king is still in check
        updateboard(old_position, new_position, board_copy, positions);
        if(ifCheck(board_copy, positions, 2) == 1)
        {

            return 1;
        }
    }
    else if(color == 2)
    {
        //test that move and see if king is still in check
        updateboard(old_position, new_position, board_copy, positions);
        if(ifCheck(board_copy, positions, 1) == 1)
        {
            return 1;
        }
    }

    return 0;
}

//check if checkmate for draw
//return 1 if yes
//return 0 if no
//color = 1: check black king
//color = 2: check white king 
int draw_Checkmate(char board[8][8], const char positions[8][8][3], int color)
{
    char piece_positions[16][3];
    char board_copy[8][8];
    int length = 0;
    int checkmate_flag = 1;

    //check checkmate condition for black king
    if(color == 1)
    {

        //get all black pieces' positions
        opponent_positions(board, positions, piece_positions, 2);
        for(int x = 0; x < 16; x++)
        {
            if(strcmp(piece_positions[x], "") != 0)//make sure it is not empty
            {
                //get all legal moves of a black piece
                length = getLength(board, positions, piece_positions[x]);
                char** all_moves = get_all_moves(board, positions, piece_positions[x], length);
                for(int i = 0; i < length; i++)
                {
                    //make a copy of the board
                    for(int a = 0; a < 8; a++)
                    {
                        for(int b = 0; b < 8; b++)
                        {
                            board_copy[a][b] = board[a][b];
                        }
                    }
                    //test that move and see if king is still in check
                    updateboard(piece_positions[x], all_moves[i], board_copy, positions);
                    if(ifCheck(board_copy, positions, 1) == 0)
                    {
                        //if it's not in check in any case, set the flag to 0
                        checkmate_flag = 0;
                    }
                }
                free_all_moves(all_moves, length);

                
            }
        }

    }
  	//check checkmate condition for white king
    if(color == 2)
    {
        
		//get all white pieces' positions
        opponent_positions(board, positions, piece_positions, 1);
        for(int x = 0; x < 16; x++)
        {
            if(strcmp(piece_positions[x], "") != 0)//make sure it is not empty
            {
                //get all legal moves of a white piece
                length = getLength(board, positions, piece_positions[x]);
                char** all_moves = get_all_moves(board, positions, piece_positions[x], length);
                for(int i = 0; i < length; i++)
                {
                    //make a copy of the board
                    for(int a = 0; a < 8; a++)
                    {
                        for(int b = 0; b < 8; b++)
                        {
                            board_copy[a][b] = board[a][b];
                        }
                    }
                    //test that move and see if king is still in check
                    updateboard(piece_positions[x], all_moves[i], board_copy, positions);
                    if(ifCheck(board_copy, positions, 2) == 0)
                    {
                        //if it's not in check in any case, set the flag to 0
                        checkmate_flag = 0;
                    }
                }
                free_all_moves(all_moves, length);
                
            }
        }

    }
    
    return checkmate_flag;
}

int restrict_king(char board[8][8], const char positions[8][8][3], int color, char current_position[3], char location[3]){
	char king;
	int is_it_king = 0;
	int threat = 0;
	int suicide_king = 0;
	int threat_color = 0;

	if(color == 1){
		threat_color = 2;
	}
	else if(color == 2){
		threat_color = 1;
	}

	//position to piece will take the current position and change it to a character
	king = position_to_piece(board, positions, current_position);

	//take that character and check if it is a king
	//if it is a king and it is not threatened then
	if((king == 'k') || (king == 'K')){
		is_it_king = 1;
	}

	//threat = 1 means yes threatened
	//threat = 0 means no threat
	threat = isThreatened(board, positions, location, threat_color);

	if((threat == 1) && (is_it_king == 1)){
		suicide_king = 1;
	}

 	//if its 1 then it is an invalid move since king cannot kill itself
	return suicide_king;

}

int check_draw(char board[8][8], const char positions [8][8][3], int color){
	int check = 0;
	int check_mate = 0;
	char all_positions[16][3];
	int num_legal_moves = 0;
	int drawFlag = 0;
    int check_color = 0;
	int check_kingx = 0;
	int check_kingy = 0;
	int one_piece = 0;

    if(color == 1) //white player
    {
        check_color = 2;
    }
    else if(color == 2) //black player
    {
        check_color = 1;
    }
    
	//see if player is in check
	//if check = 0 then draw
	check = ifCheck(board, positions, check_color);
	
	//see if there are any legal moves
	//if num_legal_moves is 0 then draw
	opponent_positions(board, positions, all_positions, color);
	for(int x = 0; x < 8; x++){
		for(int y = 0; y < 8; y++){
			if(strcmp(positions[x][y], all_positions[0]) == 0){ //only want the first position becasue this is case where only king is left
				check_kingx = x;
				check_kingy = y;
			}
		}
	}
	//check if there is only one piece on the board
	one_piece = strcmp(all_positions[1], "");

	//if there is only one piece left on the board and it is a king
	if((one_piece == 0) && ((board[check_kingx][check_kingy] == 'K') || (board[check_kingx][check_kingy] == 'k'))){
		check_mate = draw_Checkmate(board, positions, check_color); //seeing if moving my own piece puts me in checkmate
	}

	//go through all pieces  on the board and check if there are any legal moves
	for(int i = 0; i < 16; i++){
		if(strcmp(all_positions[i], "") != 0){//make sure it is not empty
			if(getLength(board, positions, all_positions[i]) != 0){
				num_legal_moves++;
			}
		}
	}
	
	//if all of the legal moves cause a checkmate then there are no legal moves
	if(check_mate == 1){
		num_legal_moves = 0;
	}	

	if((num_legal_moves == 0) && (check ==  0)){
		drawFlag = 1; //if draw flag is 1 then it is a draw
	}
	
	return drawFlag;
}
