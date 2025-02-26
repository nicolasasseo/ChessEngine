#include "pieces.h"
#include "utility.h"
#include <iostream>
#include <vector>
#include "print.h"
// file just important for display functions
// all functions under just to print each piece




void piece_t::print_piece() // if piece_t this means that it is not a piece on our board so just print a space
{
  std::cout << "  ";
}

void king_t::print_piece(){
  if (my_piece){
    std::cout << "mK";
  }
  else{
    std::cout << "oK";
  }
}

void queen_t::print_piece(){
  if (my_piece){
    std::cout << "mQ";
  }
  else{
    std::cout << "oQ";
  }
}

void bishop_t::print_piece(){
  if (my_piece){
    std::cout << "mB";
  }
  else{
    std::cout << "oB";
  }
}

void pawn_t::print_piece(){
  if (my_piece){
    std::cout << "mP";
  }
  else{
    std::cout << "oP";
  }
}

void knight_t::print_piece(){
  if (my_piece){
    std::cout << "mN";
  }
  else{
    std::cout << "oN";
  }
}

void rook_t::print_piece(){
  if (my_piece){
    std::cout << "mR";
  }
  else{
    std::cout << "oR";
  }
}

piece_t* in_board(std::vector<piece_t *> board, int column, int row)
{ // if their is a piece at this place returns the piece, if not returns a piece_t so that print function prints space
  for (piece_t *value : board)
  {
    if (column == value->column && row == value->row)
    {
      return value;
    }
  }
  return new empty_t(); // empty square
}

void display_board(std::vector<piece_t *> board)
{
  std::cout << "    a    b    c    d    e    f    g    h\n"; // File labels
  std::cout << "  +----+----+----+----+----+----+----+----+\n";

  for (int row = 8; row >= 1; row--)
  {                           // Rank labels
    std::cout << row << " |"; // Start the row with the rank label

    for (int col = 1; col <= 8; col++)
    {
      piece_t *piece = in_board(board, col, row);

      // Print the piece in the square
      std::cout << " ";
      piece->print_piece(); // Calls the piece's print function
      // std::cout << col << row;
      std::cout << " |"; // End of the square
    }

    std::cout << " " << row << "\n";                              // End the row with the rank label
    std::cout << "  +----+----+----+----+----+----+----+----+\n"; // Horizontal line between rows
  }

  std::cout << "    a    b    c    d    e    f    g   h\n\n"; // File labels again at the bottom
}

std::string getPenultimateLine(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    std::string line;
    std::string previousLine;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            previousLine = line;
        }
    }

    file.close();
    return previousLine;
}
std::string toChessFormat(int column, int row) {
    if (column < 1 || column > 8 || row < 1 || row > 8) {
        return "Invalid"; // Handle out-of-bounds input
    }
    char colChar = 'a' + (column - 1); // Convert column to a letter ('a' to 'h')
    return std::string(1, colChar) + std::to_string(row); // Combine column and row
}
