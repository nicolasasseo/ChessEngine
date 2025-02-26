#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include "pieces.h"
#include "initialize.h"
#include "print.h"
#include "utility.h"
#include <map>
#include <chrono>

std::string convert_chessboard_position(int column, int row) {
    // Check if column and row are valid
    

    char col_letter;
    switch (column) {
        case 1: col_letter = 'a'; break;
        case 2: col_letter = 'b'; break;
        case 3: col_letter = 'c'; break;
        case 4: col_letter = 'd'; break;
        case 5: col_letter = 'e'; break;
        case 6: col_letter = 'f'; break;
        case 7: col_letter = 'g'; break;
        case 8: col_letter = 'h'; break;
        default: break;
            //throw std::out_of_range("Invalid column"); // This shouldn't happen due to the earlier check
    }

    // Combine column letter and row number into the chessboard position
    return std::string(1, col_letter) + std::to_string(row);
}



bool in_board_my_pos(std::vector<piece_t *>& board, int column, int row)
{ // if their is a piece at this the place columnns and row in board that is ours returns true, if not returns false
  for (piece_t *value : board)
  {
    if (column == value->column && row == value->row and value->my_piece==true)
    {
      return true;
    }
  }
  return false; 
}

bool in_board_enemy_pos(std::vector<piece_t *>& board, int column, int row)
{ // if their is a piece at this place that is enemy returns true, if not returns false
  for (piece_t *value : board)
  {
    if (column == value->column && row == value->row and value->my_piece==false)
    {
      return true;
    }
  }
  return false; 
}


std::map<std::string,bool> create_enemy_map(std::vector<piece_t*>& board){ //function that creates a map with keys all position in board in string mode so like a1 or b2 and the value is true if their is one of our pieces and false otherwirse
  std::map<std::string,bool> my_pos;
  for(auto piece: board){
    if(!piece->my_piece){
      std::string place = convert_chessboard_position(piece->column,piece->row);
      my_pos[place] = true;
    }
  }
  return my_pos;
}

std::map<std::string,bool> create_our_map(std::vector<piece_t*>& board){ //exact same map as just above but true if enemy and false otherwise
  std::map<std::string,bool> our_map;
  for (int row = 1; row < 9; row++) {
        for (int column = 1; column < 9; column++) {
            std::string position = convert_chessboard_position(column, row);
            // Default value assignments
            our_map[position] = false;          // Our pieces: false by default
        }
    }

    // Now, iterate through the pieces and update the maps
    for (piece_t* piece : board) {
        std::string position = convert_chessboard_position(piece->column, piece->row);

        if (piece->my_piece) {
            // If it's our piece, update the "our" maps
            our_map[position] = true;
            
        }
    }

  return our_map;
}

std::map<std::string,std::string> create_enemy_map_string(std::vector<piece_t*>& board,std::map<std::string,bool>& enemy_map){ //function that creates a map with keys all position in board in string mode so like a1 or b2 and the value is true if their is one of our pieces and false otherwirse
  std::map<std::string,std::string> my_pos;
  for(int row=1; row<9;row++){
    for(int column=1; column<9;column++){
      std::string pos=convert_chessboard_position(column,row);
      if(enemy_map[pos]){
        my_pos[pos]=return_piece(board,column,row)->type();
      }
      else{
        std::string none="none";
        my_pos[pos]= none;
      }
    }
  }
  return my_pos;
}

std::map<std::string,std::string> create_our_map_string(std::vector<piece_t*>& board,std::map<std::string,bool>& our_map){ //exact same map as just above but true if enemy and false otherwise
  std::map<std::string,std::string> my_pos;
  for(int row=1; row<9;row++){
    for(int column=1; column<9;column++){
      std::string pos=convert_chessboard_position(column,row);
      if(our_map[pos]){
        my_pos[pos]=return_piece(board,column,row)->type();
      }
      else{
        std::string none="none";
        my_pos[pos]= none;
      }
    }
  }
  return my_pos;
}



