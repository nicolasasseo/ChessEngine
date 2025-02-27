#include "eval.h"
//A helper file to make the eval.cpp file less messy and more readable


//Piece square table, which we will use in the val_of_board function

// Knight Piece-Square Table
int whiteknightPST[8][8] = {
    {-50, -40, -30, -30, -30, -30, -40, -50},
    {-40, -20, 0, 5, 5, 0, -20, -40},
    {-30, 5, 10, 15, 15, 10, 5, -30},
    {-30, 0, 15, 20, 20, 15, 0, -30},
    {-30, 5, 15, 20, 20, 15, 5, -30},
    {-30, 0, 10, 15, 15, 10, 0, -30},
    {-40, -20, 0, 0, 0, 0, -20, -40},
    {-50, -40, -30, -30, -30, -30, -40, -50}
};

int blackknightPST[8][8] = {
    {-50, -40, -30, -30, -30, -30, -40, -50},
    {-40, -20, 0, 0, 0, 0, -20, -40},
    {-30, 0, 10, 15, 15, 10, 0, -30},
    {-30, 5, 15, 20, 20, 15, 5, -30},
    {-30, 0, 15, 20, 20, 15, 0, -30},
    {-30, 5, 10, 15, 15, 10, 5, -30},
    {-40, -20, 0, 5, 5, 0, -20, -40},
     {-50, -40, -30, -30, -30, -30, -40, -50},
};

// Bishop Piece-Square Table
int whitebishopPST[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-10, 5, 10, 15, 15, 10, 5, -10},
    {-10, 10, 15, 20, 20, 15, 10, -10},
    {-10, 5, 10, 20, 20, 10, 5, -10},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-30, -20, -20, -20, -20, -20, -20, -30}
};

int blackbishopPST[8][8] = {
    {-30, -20, -20, -20, -20, -20, -20, -30},
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-10, 5, 10, 20, 20, 10, 5, -10},
    {-10, 10, 15, 20, 20, 15, 10, -10},
    {-10, 5, 10, 15, 15, 10, 5, -10},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-20, -10, -10, -10, -10, -10, -10, -20},
};

// Rook Piece-Square Table
int whiterookPST[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {5, 10, 10, 10, 10, 10, 10, 5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 10, 15, 15, 10, 0, -5},
    {-5, 0, 10, 15, 15, 10, 0, -5},
    {-5, 0, 5, 10, 10, 5, 0, -5},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

int blackrookPST[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {-5, 0, 5, 10, 10, 5, 0, -5},
    {-5, 0, 10, 15, 15, 10, 0, -5},
    {-5, 0, 10, 15, 15, 10, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {5, 10, 10, 10, 10, 10, 10, 5},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

// Queen Piece-Square Table
int whitequeenPST[8][8] = {
    {-20, -10, -10, -5, -5, -10, -10, -20},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-10, 5, 10, 15, 15, 10, 5, -10},
    {-5, 10, 15, 20, 20, 15, 10, -5},
    {-5, 5, 15, 20, 20, 15, 5, -5},
    {-10, 0, 10, 15, 15, 10, 0, -10},
    {-10, -5, 0, 0, 0, 0, -5, -10},
    {-20, -10, -10, -5, -5, -10, -10, -20}
};

int blackqueenPST[8][8] = {
    {-20, -10, -10, -5, -5, -10, -10, -20},
    {-10, -5, 0, 0, 0, 0, -5, -10},
    {-10, 0, 10, 15, 15, 10, 0, -10},
    {-5, 5, 15, 20, 20, 15, 5, -5},
    {-5, 5, 15, 20, 20, 15, 5, -5},
    {-10, 5, 10, 15, 15, 10, 5, -10},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-20, -10, -10, -5, -5, -10, -10, -20},
};

// King Piece-Square Table for the Opening/Middlegame
int kingPST[8][8] = {
    {-30, -40, -50, -50, -50, -50, -40, -30},
    {-40, -50, -60, -60, -60, -60, -50, -40},
    {-50, -60, -70, -80, -80, -70, -60, -50},
    {-50, -60, -80, -90, -90, -80, -60, -50},
    {-50, -60, -80, -90, -90, -80, -60, -50},
    {-50, -60, -70, -80, -80, -70, -60, -50},
    {-40, -50, -60, -60, -60, -60, -50, -40},
    {-30, -40, -50, -50, -50, -50, -40, -30}
};
// King Piece-Square Table (Endgame)
int king_endgamePST[8][8] = {
    {-50, -40, -30, -20, -20, -30, -40, -50},
    {-40, -20, 0, 10, 10, 0, -20, -40},
    {-30, 0, 10, 20, 20, 10, 0, -30},
    {-20, 10, 20, 30, 30, 20, 10, -20},
    {-20, 10, 20, 30, 30, 20, 10, -20},
    {-30, 0, 10, 20, 20, 10, 0, -30},
    {-40, -20, 0, 10, 10, 0, -20, -40},
    {-50, -40, -30, -20, -20, -30, -40, -50}
};

// Piece-square table for pawns (white pawns)
int whitePawnPST[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {5, 5, 10, 25, 25, 10, 5, 5},
    {0, 0, 0, 20, 20, 0, 0, 0},
    {5, -5, -10, 0, 0, -10, -5, 5},
    {5, 10, 10, -20, -20, 10, 10, 5},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

// Piece-square table for pawns (black pawns, mirrored)
int blackPawnPST[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {5, 10, 10, -20, -20, 10, 10, 5},
    {5, -5, -10, 0, 0, -10, -5, 5},
    {0, 0, 0, 20, 20, 0, 0, 0},
    {5, 5, 10, 25, 25, 10, 5, 5},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {0, 0, 0, 0, 0, 0, 0, 0}
};