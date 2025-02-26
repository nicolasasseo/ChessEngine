#include "eval.h"

// Main evaluation function for the chess engine. Combines various positional and strategic factors: (material value, mobility, king safety, center control) to compute an overall evaluation score in centipawns.
int eval_position( std::vector<piece_t*>& board, bool am_white, int game_phase,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string){

  bool my_eval = true;
  int eval = 0;
  int val_b = val_of_board(board, am_white, game_phase);
 

  eval += val_b;

  

  int my_king_safety_score = king_safety(board, my_eval, am_white, our_map, enemy_map, our_map_string, enemy_map_string);
  
  int enemy_king_safety_score = king_safety(board, !my_eval, am_white, our_map, enemy_map, our_map_string, enemy_map_string);
 
  int my_pawn_structure_score = pawn_structure(board, my_eval, am_white, our_map, enemy_map);
 

  int enemy_pawn_structure_score = pawn_structure(board, !my_eval, !am_white, our_map, enemy_map);
  

  eval += my_pawn_structure_score;
  eval -= enemy_pawn_structure_score; 

  int space_score = eval_space(board, am_white, our_map, enemy_map, our_map_string, enemy_map_string);
 
  eval += space_score;

  return eval;
}

// Evaluates the material value of the board for the current player. Incorporates Piece-Square Tables (PST) to adjust scores based on positional factors (e.g., center control, endgame king positioning).
int val_of_board( std::vector<piece_t*>& board, bool am_white, int game_phase){
  int tot = 0;
  int param = 1;
  for (piece_t* piece : board){
    if (piece->my_piece){param = 1;}
    else{param = -1;};

    if(piece->type() == "king"){
      if(game_phase<2){
        //we are in the opening or in the middle game
        tot += (param)*kingPST[8-piece->row][piece->column-1];
      }else{
        //we are in the endgame
        tot += (param)*king_endgamePST[8-piece->row][piece->column-1];}
    }

    else if(piece->type() == "queen"){
      if((am_white && piece->my_piece) || (!am_white && !piece->my_piece)){
          tot += param*(whitequeenPST[8-piece->row][piece->column-1]+piece->get_points());
      }else{
          tot += param*(blackqueenPST[8-piece->row][piece->column-1]+piece->get_points());
        }
    }

    else if(piece->type() == "rook"){
      if((am_white && piece->my_piece) || (!am_white && !piece->my_piece)){
          tot += param*(whiterookPST[8-piece->row][piece->column-1]+piece->get_points());
      }else{
          tot += param*(blackrookPST[8-piece->row][piece->column-1]+piece->get_points());}
    }

    else if(piece->type() == "bishop"){
      if((am_white && piece->my_piece) || (!am_white && !piece->my_piece)){
          tot += param*(whitebishopPST[8-piece->row][piece->column-1]+piece->get_points());
      }else{
          tot += param*(blackbishopPST[8-piece->row][piece->column-1]+piece->get_points());}
    }

    else if(piece->type() == "knight"){
      if((am_white && piece->my_piece) || (!am_white && !piece->my_piece)){
          tot += param*(whiteknightPST[8-piece->row][piece->column-1]+piece->get_points());
      }else{
          tot += param*(blackknightPST[8-piece->row][piece->column-1]+piece->get_points());}
    }

    else{
      //the piece is a pawn
      if((am_white && piece->my_piece) || (!am_white && !piece->my_piece)){
        tot += param*(whitePawnPST[8-piece->row][piece->column-1] + piece->get_points());
      }else{
        tot += param*(blackPawnPST[8-piece->row][piece->column-1] + piece->get_points());}

      
    }
  }
  return tot;
}

// Calculates mobility scores for all pieces on the board. Rewards pieces for the number of legal moves they can make, with different weights for different piece types (e.g., knights, bishops, queens).
// int calculate_mob( std::vector<piece_t*>& board, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map){
//   int score = 0;
//   int param = 1;
//   for(piece_t* piece : board){
//     std::vector<std::string> possible_moves;
//     if(piece->my_piece){
//       param = 1;
//       possible_moves = piece->possible_moves(board, am_white, our_map, enemy_map);}
//     else{
//       param=-1;
//       //have to swap the vaulue of am_white and swap our_map and enemy_map
//       possible_moves = piece->possible_moves(board, !am_white, enemy_map, our_map);}
    
//     int mobility = possible_moves.size();