std::vector<std::string> pawn_t::possible_moves(std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king){
  if(pined_in_direct(board,am_white,"row",column_king,row_king,my_map_string,enemy_map_string)){
    return std::vector<std::string>();
  }
  // possible moves for the pawn
  std::vector<std::string> moves_pawn;
  std::vector<std::string> promotion_options = {"q", "k"};
  std::string move1 = "";
  std::string init_pos = convert_chessboard_position(column, row);
  //we have to account for the colour of the pawn, as it can only move up/down the board if it is resp. white/black
  if(am_white){
    std::string move1 = convert_chessboard_position(column, row+1);
    std::string eat_left = convert_chessboard_position(column-1, row+1);
    std::string eat_right = convert_chessboard_position(column+1, row+1);
    std::string double_move = convert_chessboard_position(column, row+2);
    
    if(row==7){
      //we have to account for promotion here as the pawn will land on the last line of the chess board

      //checking if there is an enemy where the white pawn can eat it
      if(pined_in_direct(board,am_white,"diag1",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"column",column_king,row_king,my_map_string,enemy_map_string)==false&&column-1>0&&row+1<9){
        if(enemy_map[eat_left]){
          for(auto promotion: promotion_options){
            moves_pawn.push_back(init_pos+eat_left+promotion);}
          }
      } 
      if(pined_in_direct(board,am_white,"diag2",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"column",column_king,row_king,my_map_string,enemy_map_string)==false&&column+1<9&&row+1<9){
        if(enemy_map[eat_right]){
          for(auto promotion: promotion_options){
            moves_pawn.push_back(init_pos+eat_right+promotion);}}
      }
      if(pined_in_direct(board,am_white,"diag1",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"diag2",column_king,row_king,my_map_string,enemy_map_string)==false){
        if(!my_map[move1] && !enemy_map[move1]){//normal pawn move
          //checking if there is no enemy or friendly piece in front/back of the white/black pawn
          for(auto promotion: promotion_options){  
            moves_pawn.push_back(init_pos+move1+promotion);}}
      }
      //dont even consider the double move

    }else{
      //regular white pawn move without promotion

      //checking if there is an enemy where the white pawn can eat it
      if(pined_in_direct(board,am_white,"diag1",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"column",column_king,row_king,my_map_string,enemy_map_string)==false&&column-1>0&&row+1<9){
        if(enemy_map[eat_left]){
          moves_pawn.push_back(init_pos+eat_left);}
      }
      if(pined_in_direct(board,am_white,"diag2",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"column",column_king,row_king,my_map_string,enemy_map_string)==false&&column+1<9&&row+1<9){
        if(enemy_map[eat_right]){
          moves_pawn.push_back(init_pos+eat_right);}
      }

      //normal pawn move
      if(pined_in_direct(board,am_white,"diag2",column_king,row_king,my_map_string,enemy_map_string)==false && pined_in_direct(board,am_white,"diag1",column_king,row_king,my_map_string,enemy_map_string)==false){
        if(!my_map[move1] && !enemy_map[move1]){
          //checking if there is no enemy or friendly piece in front/back of the white/black pawn
          moves_pawn.push_back(init_pos+move1);}

        if( row==2 && !my_map[double_move] && !enemy_map[double_move] && !my_map[move1] && !enemy_map[move1]){
          //white pawn is at row 2 and has not moved and there is nothing in the way of it moving two blocks forward
          moves_pawn.push_back(init_pos+double_move);}
      }
    }
    if(row == 5){
      /*
      //have to check for the possibility of en passant from the white pawn
      std::string penultimate_line = getPenultimateLine(g_argv[1]);
      if(penultimate_line[1] == '7' && penultimate_line[3] == '5'){

        //en passant to the left
        if(penultimate_line[0]==penultimate_line[2] && penultimate_line[0]-'0'==column-1){
          // we have a piece doing exactly the movement we need it to be doing
          for(int i=0; i<board.size(); i++){
            if(board[i]->column == column-1 && board[i]->row == row && dynamic_cast<pawn_t*>(board[i])){
              //we have checked that it is a pawn that has just moved next to our pawn
              pawn_moves.push_back(init_pos+convert_chessboard_position(column-1, row+1));}}

        //en passant to the right
        }else if(penultimate_line[0]==penultimate_line[2] && penultimate_line[0]-'0'==column+1){
          // we have a piece doing exactly the movement we need it to be doing
          for(int i=0; i<board.size(); i++){
            if(board[i]->column == column+1 && board[i]->row == row && dynamic_cast<pawn_t*>(board[i])){
              //we have checked that it is a pawn that has just moved next to our pawn
              pawn_moves.push_back(init_pos+convert_chessboard_position(column+1, row+1));}}
        
        }
      }
      */
      }
  
  }else{
    //a black pawn is moving
    std::string move1 = convert_chessboard_position(column, row-1);
    std::string eat_left = convert_chessboard_position(column-1, row-1);
    std::string eat_right = convert_chessboard_position(column+1, row-1);
    std::string double_move = convert_chessboard_position(column, row-2);
    if(row==2){
      //we have to account for promotion as the black pawn will move on the last row of the chessboard
      if(pined_in_direct(board,am_white,"diag2",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"column",column_king,row_king,my_map_string,enemy_map_string)==false&&column-1>0&&row-1>0){
        if(enemy_map[eat_left]){
          for(auto promotion: promotion_options){
            moves_pawn.push_back(init_pos+eat_left+promotion);}}
      }

      if(pined_in_direct(board,am_white,"diag1",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"column",column_king,row_king,my_map_string,enemy_map_string)==false&&column+1<9&&row-1>0){
        if(enemy_map[eat_right]){
          for(auto promotion: promotion_options){
            moves_pawn.push_back(init_pos+eat_right+promotion);}}
      }
      //normal pawn move
      if(pined_in_direct(board,am_white,"diag1",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"diag2",column_king,row_king,my_map_string,enemy_map_string)==false){
        if(!my_map[move1] && !enemy_map[move1]){
          //checking if there is no enemy or friendly piece in front/back of the white/black pawn
          for(auto promotion: promotion_options){
            moves_pawn.push_back(init_pos+move1+promotion);}}
      }

      //dont even consider the double move
    
    }else{
      //a normal black pawn move without promotion

      //checking if there is an enemy where the black pawn can eat it
      if(pined_in_direct(board,am_white,"diag2",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"column",column_king,row_king,my_map_string,enemy_map_string)==false&&column-1>0&&row-1>0){
        if(enemy_map[eat_left]){
          moves_pawn.push_back(init_pos+eat_left);}
      }

      if(pined_in_direct(board,am_white,"diag1",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"column",column_king,row_king,my_map_string,enemy_map_string)==false&&column+1<9&&row-1>0){
        if(enemy_map[eat_right]){
          moves_pawn.push_back(init_pos+eat_right);}
      }
      
      //normal pawn move
      if(pined_in_direct(board,am_white,"diag1",column_king,row_king,my_map_string,enemy_map_string)==false&&pined_in_direct(board,am_white,"diag2",column_king,row_king,my_map_string,enemy_map_string)==false){
        if(!my_map[move1] && !enemy_map[move1]){
          //checking if there is no enemy or friendly piece in front/back of the white/black pawn
          moves_pawn.push_back(init_pos+move1);}

        if(row==7 && !my_map[double_move] && !enemy_map[double_move] && !my_map[move1] && !enemy_map[move1]){
          //the black pawn is at row 7 and has not moved and there is nothing in the way of it moving two blocks forward
          moves_pawn.push_back(init_pos+double_move);}
      }  
    }
    if(row == 4){
      /*
      //have to check for the possibility of en passant from the black pawn
      std::string penultimate_line = getPenultimateLine(g_argv[1]);
      if(penultimate_line[2] == '7' && penultimate_line[3] == '4'){

        //en passant to the left
        if(penultimate_line[0]==penultimate_line[2] && penultimate_line[0]-'0'==column-1){
          // we have a piece doing exactly the movement we need it to be doing
          for(int i=0; i<board.size(); i++){
            if(board[i]->column == column-1 && board[i]->row == row && dynamic_cast<pawn_t*>(board[i])){
              //we have checked that it is a pawn that has just moved next to our pawn
              pawn_moves.push_back(init_pos+convert_chessboard_position(column-1, row-1));}}

        //en passant to the right
        }else if(penultimate_line[0]==penultimate_line[2] && penultimate_line[0]-'0'==column+1){
          // we have a piece doing exactly the movement we need it to be doing
          for(int i=0; i<board.size(); i++){
            if(board[i]->column == column+1 && board[i]->row == row && dynamic_cast<pawn_t*>(board[i])){
              //we have checked that it is a pawn that has just moved next to our pawn
              pawn_moves.push_back(init_pos+convert_chessboard_position(column+1, row-1));}}
        
        }
      }
      */}
  }
    

  return moves_pawn;
}
  


