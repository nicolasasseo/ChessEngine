#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <string>
#include <vector>
#include <map>
#include "pieces.h"

std::string change_format(std::string line);

std::tuple<std::vector<piece_t *>, std::map<std::string, bool>, std::map<std::string, bool>> initialise_chessboard(bool am_white);
void perform_move(std::vector<piece_t *> &board, std::string move, std::map<std::string, bool> &my_positions, std::map<std::string, bool> &enemy_positions);
std::tuple<std::vector<piece_t*>, std::map<std::string, bool>, std::map<std::string, bool>, bool, int> update_chessboard(const std::string &filePath);

#endif
