#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <limits>
#include "pieces.h"
#include <chrono>

// TreeNode class to represent the game state tree
class TreeNode {
public:
    std::vector<piece_t*> board; // Current state of the chessboard
    std::vector<TreeNode*> children; // Child nodes
    bool is_my_turn; // Indicates if it's the current player's turn
    std::string move;
    int move_score;
    int depth; //depth of node in the tree
    std::string parent_move;

    // Constructor
    TreeNode(std::vector<piece_t*> board_state, 
             std::vector<TreeNode*> child_nodes, 
             int depth, bool my_turn, std::string cur_move, int cur_score, std::string parent_move)
        : board(board_state), 
          children(child_nodes),
          depth(depth), is_my_turn(my_turn), move(cur_move), move_score(cur_score), parent_move(parent_move) {}

    // Destructor to clean up dynamically allocated pieces
    ~TreeNode() {
        for (piece_t* piece : board) {
            delete piece;
        }
    }
};

TreeNode* construct_tree(std::vector<piece_t*> board, int depth, bool is_my_turn, bool am_white,  const std::string& move, int move_score);

std::pair<std::string, int> minimax(TreeNode* node, bool am_white, int depth, bool our_move, int alpha, int beta, int max_depth,
                                    const std::chrono::time_point<std::chrono::high_resolution_clock>& start_time, int time_limit,
                                    std::string& best_move_at_root, int& best_score_at_root, const std::string& filename,
                                    int game_phase);

void print_tree(TreeNode* node, int depth, const std::string& prefix, bool is_last);
void print_result(const std::vector<std::pair<std::string, int>>& result);
void writeMoveToFile(const std::string& move, const std::string& filename);

std::pair<std::string, int> find_best_move(std::vector<piece_t*> board, bool am_white, std::map<std::string, bool> my_pos,
    std::map<std::string, bool> enemy_pos, std::map<std::string, std::string> my_pos_string,
    std::map<std::string, std::string> enemy_pos_string, int max_depth, const std::string& filename, int game_phase,std::string best_greedy_move);
#endif