// horace not finished
std::vector<std::string> king_t::possible_moves(std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king)
{
    // This function does not check certain conditions especially check
    std::string init_pos = convert_chessboard_position(column, row); // Converts position to string like "a1"
    std::vector<std::string> moves_king;
    
    // Horizontal and vertical moves
    if (column - 1 >= 1) {
      if(!check_at_pos(board,am_white,column-1,row,our_map_string,enemy_map_string)){
        std::string move = convert_chessboard_position(column - 1, row);
        std::string move_tot = init_pos + move;
        if (our_map[move] == false) { // Check not one of our pieces at this spot
            moves_king.push_back(move_tot);
        }
      }
    }
    if (column + 1 <= 8) {
      if(!check_at_pos(board,am_white,column+1,row,our_map_string,enemy_map_string)){
        std::string move = convert_chessboard_position(column + 1, row);
        std::string move_tot = init_pos + move;
        if (our_map[move] == false) {
            moves_king.push_back(move_tot);
        }
      }
    }
    if (row - 1 >= 1) {
      if(!check_at_pos(board,am_white,column,row-1,our_map_string,enemy_map_string)){
        std::string move = convert_chessboard_position(column, row - 1);
        std::string move_tot = init_pos + move;
        if (our_map[move] == false) {
            moves_king.push_back(move_tot);
        }
      }
    }
    if (row + 1 <= 8) {
      if(!check_at_pos(board,am_white,column,row+1,our_map_string,enemy_map_string)){
        std::string move = convert_chessboard_position(column, row + 1);
        std::string move_tot = init_pos + move;
        if (our_map[move] == false) {
            moves_king.push_back(move_tot);
        }
      }
    }


    // Diagonal moves
    
    if (column - 1 >= 1 && row - 1 >= 1) { // Top-left diagonal
      if(!check_at_pos(board,am_white,column-1,row-1,our_map_string,enemy_map_string)){
        std::string move = convert_chessboard_position(column - 1, row - 1);
        std::string move_tot = init_pos + move;
        if (our_map[move] == false) {
            moves_king.push_back(move_tot);
        }
      }
    }
    if (column + 1 <= 8 && row - 1 >= 1) { // Top-right diagonal
      if(!check_at_pos(board,am_white,column+1,row-1,our_map_string,enemy_map_string)){
        std::string move = convert_chessboard_position(column + 1, row - 1);
        std::string move_tot = init_pos + move;
        if (our_map[move] == false) {
            moves_king.push_back(move_tot);
        }
      }
    }
    if (column - 1 >= 1 && row + 1 <= 8) { // Bottom-left diagonal
      if(!check_at_pos(board,am_white,column-1,row+1,our_map_string,enemy_map_string)){
        std::string move = convert_chessboard_position(column - 1, row + 1);
        std::string move_tot = init_pos + move;
        if (our_map[move] == false) {
            moves_king.push_back(move_tot);
        }
      }
    }
    if (column + 1 <= 8 && row + 1 <= 8) { // Bottom-right diagonal
      if(!check_at_pos(board,am_white,column+1,row+1,our_map_string,enemy_map_string)){
        std::string move = convert_chessboard_position(column + 1, row + 1);
        std::string move_tot = init_pos + move;
        if (our_map[move] == false) {
            moves_king.push_back(move_tot);
        }
      }
    }

    /*
    //castling
    /*
    if(has_moved==false){
      bool long_castling_possible = true;
      bool short_castling_possible = true;
      for(int i=1; i<9; i++){
        if(i!=1 && i!= 5 && i!= 8){
          if(our_map[convert_chessboard_position(i, row)] || check_at_pos(board, am_white, i, row)){
            if(i<5){
              long_castling_possible = false;
            }else{
              short_castling_possible = false;}
          }}
      }
    
      //we have checked if there are friendly pieces and if the oppenent's pieces can visit the blocks in the way of either the long or short castling
      if(long_castling_possible || short_castling_possible){
        for(int i=0; i<board.size(); i++){
          if(dynamic_cast<rook_t*>(board[i]) && board[i]->has_moved==false && board[i]->row==this->row && board[i]->column == 1 && long_castling_possible){
            // we have checked that the rook has not moved, that it is in the correct position and that long castling is possible
            moves_king.push_back(init_pos+convert_chessboard_position(1, row));}

          if(dynamic_cast<rook_t*>(board[i]) && board[i]->has_moved==false && board[i]->row==this->row && board[i]->column == 8 && short_castling_possible){
            // we have checked that the rook has not moved, that it is in the correct position and that short castling is possible
            moves_king.push_back(init_pos+convert_chessboard_position(8, row));}
        }
      }
    
    }
    */
    return moves_king;
}



