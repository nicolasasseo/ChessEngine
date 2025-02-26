#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <memory>
#include <algorithm>
#include "print.h"
#include "initialize.h"
#include "pieces.h"
#include "eval.h"
#include "tree.h"
#include <chrono>




bool is_checkmate(std::vector<piece_t*>& board, bool am_white, bool our_move, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string) {

    // Generate all legal moves for the opponent
    std::vector<std::string> legal_moves;
    if(our_move){
      legal_moves = all_enemy_moves_final(board, am_white, my_map, enemy_map, my_map_string, enemy_map_string); //need to generate the legal moves of the other team than the move that is performed
    }else{
        legal_moves = all_our_moves_final(board, am_white, my_map, enemy_map, my_map_string, enemy_map_string);
    }
     

    // If there are no legal moves, it's a checkmate
    return legal_moves.empty();
}

bool is_stalemate(std::vector<piece_t*>& board, bool am_white, bool our_move, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string) {
    // Generate all legal moves for the opponent
    std::vector<std::string> legal_moves;
    if(our_move){
      legal_moves = all_enemy_moves_final(board, am_white, my_map, enemy_map, my_map_string, enemy_map_string); //need to generate the legal moves of the other team than the move that is performed
    }else{
        legal_moves = all_our_moves_final(board, am_white, my_map, enemy_map, my_map_string, enemy_map_string);
    }
     
    // If there are no legal moves, it's a stalemate
    return legal_moves.empty();
}

std::vector<TreeNode*> construct_ordered_children(TreeNode* node, bool am_white, int depth, int game_phase, bool our_move){
    // this function will be called inside of the minimax algorithm and is tasked with created the "children of the current node"
    std::map<std::string, bool> our_map = create_our_map(node->board);
    std::map<std::string, bool> enemy_map = create_enemy_map(node->board);
    std::map<std::string,std::string> our_map_string = create_our_map_string(node->board, our_map);
    std::map<std::string,std::string> enemy_map_string = create_enemy_map_string(node->board, enemy_map);


    std::vector<TreeNode*> children; //initialize the empty vector of children
    std::vector<std::pair<std::string, int>> ordered_moves_mini = mini_eval(node->board, am_white, game_phase, our_map, enemy_map, our_map_string, enemy_map_string, our_move); //create a vector where the moves are ordered with the mini_eval function
    //display_board(node->board);
    int size = ordered_moves_mini.size();
    if(game_phase == 0){
        size = std::min(size, 5);
    }else if(game_phase == 2){
        size = std::min(size, 15);
    }
    for(int i=0; i<size; i++){ //here to put in place the idea I had with horace, only need to iterate till i<n for an arbitrary n we still need to decide on
        std::string child_move = ordered_moves_mini[i].first;
        //std::cout << "Depth: " << depth << ", Creating child for move: " << child_move << "\n";
        //display_board(node->board);
        std::vector<piece_t*> new_board = create_copy_board(node->board); //copy of board

        std::string move_format = change_format(child_move);
        //std::cout << child_move << "\n";
        //display_board(new_board);
        perform_move(new_board, move_format, our_map, enemy_map);
        std::map<std::string, bool> our_map_copy = create_our_map(new_board);
        std::map<std::string, bool> enemy_map_copy = create_enemy_map(new_board);
        std::map<std::string,std::string> our_map_string_copy = create_our_map_string(new_board, our_map_copy);
        std::map<std::string,std::string> enemy_map_string_copy = create_enemy_map_string(new_board, enemy_map_copy);
        int score_eval = eval_position(new_board, am_white, game_phase, our_map_copy, enemy_map_copy, our_map_string_copy, enemy_map_string_copy);
        int final_eval;
        if(!our_move){final_eval = -1*score_eval;}
        else{final_eval = score_eval;}
        auto child = new TreeNode(new_board, {}, depth+1, !our_move, child_move, final_eval, node->move); //construct the children node based on the new board
        
        children.push_back(child);

    }
    return children;
}

