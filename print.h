#ifndef PRINT_H
#define PRINT_H

#include "pieces.h"
#include <vector>


// Function declarations
void display_board(std::vector<piece_t *> board); // changes
std::string getPenultimateLine(const std::string& filename); // get the penultimate line of the text file used for the en passant case
std::string toChessFormat(int column, int row);
#endif