// horace
std::vector<std::string> queen_t::possible_moves(std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king)
{
  // since queen can just do the same moves as a bishop and rook i just created intermediate pieces of rook and bishop at this stop and add the two vector of moves
  bishop_t* intermediate_bishop= new bishop_t(column,row,true);
  std::vector<std::string> moves_queen = intermediate_bishop->possible_moves(board, am_white,our_map,enemy_map,our_map_string,enemy_map_string, column_king,row_king);// get moves of bishop if it was at stop of queen
  rook_t* intermediate_rook= new rook_t(column,row,true);
  std::vector<std::string> moves_rook=intermediate_rook->possible_moves(board, am_white,our_map,enemy_map,our_map_string,enemy_map_string,column_king,row_king);// get moves of rook if it was at stop of queen
  moves_queen.insert(moves_queen.end(),moves_rook.begin(),moves_rook.end()); //add moves rook to moves queen
  delete intermediate_bishop;//delete intermediate pieces created
  delete intermediate_rook;
  
  return moves_queen;
}



// horace
std::vector<std::string> bishop_t::possible_moves(std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king)
{ 
  int i;
  int j;
  if(pined_in_direct(board,am_white,"column",column_king,row_king,our_map_string,enemy_map_string) || pined_in_direct(board,am_white,"row",column_king,row_king,our_map_string,enemy_map_string)){
    return std::vector<std::string>();
  }
  std::string init_pos=convert_chessboard_position(column,row);//converts position to string position like a1
  std::vector<std::string> moves_bishop;
  if(pined_in_direct(board,am_white,"diag2",column_king,row_king,our_map_string,enemy_map_string)==false){
    i=row+1;
    j=column+1;
    while(i<9&&j<9){//same as for rook we advance diagonally going up until meet piece or out of chessboard
      std::string move = convert_chessboard_position(j,i);
      std::string move_tot=init_pos+move;
      if(enemy_map[move]==true){// if enemy position at this position adds this move and then stops advancing diagonally in this direction
        moves_bishop.push_back(move_tot);
        break;
        }
      else if(our_map[move]==true){//if our piece at this spot than stop advancing diagonnaly in this direction
        break;
      }
      else{//if no piece just continue as normal
        moves_bishop.push_back(move_tot);
      }
      i++;
      j++;
    }
    i=row-1;
    j=column-1;
    while(i>0&&j>0){//moving diagonally down
      std::string move = convert_chessboard_position(j,i);
      std::string move_tot=init_pos+move;
      if(enemy_map[move]==true){// if enemy position at this position adds this move and then stops advancing diagonally in this direction
        moves_bishop.push_back(move_tot);
        break;
        }
      else if(our_map[move]==true){//if our piece at this spot than stop advaning diagonally in this direction
        break;
      }
      else{//if no piece just continue as normal
        moves_bishop.push_back(move_tot);
      }
      i--;
      j--;
    }

  }
  if(pined_in_direct(board,am_white,"diag1",column_king,row_king,our_map_string,enemy_map_string)==false){
    i=row+1;
    j=column-1;
    while(i<9&&j>0){//diagonally up on rows down on columns
      std::string move = convert_chessboard_position(j,i);
      std::string move_tot=init_pos+move;
      if(enemy_map[move]==true){//if enemy position at this position adds this move and then stops advancing diagonally in this direction
        moves_bishop.push_back(move_tot);
        break;
        }
      else if(our_map[move]==true){//if our piece at this spot than stop advaning diganoally in this direction
        break;
      }
      else{//if no piece just continue as normal
        moves_bishop.push_back(move_tot);
      }
      i++;
      j--;
      
    }
    i=row-1;
    j=column+1;
    while(i>0&&j<9){// same as before diaggonally up on columns down on rows
      std::string move = convert_chessboard_position(j,i);
      std::string move_tot=init_pos+move;
      if(enemy_map[move]==true){// if enemy position at this position adds this move and then stops advancing diagonally in this direction
        moves_bishop.push_back(move_tot);
        break;
        }
      else if(our_map[move]==true){//if our piece at this spot than stop advaning diagonally in this direction
        break;
      }
      else{//if no piece just continue as normal
        moves_bishop.push_back(move_tot);
      }
      i--;
      j++;
    }
  }
  

  return moves_bishop;
}

