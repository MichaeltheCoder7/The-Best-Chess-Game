#include <stdio.h>

#include "Menu.h"

void Challenger(void)
{
    printf("********** Mode ***********\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs CPU\n");
    printf("Please choose a number: ");
}

void Color(void)
{
    printf("********** Color **********\n");
    printf("1. White Piece\n");
    printf("2. Black Piece\n");
    printf("Please choose a number: ");
}

void PrintMenu(void)
{
    printf("************ Menu ************\n");
    printf("1. Make Movement\n");
    printf("2. Help\n");
    printf("3. Exit Game\n");
    printf("Please choose a number: ");
}

void definitions(void)
{
    printf("************************\n");
    printf("* Upper case for WHITE *\n");
    printf("* Lower case for BLACK *\n");
    printf("* 'R' or 'r' = Rook    *\n");
    printf("* 'N' or 'n' = Knight  *\n");
    printf("* 'B' or 'b' = Bishop  *\n");
    printf("* 'Q' or 'q' = Queen   *\n");
    printf("* 'K' or 'k' = King    *\n");
    printf("* 'P' or 'p' = Pawn    *\n");
    printf("************************\n");
}

