#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include "pieces.h"
#include "initialize.h"
#include "print.h"
#include "utility.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <map>
#include <tuple>
#include <chrono>
#include <tree.h>

std::tuple<
    std::map<std::string, bool>, 
    std::map<std::string, bool>, 
    std::map<std::string, std::string>, 
    std::map<std::string, std::string>
> create_chessboard_all_maps(std::vector<piece_t*>& board) {
    // Initialize the maps with default values
    std::map<std::string, bool> enemy_map;
    std::map<std::string, bool> our_map;
    std::map<std::string, std::string> enemy_map_string;
    std::map<std::string, std::string> our_map_string;

    // Fill the maps with default values for all positions (a1 to h8)
    for (int row = 1; row < 9; row++) {
        for (int column = 1; column < 9; column++) {
            std::string position = convert_chessboard_position(column, row);
            // Default value assignments
            our_map[position] = false;          // Our pieces: false by default
            enemy_map[position] = false;       // Enemy pieces: false by default
            our_map_string[position] = "none"; // Our piece type: "none" by default
            enemy_map_string[position] = "none"; // Enemy piece type: "none" by default
        }
    }

    // Now, iterate through the pieces and update the maps
    for (piece_t* piece : board) {
        std::string position = convert_chessboard_position(piece->column, piece->row);

        if (piece->my_piece) {
            // If it's our piece, update the "our" maps
            our_map[position] = true;
            our_map_string[position] = piece->type();
        } else {
            // If it's an enemy piece, update the "enemy" maps
            enemy_map[position] = true;
            enemy_map_string[position] = piece->type();
        }
    }

    // Return all maps as a tuple
    return {enemy_map, our_map, enemy_map_string, our_map_string};
}



std::tuple<
    std::map<std::string, bool>, 
    std::map<std::string, bool>
> create_chessboard_maps_bools(std::vector<piece_t*>& board) {
    // Initialize the maps
    std::map<std::string, bool> enemy_map;
    std::map<std::string, bool> our_map;

    // Iterate over all pieces in the board vector
    for (piece_t* piece : board) {
        // Convert the piece's position to chessboard notation (e.g., "a1")
        std::string position = convert_chessboard_position(piece->column, piece->row);

        if (piece->my_piece) {
            // If it's our piece, update the "our" maps
            our_map[position] = true;
        } else {
            // If it's an enemy piece, update the "enemy" maps
            enemy_map[position] = true;
        }
    }

    // Fill remaining positions on the chessboard with default values
    for (int row = 1; row < 9; row++) {
        for (int column = 1; column < 9; column++) {
            std::string position = convert_chessboard_position(column, row);

            // Set default values only if the position isn't already in the map
            if (!our_map.count(position)) {
                our_map[position] = false;
            }
            if (!enemy_map.count(position)) {
                enemy_map[position] = false;
            }
        }
    }

    // Return all maps as a tuple
    return {enemy_map, our_map};
}



std::vector<piece_t*> create_copy_board(std::vector<piece_t*>& board){//create a copy of the board and pieces, so different pointers
  std::vector<piece_t*> new_board;
  for(piece_t* piece:board){
    piece_t* new_piece=piece->create_copy();
    new_board.push_back(new_piece);
  }
  return new_board;
}

void delete_board(std::vector<piece_t*> board){
  for(piece_t* piece:board){
    delete piece;
  }
}


piece_t* find_king(std::vector<piece_t*>& board, bool my_king){//finds our teams king and returns it from board if my_king==true else it is the opposing teams king
  for(piece_t* piece:board){
    if(piece->type()=="king" && piece->my_piece==my_king){
      return piece;
    }
  }
  writeMoveToFile(greedy_move_external,file_name_external);
  error("did not find a king which is not normal");
  
}

bool our_team_check(std::vector<piece_t*>& board, bool am_white,std::map<std::string, std::string>& our_map, std::map<std::string, std::string>& enemy_map){
  piece_t* our_king=find_king(board,true);
  return check_at_pos(board,am_white,our_king->column,our_king->row,our_map,enemy_map);
}

bool enemy_team_check(std::vector<piece_t*>& board, bool am_white,std::map<std::string, std::string>& our_map, std::map<std::string, std::string>& enemy_map){
  piece_t* enemy_king=find_king(board,false);
  return check_at_pos(board,!am_white,enemy_king->column,enemy_king->row,enemy_map,our_map);
}