// horace
std::vector<std::string> rook_t::possible_moves(std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king)//returns legal moves in good format
{
  int i;
  int j;
  if(pined_in_direct(board,am_white,"diag1",column_king,row_king,our_map_string,enemy_map_string)||pined_in_direct(board,am_white,"diag2",column_king,row_king,our_map_string,enemy_map_string)){
    return std::vector<std::string>();
  }
  std::string init_pos=convert_chessboard_position(column,row);
  std::vector<std::string> moves_rook;
  if(!pined_in_direct(board,am_white,"row",column_king,row_king,our_map_string,enemy_map_string)){
    for (int i = row+1; i < 9; i++)//start from row under and continue our way down until hit our piece or enemy piece or get out of bounds
    {
      std::string move = convert_chessboard_position(column,i);//create what we will add in our moves so like a1b2 so A1 to B2
      std::string move_tot=init_pos+move;
      if(enemy_map[move]==true){// if enemy position adds this move and then stops advancing lower in rows
        moves_rook.push_back(move_tot);
        break;
        }
      else if(our_map[move]==true){//if our piece at this spot than stop advaning down in rows
        break;
      }
      else{//if no piece just continue as normal
        moves_rook.push_back(move_tot);
      }
      
    }
    for (int i = row-1; i > 0; i--)//start from row above and continue our way up until hit our piece our enemy piece
    {
      std::string move = convert_chessboard_position(column,i);
      std::string move_tot=init_pos+move;
      if(enemy_map[move]==true){// if enemy position adds this move and then stops advancing up in rows
        moves_rook.push_back(move_tot);
        break;
        }
      else if(our_map[move]==true){//if our piece at this spot than stop advaning up in rows
        break;
      }
      else{//if no piece just continue as normal
        moves_rook.push_back(move_tot);
      }
      
    }
  }
  if(!pined_in_direct(board,am_white,"column",column_king,row_king,our_map_string,enemy_map_string)){
    for (int j = column+1; j < 9; j++)//start from column to righ and continue our way right until hit our piece our enemy piece
    {
      std::string move = convert_chessboard_position(j,row);
      std::string move_tot=init_pos+move;
      if(enemy_map[move]==true){// if enemy position adds this move and then stops advancing right in columns
        moves_rook.push_back(move_tot);
        break;
        }
      else if(our_map[move]==true){//if our piece at this spot than stop advaning right in columns
        break;
      }
      else{//if no piece just continue as normal
        moves_rook.push_back(move_tot);
      }
      
    }
    for (int j = column-1; j > 0; j--)//start from column to left  and continue our way left until hit our piece our enemy piece
    {
      std::string move = convert_chessboard_position(j,row);
      std::string move_tot=init_pos+move;
      if(enemy_map[move]==true){// if enemy position adds this move and then stops advancing left in columns
        moves_rook.push_back(move_tot);
        break;
        }
      else if(our_map[move]==true){//if our piece at this spot than stop advaning left in columns
        break;
      }
      else{//if no piece just continue as normal
        moves_rook.push_back(move_tot);
      }
      
    }
  }
  return moves_rook;
}

