#include <stdio.h>

#include "Help.h"

void help(void)
{
    printf("\n------------------------------------------------Chess Rules------------------------------------------------------\n");
    printf("Chess Pieces Movement\n\n");
    printf("\tPawn: For each pawn, the first move may either be 1-2 steps forward. After the first usage\n");
    printf("\tof that Pawn, they may only move 1 space forward at a time. The Pawn may also move diagonally\n");
    printf("\tforward 1 step to capture the opponent’s piece.\n\n");
    printf("\tRook: May move vertically or horizontally as many squares in the horizontal or vertical direction\n");
    printf("\tas long as the pathway is not blocked.\n\n");
    printf("\tKnight: May only move in the shape of a ‘L’. For example, 2 steps forward and 1 step right.\n\n");
    printf("\tBishop: May only move diagonally as many squares as long as the pathway is not blocked.\n\n");
    printf("\tQueen: Can move 1 step in the surrounding squares. Can also move vertically, horizontally, and\n");
    printf("\tdiagonally as many spaces as long as the pathway is not blocked.\n\n");
    printf("\tKing: Can only move 1 step in the surrounding squares.\n\n");
    printf("Gameplay\n\n");
    printf("\tThe White pieces always make the initial move.\n\n");
    printf("\tIf the Pawn makes it across the board, the Pawn must be promoted to either: a Queen, Rook, Knight,\n");
    printf("\tor Bishop.\n\n");
    printf("\tIf the player hasn’t moved their King and Rook, and there is no pieces between them, they may move\n");
    printf("\ttheir King 2 steps towards the Rook and place the Rook on the other side of the King to protect it,\n");
    printf("\tknown as “Castling”. Castling may also be on the queen side. For the king, Castling can’t be used\n");
    printf("\tto avoid being checked, passing through a check, or getting into check. To Castle in this game, move\n");
    printf("\tthe king two squares to the left or the right.\n\n");
    printf("\tIf a pawn moves two squares instead of one from its starting position in order to avoid capture,\n");
    printf("\tthe opponent can capture the just moved pawn as it “passes” through the first square which is also\n");
    printf("\tknown as “En Passant”. The opponent must do the en passant capture on their next turn or they may\n");
    printf("\tnot do so. The result of the en passant capture is the same as if the pawn only moved one square\n");
    printf("\tinstead of two.\n\n");
    printf("\tWhen the King is at risk of being captured, it is “checked”.\n\n");
    printf("\tIf the player is unable to make any moves to get out of being “checked” then the game is “checkmate”\n");
    printf("\twith that player losing the match.\n\n"); 
    printf("\tIf the play remains unchecked and has no legal moves, the match will result in a draw.\n");
    printf("--------------------------------------------------------------------------------------------------------------------\n\n");
}