std::pair<std::string, int> minimax(TreeNode* node, bool am_white, int depth, bool our_move, int alpha, int beta, int max_depth,
                                    const std::chrono::time_point<std::chrono::high_resolution_clock>& start_time, int time_limit,
                                    std::string& best_move_at_root, int& best_score_at_root,  const std::string& filename, int game_phase) {
    // Base case: if depth is 0 or the node has no children (i.e., the game ends)
    //std::cout << "Evaluating board at depth " << depth << " move: " << node->move << " score: " << node->move_score << " parent move: " << node->parent_move << std::endl;
    
    auto now = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() >= time_limit * 1000) {
        std::cout << "writing best move due to time limit \n";
        writeMoveToFile(best_move_at_root, filename);
        return {best_move_at_root, best_score_at_root}; // Return the best move found so far at depth 1
    }
    
    if(depth == max_depth){
        return {node->move, node->move_score};}
    std::map<std::string,bool> our_map = create_our_map(node->board);
    std::map<std::string,bool> enemy_map = create_enemy_map(node->board);
    std::map<std::string,std::string> our_map_string = create_our_map_string(node->board, our_map);
    std::map<std::string,std::string> enemy_map_string = create_enemy_map_string(node->board, enemy_map);
    if(our_move){
        if(enemy_team_check(node->board, am_white, our_map_string, enemy_map_string)){

            if(is_checkmate(node->board, am_white, our_move, our_map, enemy_map, our_map_string, enemy_map_string)){
                return {node->move, 10000};
            }else if(is_stalemate(node->board, am_white, our_move, our_map, enemy_map, our_map_string, enemy_map_string)){
                return {node->move, 0};
            }
        }
    }else{
        //it is not our move
        if(our_team_check(node->board, am_white, our_map_string, enemy_map_string)){
            if(is_checkmate(node->board, am_white, our_move, our_map, enemy_map, our_map_string, enemy_map_string)){
                return {node->move, -10000};
            }else if(is_stalemate(node->board, am_white, our_move, our_map, enemy_map, our_map_string, enemy_map_string)){
                return {node->move, 0};
            }
        }
    }

    if (our_move) {
        // Maximizing player
        node->children = construct_ordered_children(node, am_white, depth + 1, game_phase, true); // Our move
        int max_eval = std::numeric_limits<int>::min();
        std::string best_move;
        
        for (TreeNode* child : node->children) {
            //std::cout << "Maximizing: evaluating " << child->move << " with score: " << child->move_score << "\n";
            auto [child_move, eval] = minimax(child, am_white, depth + 1, !our_move, alpha, beta, max_depth,
                                            start_time, time_limit, best_move_at_root, best_score_at_root, filename, game_phase);
            if (eval > max_eval) {
                max_eval = eval;
                best_move = child->move;

                
            }


            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        //std::cout << "Best move for our player: " << best_move << " with score: " << max_eval << " with parent: " << node->parent_move << " depth: " << node->depth<< "\n";
        return {best_move, max_eval}; // Return best move and its score
    } else {
        // Minimizing player
        node->children = construct_ordered_children(node, am_white, depth + 1, game_phase, false); // Opponent's move
        int min_eval = std::numeric_limits<int>::max();
        std::string best_move;
        
        for (TreeNode* child : node->children) {
            auto [child_move, eval] = minimax(child, am_white, depth + 1, !our_move, alpha, beta, max_depth,
                                                start_time, time_limit, best_move_at_root, best_score_at_root, filename, game_phase);
            if (eval < min_eval) {
                min_eval = eval;
                best_move = child->move;

                
            }
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        //std::cout << "Best move for opponent player: " << best_move << " with score: " << min_eval << " with parent: " << node->move << " depth: " << node->depth << "\n";
        return {best_move, min_eval}; // Return best move and its score
    }
}


void print_tree(TreeNode* node, int depth = 0, const std::string& prefix = "", bool is_last = true) {
    if (!node) return;

    // Print the current node's move and score
    std::cout << prefix << (is_last ? "└── " : "├── ")
              << "Move: " << (node->move.empty() ? "ROOT" : node->move)
              << ", Score: " << node->move_score << std::endl;

    // Prepare the prefix for the children
    std::string new_prefix = prefix + (is_last ? "    " : "│   ");

    // Recursively print children
    for (size_t i = 0; i < node->children.size(); ++i) {
        bool is_last_child = (i == node->children.size() - 1);
        print_tree(node->children[i], depth+1, new_prefix, is_last_child);
    }
}


std::pair<std::string, int> find_best_move(std::vector<piece_t*> board, bool am_white, std::map<std::string, bool> my_pos, std::map<std::string, bool> enemy_pos, std::map<std::string,
                                            std::string> my_pos_string, std::map<std::string, std::string> enemy_pos_string, int max_depth,  const std::string& filename, int game_phase,std::string greedy_final_move){
    
    auto start_time = std::chrono::high_resolution_clock::now();
    int best_score_at_root = std::numeric_limits<int>::min();
    int time_limit = 9;
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();


    TreeNode* root = new TreeNode(board, {}, 0, true, "root", eval_position(board, am_white, game_phase, my_pos, enemy_pos, my_pos_string, enemy_pos_string), "");
    std::pair<std::string, int> minimax_eval = minimax(root, am_white, 0, true, alpha, beta, max_depth,
                                                        start_time, time_limit, greedy_final_move, best_score_at_root, filename, game_phase);

    return minimax_eval;
}