std::vector<std::string> knight_t::possible_moves(std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king)
{
  if(pined_in_direct(board,am_white,"diag1",column_king,row_king,our_map_string,enemy_map_string)||pined_in_direct(board,am_white,"diag2",column_king,row_king,our_map_string,enemy_map_string)||pined_in_direct(board,am_white,"column",column_king,row_king,our_map_string,enemy_map_string)||pined_in_direct(board,am_white,"row",column_king,row_king,our_map_string,enemy_map_string)){
    return std::vector<std::string>();
  }
  std::string init_pos=convert_chessboard_position(column,row);
  std::vector<std::string> moves_knight;

  // All valid knight moves (8 possibilities)n)
  std::vector<std::pair<int, int>> offsets = {
      {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

  // Add valid moves within bounds and only if there is not team piece at stop
  for (auto offset : offsets)
  {
    
    int new_column = column + offset.first;
    int new_row = row + offset.second;
    if (0<new_column && new_column<9&&0<new_row && new_row<9)
    {
      std::string move = convert_chessboard_position(new_column,new_row);
      std::string move_tot=init_pos+move;//create what we will add in our moves so like a1b2 so A1 to B2
      if(our_map[move]==false){// only add move if their is not one of our pieces at this spot
        moves_knight.push_back(move_tot);
      }
      
      
    }
  }
  return moves_knight;
}