std::vector<std::string> all_moves_our_pieces(std::vector<piece_t*>& board,bool am_white,std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string){//returns std string of all moves our pieces can on the board, deos not take into account if we are in check and can only do moves to get out of check
  std::vector<std::string> all_moves;
  piece_t* our_king=find_king(board,true);
  for(piece_t* piece:board){//just iterate throught all our pieces and add their moves
    if(piece->my_piece){
        std::vector<std::string> moves=piece->possible_moves(board,am_white,my_map,enemy_map,my_map_string,enemy_map_string,our_king->column,our_king->row);
        all_moves.insert(all_moves.end(),moves.begin(),moves.end());


        
    }
  }
    
  return all_moves;
}

std::vector<std::string> all_moves_enemy_pieces(std::vector<piece_t*>& board,bool am_white,std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string){//returns std string of all moves our pieces can on the board, deos not take into account if we are in check and can only do moves to get out of check
  std::vector<std::string> all_moves;
  piece_t* enemy_king=find_king(board,false);
  for(piece_t* piece:board){//just iterate throught all our pieces and add their moves
    if(!piece->my_piece){
        std::vector<std::string> moves=piece->possible_moves(board,!am_white,enemy_map,my_map,enemy_map_string,my_map_string,enemy_king->column,enemy_king->row);
        all_moves.insert(all_moves.end(),moves.begin(),moves.end());


        
    }
  }
    
  return all_moves;
}

std::vector<std::string> all_our_moves_final(std::vector<piece_t*>& board,bool am_white,std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string){
  std::vector<std::string> all_moves=all_moves_our_pieces(board,am_white,our_map,enemy_map,our_map_string,enemy_map_string);
  
  if(!our_team_check(board,am_white,our_map_string,enemy_map_string)){
    return all_moves;
  }
  
  std::vector<std::string> legal_moves;
  for(std::string move:all_moves){//pour chaque move, on cree une copy du board, fait le move et regarde si dans ce board hyptothetique le roi est en echec
    std::vector<piece_t*> copy_of_board=create_copy_board(board);
    std::map<std::string,bool> our_map=create_our_map(board);
    std::map<std::string,bool> enemy_map=create_enemy_map(board);
    std::string move_rewrite=change_format(move);
    perform_move(copy_of_board,move_rewrite,our_map,enemy_map);
    std::map<std::string,bool> new_map=create_our_map(copy_of_board);
    std::map<std::string,bool> new_enemy_map=create_enemy_map(copy_of_board);
    std::map<std::string,std::string> our_map_string =create_our_map_string(copy_of_board,new_map);
    std::map<std::string,std::string> enemy_map_string =create_enemy_map_string(copy_of_board,new_enemy_map);
    if(our_team_check(copy_of_board,am_white,our_map_string,enemy_map_string)==false){//si le roi nest pas en echec dans ce nouveau board on rajout le move au move final
      legal_moves.push_back(move);
    }
    delete_board(copy_of_board);
  }
  return legal_moves;
  
}

std::vector<std::string> all_enemy_moves_final(std::vector<piece_t*>& board,bool am_white,std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string){

  std::vector<std::string> all_moves=all_moves_enemy_pieces(board,am_white,our_map,enemy_map,our_map_string,enemy_map_string);
  if(!enemy_team_check(board,am_white,our_map_string,enemy_map_string)){
    return all_moves;
  }
  std::vector<std::string> legal_moves;
  for(std::string move:all_moves){//pour chaque move, on cree une copy du board, fait le move et regarde si dans ce board hyptothetique le roi est en echec
    std::vector<piece_t*> copy_of_board=create_copy_board(board);
    std::map<std::string,bool> our_map=create_our_map(board);
    std::map<std::string,bool> enemy_map=create_enemy_map(board);
    std::string move_rewrite=change_format(move);
    perform_move(copy_of_board,move_rewrite,enemy_map,our_map);
    std::map<std::string,bool> new_map=create_our_map(copy_of_board);
    std::map<std::string,bool> new_enemy_map=create_enemy_map(copy_of_board);
    std::map<std::string,std::string> our_map_string =create_our_map_string(copy_of_board,new_map);
    std::map<std::string,std::string> enemy_map_string =create_enemy_map_string(copy_of_board,new_enemy_map);
    if(enemy_team_check(copy_of_board,am_white,our_map_string,enemy_map_string)==false){//si le roi nest pas en echec dans ce nouveau board on rajout le move au move final
      legal_moves.push_back(move);
    }
    delete_board(copy_of_board);
  }
  return legal_moves;
}