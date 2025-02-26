#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "pieces.h"
#include "initialize.h"
#include "print.h"
void print_map(const char *prefix, const std::map<std::string, bool> enemy_positions)
{
  std::cout << std::string(prefix) + "Positions: " << std::endl;
  for (const auto &pair : enemy_positions)
  {
    std::cout << "Key: ";
    std::cout << pair.first; // Print the vector<int> key
    std::cout << " -> Value: " << (pair.second ? "true" : "false") << std::endl;
  }
  std::cout << "-----------------------" << std::endl;
}
std::string change_format(std::string line)
{
  // thus function receives a std::string of format (g1h3) and returns the corresponding move in the format we use
  
  for (int i = 0; i < 3; i += 2)
  {
    if (line[i] == 'a')
    {
      line[i] = '1';
    }
    else if (line[i] == 'b')
    {
      line[i] = '2';
    }
    else if (line[i] == 'c')
    {
      line[i] = '3';
    }
    else if (line[i] == 'd')
    {
      line[i] = '4';
    }
    else if (line[i] == 'e')
    {
      line[i] = '5';
    }
    else if (line[i] == 'f')
    {
      line[i] = '6';
    }
    else if (line[i] == 'g')
    {
      line[i] = '7';
    }
    else
    {
      line[i] = '8';
    }
  }
  return line;
}

// function that creates base chess board
std::tuple<std::vector<piece_t *>, std::map<std::string, bool>, std::map<std::string, bool>> initialise_chessboard(bool am_white)
{

  std::vector<piece_t *> board;
  std::map<std::string, bool> enemy_positions;
  std::map<std::string, bool> my_positions;
  // add pawns to board

  auto add_piece = [&](piece_t *piece)
  {
    std::string move = toChessFormat(piece->column, piece->row);
    board.push_back(piece); // Add the pointer to the board
    if (!piece->my_piece){
      
      enemy_positions[move] = true; // Use the piece's coordinates
    }
    else
    {
      my_positions[move] = true;
    }
  };

  for (int i = 1; i < 9; i++)
  {
    add_piece(new pawn_t(i, 2, am_white));  // white's pawns
    add_piece(new pawn_t(i, 7, !am_white)); // blacks' pawns
  }

  // add rooks
  add_piece(new rook_t(1, 1, am_white));
  add_piece(new rook_t(8, 1, am_white));
  add_piece(new rook_t(1, 8, !am_white));
  add_piece(new rook_t(8, 8, !am_white));

  // add knights
  add_piece(new knight_t(2, 1, am_white));
  add_piece(new knight_t(7, 1, am_white));
  add_piece(new knight_t(2, 8, !am_white));
  add_piece(new knight_t(7, 8, !am_white));

  // add bishops
  add_piece(new bishop_t(3, 1, am_white));
  add_piece(new bishop_t(6, 1, am_white));
  add_piece(new bishop_t(3, 8, !am_white));
  add_piece(new bishop_t(6, 8, !am_white));

  // add queens
  add_piece(new queen_t(4, 1, am_white));
  add_piece(new queen_t(4, 8, !am_white));

  // Add kings
  add_piece(new king_t(5, 1, am_white));
  add_piece(new king_t(5, 8, !am_white));

  return std::make_tuple(board, enemy_positions, my_positions);
  // j'ai fait ca pour initialiser le board au debut de la partie, jsp si ca marche
  //  il faudrai avoir un display board function pour tester comment ca marche
  
}