//     //  Weights depend on type of piece
//     if (piece->type() == "knight") {
//       score += param*mobility * 6; 
//     } else if (piece->type() == "bishop") {
//       score += param*mobility * 6; 
//     } else if (piece->type() == "rook") {
//       score += param*mobility * 10; 
//     } else if (piece->type() == "queen") {
//       score += param*mobility * 20; 
//     } else if (piece->type() == "king") {
//       score += param*mobility * 1;} // King (low weight to prevent overvaluation)
            
//   }

//   return score;
// };

// Identifies the "king zone," a group of squares around the king, including nearby adjacent squares and forward-facing squares, which are critical for king safety evaluation.
std::vector<std::string> find_king_zone(std::vector<piece_t*>& board, bool my_king, bool am_white){
  piece_t* king = find_king(board, my_king);
  
  //we define the king zone as all the positions can reach(if there were no pieces around it) & 1 more forward squares facing the enemy direction
  std::vector<std::string> king_zone;
  for(int i=-1; i<=1; i++){
    for(int j=-1; j<=1; j++){
      if(king->column+i>=1 && king->column+i<=8 && king->row+j>=1 && king->row+j<=8){
        king_zone.push_back(convert_chessboard_position(king->column+i, king->row+j));}
    }
  }
  if((my_king && am_white) || (!my_king && !am_white)){
    //looking for a white king zone
    if(king->row <= 6){
      if(king->column-1>=1){king_zone.push_back(convert_chessboard_position(king->column-1, king->row+2));}
      if(king->column+2<=8){king_zone.push_back(convert_chessboard_position(king->column, king->row+2));}
      if(king->column+1<=8){king_zone.push_back(convert_chessboard_position(king->column+1, king->row+2));}
    }
  }else{
    //looking for a black king zone
    if(king->row >= 2){
      if(king->column-1>=1 ){king_zone.push_back(convert_chessboard_position(king->column-1, king->row-2));}
      if(king->column+2<=8){king_zone.push_back(convert_chessboard_position(king->column, king->row-2));}
      if(king->column+1<=8){king_zone.push_back(convert_chessboard_position(king->column+1, king->row-2));}
    }
  }

  return king_zone;
}

// Counts the number of enemy attacks targeting the king's zone and assigns weights to those attacks based on the attacking piece type (e.g., queen, rook, bishop).
int get_attack_counter( std::vector<piece_t*>& board, bool my_eval, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string, std::vector<std::string> king_zone){
 
  int attack_counter = 0;
    std::vector<std::string> enemy_moves = all_enemy_moves_final(
        board, am_white, our_map, enemy_map, our_map_string, enemy_map_string
    );
    for ( auto& move : enemy_moves)
    {
        std::string from_sq = move.substr(0, 2);  
        std::string to_sq   = move.substr(2, 2);  
        std::string piece_type = enemy_map_string[from_sq];
        if (piece_type == "pawn" || piece_type == "king")
            continue;
        if (std::find(king_zone.begin(), king_zone.end(), to_sq) != king_zone.end())
        {
            if (piece_type == "knight" || piece_type == "bishop"){
                attack_counter -= 20;
            }
            else if (piece_type == "rook"){
                attack_counter -= 30;
            }
            else {
                attack_counter -= 50;
            }
        }
    }

    return attack_counter;
}


// Calculates the "king shield" score based on the presence of friendly pawns around the king, particularly in forward and adjacent positions. Assigns a positive score for such pawns.
int calculate_king_shield( std::vector<piece_t*>& board, bool my_eval, bool am_white) {
    piece_t* king = find_king(board, my_eval);
    int shield_score = 0;

    // Check for pawns in front and to the sides of the king
    int king_row = king->row;
    int king_col = king->column;

    std::vector<int> king_shield = {king_col-1, king_col, king_col+1};
        for (piece_t* piece : board) {
            if (piece->type() == "pawn" && piece->my_piece == my_eval) {
                if (piece->row == (am_white ? king_row + 1 : king_row - 1) && std::find(king_shield.begin(), king_shield.end(), piece->column) != king_shield.end()) {
                    shield_score += 10; // Assign a positive score for pawns near the king
                }
            }
        }

    return shield_score;
}

// Calculates the mobility of friendly pieces within the king's zone, rewarding pieces that can defend or influence this critical area. Helps assess the player's ability to protect the king.    
int get_mob_near_king( std::vector<piece_t*>& board, bool my_eval, bool am_white,std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string, std::vector<std::string> king_zone){
   int mob_near_king = 0;
    std::vector<std::string> moves;
    if (my_eval){
        moves = all_our_moves_final(
            board, am_white, our_map, enemy_map, our_map_string, enemy_map_string
        );
    }
    else{
        moves = all_enemy_moves_final(
            board, am_white, our_map, enemy_map, our_map_string, enemy_map_string
        );
    }
    for ( auto& mv : moves){
        std::string from_sq = mv.substr(0, 2);
        std::string to_sq   = mv.substr(2, 2);
    
        std::string piece_type = my_eval ? our_map_string[from_sq]  : enemy_map_string[from_sq];
        if (piece_type == "king") { continue;
        }
        if (std::find(king_zone.begin(), king_zone.end(), to_sq) != king_zone.end()){
            mob_near_king += 10;
        }
    
}
    return mob_near_king;

    };

