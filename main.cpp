#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <chrono>
#include "print.h"
#include "initialize.h"
#include "pieces.h"
#include "eval.h"
#include "tree.h"

std::string greedy_move_external;
std::string file_name_external;
void print_result(const std::vector<std::pair<std::string, int>>& result) {
      std::cout << "Result" << std::endl;
    for (const auto& [move, score] : result) {
        std::cout << "Move: " << move << ", Score: " << score << std::endl;
    }
}

void print_vector(const std::vector<int> vec)
{
  std::cout << "[";
  for (size_t i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i];
    if (i != vec.size() - 1)
    {
      std::cout << ", ";
    }
  }
  std::cout << "]";
}


// Function to print the map
void print_map(const char *prefix, const std::map<std::vector<int>, bool> enemy_positions)
{
  std::cout << std::string(prefix) + "Positions: " << std::endl;
  for (const auto &pair : enemy_positions)
  {
    std::cout << "Key: ";
    print_vector(pair.first); // Print the vector<int> key
    std::cout << " -> Value: " << (pair.second ? "true" : "false") << std::endl;
  }
  std::cout << "-----------------------" << std::endl;
}

void writeMoveToFile(const std::string& move, const std::string& filename) {
    // Create an output file stream
    std::ofstream file(filename);

    // Check if the file was successfully opened
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Write the move to the file
    file << move;

    // Close the file
    file.close();

    std::cout << "Move written to " << filename << " successfully!\n";
    std::exit(0);
}

std::vector<piece_t*> switch_board(std::vector<piece_t*> board){
  std::vector<piece_t*> new_board;
  for(auto piece: board){
    piece_t* new_piece=piece->create_copy();
    new_piece-> my_piece = !piece->my_piece;
    new_board.push_back(new_piece);
  }
  return new_board;
}

int main(int argc, char **argv){
  if (argc < 4)
  {
    std::cerr << "Usage: " << argv[0] << " <path_to_moves_file>\n";
    return 1;
  }
  std::cout << argc;
 
 
  // we receive a text file with all the moves that have been made until now
  auto [board_greedy, my_pos_greedy, enemy_pos_greedy, am_white_greedy, game_phase_greedy] = update_chessboard(argv[2]);
  std::map<std::string, std::string> my_pos_string_greedy = create_our_map_string(board_greedy, my_pos_greedy);
  std::map<std::string, std::string> enemy_pos_string_greedy = create_enemy_map_string(board_greedy, enemy_pos_greedy);


 /*
 for(auto move: possible_moves){
  std::cout << move << " ";}
 std::cout << "\n";*/
 /*
 clock_t start = clock();
  for(int i=1;i<10000;i++){
      for(auto piece : board){
        std::vector<std::string> possible_moves = piece->possible_moves(board, am_white, my_pos, enemy_pos);
      }
  }
  clock_t end = clock();
        double duration = double(end - start) / CLOCKS_PER_SEC;
        std::cout << "Execution timefinal: " << duration << " s" << std::endl;*/
 
//  std::cout << "random move: " << random_move << "\n";

/*
std::cout << "Eval with original board:" << std::endl;
 int eval = eval_position(board, am_white, 0,my_pos,enemy_pos, my_pos_string, enemy_pos_string);
//  display_board(board);
 std::cout << "eval: " << eval << "\n";
 std::vector<piece_t*> switched = switch_board(board);
 std::cout << "--------------------------------------------------------------------------------" << std::endl;
 std::cout << "Eval with switched board:" << std::endl;
//  display_board(switched);
 int switch_eval = eval_position(switched, !am_white, 0, enemy_pos, my_pos, enemy_pos_string,my_pos_string);
  std::cout << "switched eval: " << switch_eval << "\n";
*/

  int max_depth = 5;
  if(game_phase_greedy == 1 || game_phase_greedy==2){
    max_depth = 3;
  }
  display_board(board_greedy);
  std::vector<std::pair<std::string, int>> greedy_moves = mini_eval(board_greedy, am_white_greedy, game_phase_greedy, my_pos_greedy, enemy_pos_greedy, my_pos_string_greedy, enemy_pos_string_greedy, true);
  std::pair greedy_move = greedy_moves[0];
  std::string greedy_move_final = greedy_move.first;
  greedy_move_external=greedy_move_final;
  file_name_external=argv[4];
  auto [board, my_pos, enemy_pos, am_white, game_phase] = update_chessboard(argv[2]);
  std::map<std::string, std::string> my_pos_string = create_our_map_string(board, my_pos);
  std::map<std::string, std::string> enemy_pos_string = create_enemy_map_string(board, enemy_pos);

  std::pair<std::string, int> minimax_eval = find_best_move(board, am_white, my_pos, enemy_pos, my_pos_string, enemy_pos_string,
                                                            max_depth, argv[4], game_phase, greedy_move_final);
  //for now let us try to go at a depth of 2, we also initialize the board to have no nodes and when we first call the minimax it is always our turn
  //for the root node of the tree we can assume that the previous move was no move and we evaluate the board at that position for the final parameter
  std::cout << "minimax eval: " << minimax_eval.first << " " << minimax_eval.second << "\n";
  
  writeMoveToFile(minimax_eval.first, argv[4]);
 return 0;
 
};