void perform_move(std::vector<piece_t *> &board, std::string move, std::map<std::string ,bool> &my_positions, std::map<std::string, bool> &enemy_positions)
{
  std::string from_move_str = move.substr(0, 2);
  std::string to_move_str = move.substr(2, 2);

  auto add_piece = [&](piece_t *piece){
    board.push_back(piece); // Add the pointer to the board
    if (!piece->my_piece)
    {
      enemy_positions[toChessFormat(piece->column, piece->row)] = true;
    }else{
      my_positions[toChessFormat(piece->column, piece->row)] = true;
    }
  };

  // I have to perform the move, iterating over every piece in the board
  int from_column = move[0] - '0'; // converting char type to int type
  int from_row = move[1] - '0';
  int to_column = move[2] - '0';
  int to_row = move[3] - '0';

  std::string to_move_chess_format = toChessFormat(to_column, to_row);
  std::string from_move_chess_format = toChessFormat(from_column, from_row);


  // remove any captured piece
  for (size_t j = 0; j < board.size(); j++){

    piece_t* piece = board[j]; //for clarity of the code
    if (piece->type() == "pawn" && piece->column == from_column && piece->row == from_row){
      //we have found a pawn that is moving

      //en passant
      if(my_positions[to_move_chess_format]==false && enemy_positions[to_move_chess_format]==false && abs(to_row-from_row)==1 && abs(to_column-from_column)==1){
        //we have a pawn moving in a diagonal without there being any piece where he is moving to and since only legal moves are given to us as inputs, this will be sufficient
        for(int i=0; i<board.size(); i++){
          piece_t* en_passant_piece = board[i];
          int en_passant_row = from_row;
          // determine by seeing if the pawn moves forward or backwards whether it is white or black and where the en passant row is
          // the en passant_row is the row where the pawn that is being eaten by en passant is situated at
          if(en_passant_piece->column == to_column && en_passant_piece->row == en_passant_row){
            //to determine if the pawn was black or white I simply tested if to_row was greater than from_row
            
            std::string move = toChessFormat(to_column, en_passant_row);
            if(en_passant_piece->my_piece){my_positions.erase(move);}
            else{enemy_positions.erase(move);}
            //we remove the piece that was eliminated from our maps

            delete en_passant_piece;
            board.erase(board.begin() + i); //remove the chess piece from the board
            break;
          }
        }
      }
    }
  }

  for(size_t j = 0; j < board.size(); j++){
    piece_t* piece = board[j]; //clarity
    if (piece->column == to_column && piece->row == to_row){

        if (!piece->my_piece)
        {
          enemy_positions.erase(to_move_chess_format);
        }
        else
        {
          my_positions.erase(to_move_chess_format); // TODO ADD MY_POS in here
        }
        delete board[j];
        board.erase(board.begin() + j);
        break;
    } // exit the loop
  }
  

  for (size_t i = 0; i < board.size(); i++){

    piece_t *piece = board[i]; // having this intermediate step so that the program is more readable
    // find piece to move
    if (piece->column == from_column && piece->row == from_row){
      //we have found the piece that is moving

      //handled the case when it is a king or a rook so that we can handle when we can castle
      if(dynamic_cast<king_t*>(piece) ||  dynamic_cast<rook_t*>(piece)){
        piece->has_moved = true;
      }


      //handle the case where the player is castling (note that only the king's line is encoded in the text file)
      if(dynamic_cast<king_t*>(piece) && abs(to_column-from_column) > 1){
        //as we will only be receiving legal moves for the king to be caslting he needs to be moving by at least one column 
        //the king is castling and we move it first
        piece->column = to_column;
        piece->row = to_row;


        for(int j=0; j < board.size(); j++){
          if(board[j]->row == to_row && dynamic_cast<rook_t*>(board[j])){
              if(board[j]->column == 8){
                //short castling as here the piece pointer points to the king piece
                  board[j]->column = 6;
              }else if(board[j]->column == 1){
                //long castling
                board[j]->column = 4;}
          }
        }
      }


      // handle promotion case
      if (move.size() == 5)
      {
        char promotion_type = move[4];
        std::cout << promotion_type;
        piece_t *new_piece = nullptr; // dont know what swap is occuring
        if (promotion_type == 'q')
        {
          new_piece = new queen_t(to_column, to_row, piece->my_piece);
        }
        else if (promotion_type == 'r')
        {
          new_piece = new rook_t(to_column, to_row, piece->my_piece);
        }
        else if (promotion_type == 'k')
        {
          new_piece = new knight_t(to_column, to_row, piece->my_piece);
        }
        else if (promotion_type == 'b')
        {
          new_piece = new bishop_t(to_column, to_row, piece->my_piece);
        }

        delete piece; // free memory of old piece
        board.erase(board.begin()+i); //delete the piece from the board
        add_piece(new_piece);

        if (new_piece)
        {
          if (!new_piece->my_piece)
          {
            enemy_positions[to_move_chess_format] = true;
            enemy_positions[from_move_chess_format] = false;
          }
          else
          {
            my_positions[to_move_chess_format] = true; // TODO ADD MYPOS
            my_positions[from_move_chess_format] = false;
          }
        }
      }
      else
      {
        // normal move
        piece->column = to_column;
        piece->row = to_row;

        if (!piece->my_piece)
        {
          enemy_positions[to_move_chess_format] = true;
          enemy_positions[from_move_chess_format] = false;
        }
        else
        {
          my_positions[to_move_chess_format] = true; // TODO ADD MYPOS
          my_positions[from_move_chess_format] = false;
        }
      }
    }
  }
  return;
};

bool isWhite(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) { 
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false; // Defaulting to black in case of an error
    }

    int turnCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) { // Count non-empty lines as moves
            ++turnCount;
        }
    }

    file.close();

    // If the number of turns is even, it's white's turn; otherwise, black's
    return (turnCount % 2 == 0);
}

std::tuple<std::vector<piece_t*>, std::map<std::string, bool>, std::map<std::string, bool>, bool, int> update_chessboard(const std::string &filePath){
  
  int game_phase = 0;
  int total_moves = 0;
  bool am_white = isWhite(filePath);
  

  auto [board, enemy_positions, my_positions] = initialise_chessboard(am_white);
  std::ifstream file(filePath);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not open file " << filePath << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(file, line))
  {
    total_moves ++;
    std::string move = change_format(line);                   // changing each line in the text file in a line a2 goes to 12
    perform_move(board, move, my_positions, enemy_positions); // performing each move in the text file
    //display_board(board);
  }

  if (total_moves > 10){
    game_phase = 1;
  }
    
  file.close(); // have to close the text file at the end
  return std::make_tuple(board, my_positions, enemy_positions, am_white, game_phase);
};