// Evaluates the overall safety of the king by combining factors such as the king shield score, enemy attacks on the king's zone, and friendly mobility near the king.
int king_safety( std::vector<piece_t*>& board, bool my_eval, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map,  std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string){
  //this is a very hard function to implement but it is very rewarding
  // for now we will only consider the king shield, if there are open files adjecent to the king, threat detection,
  std::vector<std::string> king_zone = find_king_zone(board, my_eval, am_white);
  int king_shield_score = calculate_king_shield(board, my_eval, am_white);
  int attack_counter = get_attack_counter( board, my_eval, am_white, our_map, enemy_map, our_map_string, enemy_map_string, king_zone);
  int our_mob_near_king = get_mob_near_king(board, my_eval, am_white, our_map,enemy_map, our_map_string, enemy_map_string, king_zone);

  return attack_counter + king_shield_score + our_mob_near_king;
}

bool is_isolated( std::vector<piece_t*>& board, bool my_eval,  piece_t* pawn){
  for(piece_t* piece : board){
    if(piece->type() == "pawn" && piece->my_piece == pawn->my_piece){
      //if the piece is a pawn and it is in the same 'team' as the parameter pawn
      if((piece->column == pawn->column - 1 || piece->column == pawn->column + 1) && abs(piece->row - pawn->row) <=1){
        return false; // the pawn is not isolated
      }
    }
  }
  return true;
}

bool is_doubled( std::vector<piece_t*>& board, bool my_eval,  piece_t* pawn){
  //return true if the pawn is double false otherwise
  for(piece_t* piece: board){
    if(piece->type() == "pawn" && piece->my_piece == pawn->my_piece){
      //if the piece is a pawn and it is in the same 'team' as the parameter pawn
      if(piece->row == pawn->row && piece->column != piece->column){
        //they are on the same row but not on the same column: they are doubled pawns
        return true;}
    }
  }
  return false;
}

bool is_passed( std::vector<piece_t*>& board, bool my_eval,  piece_t* pawn, bool am_white){
  for(piece_t* piece: board){
    if(piece->type() == "pawn" && piece->my_piece != pawn->my_piece){
      if(abs(piece->column - pawn->column) <= 1){
        //this is the most strict condition, so that is the one I am doing first to save up computations
        if((am_white && piece->row > pawn->row) || (!am_white && piece->row < pawn->row)){
          return false;}}
    }
  }

  return true;
} 

// bool is_backwards( std::vector<piece_t*>& board, bool my_eval, piece_t* pawn, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map){
//   //we verify if the pawn is backwards or not, we need to check if the pawn is behind its own pawns on the same or adjacent files and if it is blocked by an opposing piece on its path
  
//   //for now the is_backwards function is very heavy and takes a long time to compute, given that a backwards pawn is not that disadvantageous, we may have to not take this backward pawns into account when we evaluate the chess position
//   for(piece_t* piece: board){
//     if(piece->type() == "pawn" && piece->my_piece == pawn->my_piece){
//       if(abs(piece->column - pawn->column) == 1){
//         if((am_white && piece->row < pawn->row) || (!am_white && piece->row > pawn->row)){
//           return false;}} //the pawn is not backward
//     }
//   }

//   std::vector<std::string> possible_moves = pawn->possible_moves(board, am_white, our_map, enemy_map);
//   if(possible_moves.size()==0){return true;}
//   for(std::string move: possible_moves){
//     int col = int(move[2] - 'a') + 1;
//     int row = int(move[3] - '0');
//     if(check_at_pos(board, am_white, col, row)){return true;}
//   }
//   return false;
// }

bool is_center_control(piece_t* pawn){
  if(pawn->column>=4 && pawn->column<=5 && pawn->row>=4 && pawn->row<=5){
    return true;}
  return false;
}

