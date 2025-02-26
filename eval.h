#ifndef EVAL_H
#define EVAL_H

#include <vector>
#include "initialize.h"
#include "print.h"
#include "utility.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <iostream>
#include <algorithm>
#include <map>
#include <tuple>
#include "pieces.h"

/*
For each of the following parameters in the evaluation vector they will represent our score minus the enemy score for this given position
Ex: material_valuation = our_material_valuation_score - enemy_material_evaluation_score
*/

//this is not the predefined list, we can add more if necessary, have not determined the weights we will give to each of the paramaters
struct eval_vec{
    int game_phase; //the weights of each parameter may change whether we are in the opening, the middlegame of the endgame
    int material_valuation; //the material value of the board, taking into consideration the piece square tables for each piece
    int king_safety; //seeing how safe each king is, the safer a king, the higher the value of this parameter
    int pawn_structure; //evaluating pawn islands; isolated pawns; doubled pawns; passed pawns; backward pawns; pawn majority etc;
    int mobility; //seeing how much the pieces can move in the position

    eval_vec(): game_phase(0), material_valuation(0), king_safety(0), pawn_structure(0), mobility(0){} //an empty constructor for when 
};

// Main evaluation function for the chess engine. Combines various positional and strategic factors: (material value, mobility, king safety, center control) to compute an overall evaluation score in centipawns.
int eval_position( std::vector<piece_t*>& board, bool am_white, int game_phase,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string);

// Evaluates the material value of the board for the current player. Incorporates Piece-Square Tables (PST) to adjust scores based on positional factors (e.g., center control, endgame king positioning).
int val_of_board( std::vector<piece_t*>& board, bool am_white, int game_phase);

// Calculates mobility scores for all pieces on the board. Rewards pieces for the number of legal moves they can make, with different weights for different piece types (e.g., knights, bishops, queens).
int calculate_mob( std::vector<piece_t*>& board, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map);

// Identifies the "king zone," a group of squares around the king, including nearby adjacent squares and forward-facing squares, which are critical for king safety evaluation.
std::vector<std::string> find_king_zone( std::vector<piece_t*>& board, bool my_king, bool am_white);

// Counts the number of enemy attacks targeting the king's zone and assigns weights to those attacks based on the attacking piece type (e.g., queen, rook, bishop).
int get_attack_counter( std::vector<piece_t*>& board, bool my_eval, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string, std::vector<std::string> king_zone);

// Calculates the "king shield" score based on the presence of friendly pawns around the king, particularly in forward and adjacent positions. Assigns a positive score for such pawns.
int calculate_king_shield( std::vector<piece_t*>& board, bool my_eval, bool am_white);

std::string convert_chessboard_position(int column, int row);


// Calculates the mobility of friendly pieces within the king's zone, rewarding pieces that can defend or influence this critical area. Helps assess the player's ability to protect the king.
int get_mob_near_king( std::vector<piece_t*>& board, bool my_eval, bool am_white,std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string, std::vector<std::string> king_zone);

// Evaluates the overall safety of the king by combining factors such as the king shield score, enemy attacks on the king's zone, and friendly mobility near the king.
int king_safety( std::vector<piece_t*>& board, bool my_eval, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map,  std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string); // returns an int showing how safe the king is

//declarations of functions used in evaluation the pawn structure
int pawn_structure( std::vector<piece_t*>& board, bool my_eval, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map);
bool is_isolated( std::vector<piece_t*>& board, bool my_eval,  piece_t* pawn);
bool is_doubled( std::vector<piece_t*>& board, bool my_eval,  piece_t* pawn);
bool is_passed( std::vector<piece_t*>& board, bool my_eval,  piece_t* pawn, bool am_white);
bool is_backwards( std::vector<piece_t*>& board, bool my_eval, piece_t* pawn, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map);
int evaluate_pawn_islands( std::vector<piece_t*>& board, bool my_eval, bool am_white);

// Center control: check if we or the enmy have potential moves they can make in the center of the board (important area)
int eval_center_control( std::vector<piece_t*>& board, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map);

// Space: loop over all pieces, all of their possible moves and calculate how valuable they are depending on if they can control enemy territory, central positions or regular positions
int eval_space(std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string);
std::vector<std::pair<std::string, int>> mini_eval(std::vector<piece_t*>& board, bool am_white, int game_phase, std::map<std::string, bool>& our_map, std::map<std::string, bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string, bool our_turn);


//declaration of all the Piece tables for the pieces, there definition is in eval_helper.cpp
extern int whiteknightPST[8][8];
extern int blackknightPST[8][8];

extern int whitebishopPST[8][8];
extern int blackbishopPST[8][8];

extern int whiterookPST[8][8];
extern int blackrookPST[8][8];

extern int whitequeenPST[8][8];
extern int blackqueenPST[8][8];

extern int kingPST[8][8];
extern int king_endgamePST[8][8];

extern int blackPawnPST[8][8];
extern int whitePawnPST[8][8];




#endif