int evaluate_pawn_islands( std::vector<piece_t*>& board, bool my_eval, bool am_white){
  //we count how many pawn islands there are and change the pawn_structure score accordingly, penalizing more pawn islands
  std::vector<bool> files_with_pawns(8, false); // Track which files contain pawns
  for(piece_t* piece : board){
    if(piece->type() == "pawn" && piece->my_piece){
      files_with_pawns[piece->column-1] = true;}
  }
  //we count the number of disjoint pawn islands
  int islands = 0;
  bool in_island = false;
  for(bool has_pawn : files_with_pawns){
    if(has_pawn){
      if(!in_island){
        islands++;
        in_island = true;} //we have started a new island
    }else{
      //we end the current island
      in_island = false;}
  }
  return -20*(islands-1);
}

int pawn_structure( std::vector<piece_t*>& board, bool my_eval, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map){
  //look for: isolated pawns: pawn with no friendly pawns on adjacent files, doubled pawns: When two pawns occupy the same file, they are doubled pawns.
  //passed pawns:  pawn with no opposing pawns on its file or adjacent files (on its way to promotion), backward pawns: A pawn that is behind its neighboring pawns and cannot advance safely due to enemy control is backward
  // pawn chains: Connected pawns that protect one another form a pawn chain, pawn islands: Groups of connected pawns are called pawn islands. Fewer islands indicate a better structure. center control: Pawns that control the center squares (e.g., d4, d5, e4, e5) contribute to a strong position.
  int pawn_structure_score = 0;
  for(piece_t* piece: board){
    if(piece->type() == "pawn" && piece->my_piece==my_eval){
      // the piece is a pawn
      if(is_isolated(board, my_eval, piece)){
        
        pawn_structure_score -= 50;}
      if(is_doubled(board, my_eval, piece)){
        
        pawn_structure_score -= 30;}
      if(is_passed(board, my_eval, piece, am_white)){
        // we attribute different weight to the rank of the passed pawn
        if(am_white){
          //the pawn is our pawn and we are white thus the pawn is white
          if(piece->row <= 4){
            
            pawn_structure_score += 50;
            }
          else if(piece->row == 5){
          
            pawn_structure_score += 60;}
          else{
            
            pawn_structure_score += 70;}
        }else{
          //the pawn is our pawn and we are black thus the pawn is black
          if(piece->row >= 5){
           
            pawn_structure_score += 50;}
          else if(piece->row == 4){
          
            pawn_structure_score += 60;}
          else{
            
            pawn_structure_score += 70;}}}
      //the is_backwards function is very very heavy computationally so for now we are deciding to not consider it in our pawn structure evaluation
      //if(is_backwards(board, my_eval, piece, am_white, our_map, enemy_map)){
        //the is_backwards function is very heavy computationally so we may not implement it if the evaluation function takes to long, especially as backward pawns are not that important
      //  pawn_structure_score -= 20;}
      if(is_center_control(piece)){
       
        //checking if the pawn is on one of the squares e4/d4/e5/d5
        pawn_structure_score += 20;}
    }
  }
    pawn_structure_score += evaluate_pawn_islands(board, my_eval, am_white);

  return pawn_structure_score;
}

// Center control: check if we or the enmy have potential moves they can make in the center of the board (important area)
// int eval_center_control( std::vector<piece_t*>& board, bool am_white,  std::map<std::string, bool>& our_map,  std::map<std::string, bool>& enemy_map) {
//     std::vector<std::string> center_squares = {"d4", "e4", "d5", "e5"}; // most important squares for center control
//     std::vector<std::string> extended_center_squares = {"c3", "c4", "c5", "c6", "f3", "f4", "f5", "f6"}; // secondary to evaluation of center control 
//     int center_control_score = 0;
//     for (piece_t* piece : board) {
//         std::vector<std::string> possible_moves = piece->possible_moves(board, am_white, our_map, enemy_map);
//         for (std::string move: possible_moves) {
//             // Check if the move is targeting a center square
//             if (std::find(center_squares.begin(), center_squares.end(), move) != center_squares.end()) {
//                 if (piece->my_piece) {
//                     center_control_score += 20;  // our piece controls central square
//                 } else {
//                     center_control_score -= 20; // enemy controls
//                 }
//             }
//           //  Same logic for extended squares
//             else if (std::find(extended_center_squares.begin(), extended_center_squares.end(), move) != extended_center_squares.end()) {
//                 if (piece->my_piece) {
//                     center_control_score += 10;// our piece controls extended square
//                 } else {
//                     center_control_score -= 10; //enemy controls 
//                 }
//             }
//         }
//     }

//     return center_control_score;
// }

// Space: loop over all pieces, all of their possible moves and calculate how valuable they are depending on if they can control enemy territory, central positions or regular positions
int eval_space(std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& our_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string)
{
    int space_score = 0;
    std::vector<std::string> central_squares     = {"d4", "e4", "d5", "e5"};
    std::vector<std::string> extended_center_sqs = {"c3","c4","c5","c6","f3","f4","f5","f6"};
    std::vector<std::string> opponent_territory;
    std::vector<std::string> my_territory; 
    if (am_white)
    {
        for (char col = 'a'; col <= 'h'; ++col)
        {
            for (int row = 1; row <= 8; ++row)
            {
                std::string square = std::string(1, col) + std::to_string(row);
                if (row >= 5) {
                    opponent_territory.push_back(square);
                } else {
                    my_territory.push_back(square);
                }
            }
        }
    }
    else 
    {for (char col = 'a'; col <= 'h'; ++col)
        {
            for (int row = 1; row <= 8; ++row)
            {
                std::string square = std::string(1, col) + std::to_string(row);
                if (row <= 4) {
                    opponent_territory.push_back(square);
                } else {
                    my_territory.push_back(square);
                }
            }
        }
    }
    std::vector<std::string> our_moves = all_our_moves_final(
        board, am_white, our_map, enemy_map, our_map_string, enemy_map_string
    );
    std::vector<std::string> enemy_moves = all_enemy_moves_final(
        board, am_white, our_map, enemy_map, our_map_string, enemy_map_string
    );
    for (const auto& mv : our_moves)
    {
        std::string from_sq = mv.substr(0, 2);
        std::string to_sq   = mv.substr(2, 2);
        std::string piece_type = our_map_string[from_sq];
        if (std::find(central_squares.begin(), central_squares.end(), to_sq) != central_squares.end())
        {
            space_score += 20;
        }
        else if (std::find(extended_center_sqs.begin(), extended_center_sqs.end(), to_sq) != extended_center_sqs.end())
        {
            space_score += 10;
        }
        else if (std::find(opponent_territory.begin(), opponent_territory.end(), to_sq) != opponent_territory.end())
        {
            space_score += 5;
        }
        else
        {
            if (piece_type == "knight" || piece_type == "bishop") {
                space_score += 5;
            }
            else if (piece_type == "rook") {
                space_score += 10;
            }
            else if (piece_type == "queen") {
                space_score += 20;
            }

        }
    }
    for (const auto& mv : enemy_moves)
    {
        std::string from_sq = mv.substr(0, 2);
        std::string to_sq   = mv.substr(2, 2);
        std::string piece_type = enemy_map_string[from_sq];
        if (std::find(central_squares.begin(), central_squares.end(), to_sq) != central_squares.end())
        {
            space_score -= 20;
        }
        else if (std::find(extended_center_sqs.begin(), extended_center_sqs.end(), to_sq) != extended_center_sqs.end()){
            space_score -= 10;
        }
        else if (std::find(my_territory.begin(), my_territory.end(), to_sq) != my_territory.end()) {
            space_score -= 5;
        }
        else
        {
            if (piece_type == "knight" || piece_type == "bishop") {
                space_score -= 5;
            }
            else if (piece_type == "rook") {
                space_score -= 10;
            }
            else if (piece_type == "queen") {
                space_score -= 20;
            }
        }
    }
    return space_score;
}





std::vector<std::pair<std::string, int>> mini_eval(std::vector<piece_t*>& board, bool am_white, int game_phase, std::map<std::string, bool>& our_map, std::map<std::string, bool>& enemy_map, std::map<std::string,std::string>& our_map_string, std::map<std::string,std::string>& enemy_map_string, bool our_turn) {
    std::vector<std::pair<std::string, int>> result;


    std::vector<std::string> possible_moves;

    if (our_turn) {
        possible_moves = all_our_moves_final(
            board, am_white, our_map, enemy_map, our_map_string, enemy_map_string
        );
    } else {
        possible_moves = all_enemy_moves_final(
            board, am_white, our_map, enemy_map, our_map_string, enemy_map_string
        );
    }

    for ( std::string& move : possible_moves) {
        std::string move_rewrite=change_format(move);
        std::vector<piece_t*> board_copy = create_copy_board(board);
        perform_move(board_copy, move_rewrite, our_map, enemy_map);
        int score = val_of_board(board_copy, am_white, game_phase);
        
        // Add the move and its score directly to the result vector
        result.emplace_back(move, score);
    }


    if (our_turn) {
        // Sort in descending order (highest scores first)
        std::sort(result.begin(), result.end(), []( auto& a,  auto& b) {
            return a.second > b.second;
        });
    } else {
        // Sort in ascending order (lowest scores first)
        std::sort(result.begin(), result.end(), []( auto& a,  auto& b) {
            return a.second < b.second;
        });
    }

    return result;
}