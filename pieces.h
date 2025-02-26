#ifndef PIECES_H
#define PIECES_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <map>

extern std::string greedy_move_external;
extern std::string file_name_external;

// Main structure used to rpz pieces
struct piece_t
{
  bool my_piece;
  int row;
  int column;
  virtual std::string type(){
    return "none";
  }
  bool has_moved;
  virtual int get_points(){return 0;}
  piece_t() : my_piece(false), row(-1), column(-1) {}; // TODO not sure about false here
  piece_t(int column, int row, bool my_piece) : my_piece(my_piece), row(row), column(column) {}
  virtual int get_points() const { return 0;}
  virtual piece_t* create_copy()=0;//used to create a copy of board, usefull when testing if their is check with moves
  virtual std::vector<std::string> possible_moves( std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king) = 0 ;
  bool pined_in_direct( std::vector<piece_t*>& board,bool am_white, std::string direction,int column_king,int row_king, std::map<std::string,std::string>& our_map, std::map<std::string,std::string>& enemy_map);
  virtual void print_piece(); // prints every piece overrided in every piece structure to print piece and in Capital letter if ours but small letters if not ourse
  virtual ~piece_t() {}
};

//empty piece for when we print the chess board
struct empty_t : piece_t {
  std::string type() override{
    return "none";
  }
  empty_t() : piece_t(-1, -1, false) {} // No real position or ownership
  std::vector<std::string> possible_moves( std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king) override {
    return {}; // Empty piece has no possible moves
  }


  int get_points() const override {return 0;}
  void print_piece() override {
    std::cout << "  "; // Print space for an empty square
  }
  piece_t* create_copy() override{
    return new empty_t();
  }
};

// All specific pieces:
struct pawn_t : piece_t{
  bool has_moved = false; // useful for determining first move (can it move 2 squares forward). obv false by default
  int get_points() override {return 100;}
  std::string type() override{
    return "pawn";
  }
  using piece_t::piece_t;      // Inherits the constructor from piece_t
  void print_piece() override; // print mP if ours and oP if not ours
  std::vector<std::string> possible_moves( std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king) override;

  piece_t* create_copy() override{
    return new pawn_t(column,row,my_piece);
  }
};


struct king_t : piece_t{
  bool has_moved = false; //need this to determine if we can castle or not
  int get_points() const override {return 20;} //Value TBD
  std::string type() override{
    return "king";
  }
  using piece_t::piece_t;                       // Inherits the constructor from piece_t
  void print_piece() override;                  // prints K if ours and k if not ours
  std::vector<std::string> possible_moves( std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king) override;
  piece_t* create_copy() override {
    return new king_t(column,row,my_piece);
  }
};

struct queen_t : piece_t
{
  int get_points() override {return 900;}
  std::string type() override{
    return "queen";
  }
  using piece_t::piece_t;      // Inherits the constructor from piece_t
  void print_piece() override; // prints Q if ours and q if not ours
  std::vector<std::string> possible_moves( std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king) override;
  piece_t* create_copy() override{
    return new queen_t(column,row,my_piece);
  }
};

struct bishop_t : piece_t
{
  int get_points() override {return 300;}
  std::string type() override{
    return "bishop";
  }
  using piece_t::piece_t;      // Inherits the constructor from piece_t
  void print_piece() override; // prints B if ours and b if not ours
  std::vector<std::string> possible_moves( std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king) override;
  piece_t* create_copy() override{
    return new bishop_t(column,row,my_piece);
  }
};

struct rook_t : piece_t{
  bool has_moved = false; //need this to determine if we can castle or not
  int get_points() override {return 500;}
  std::string type() override{
    return "rook";
  }
  using piece_t::piece_t;      // Inherits the constructor from piece_t
  void print_piece() override; // prints R if ours and r if not ours
  std::vector<std::string> possible_moves( std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king) override;
  piece_t* create_copy() override{
    return new rook_t(column,row,my_piece);
  }
};

struct knight_t : piece_t
{
  int get_points() override {return 300;}
  std::string type() override {
    return "knight";
  }
  using piece_t::piece_t;      // Inherits the constructor from piece_t
  void print_piece() override; // prints KN if ours and kn if not ours
  std::vector<std::string> possible_moves( std::vector<piece_t*>& board, bool am_white, std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string,int column_king,int row_king) override;
  piece_t* create_copy() override{
    return new knight_t(column,row,my_piece);
  }
};

std::map<std::string,bool> create_our_map(std::vector<piece_t*>& board);
std::map<std::string,bool> create_enemy_map(std::vector<piece_t*>& board);
std::map<std::string,std::string> create_our_map_string(std::vector<piece_t*>& board,std::map<std::string,bool>& our_map);
std::map<std::string,std::string> create_enemy_map_string(std::vector<piece_t*>& board,std::map<std::string,bool>& enemy_map);
std::vector<std::string> all_moves_our_pieces(std::vector<piece_t*>& board,bool am_white,std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string);
std::vector<std::string> all_moves_enemy_pieces(std::vector<piece_t*>& board,bool am_white,std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string);
std::vector<piece_t*> create_copy_board(std::vector<piece_t*>& board);
std::string return_random_move(std::vector<piece_t*>& board, bool am_white);
std::vector<std::string> our_final_moves_check(std::vector<piece_t*>& board, bool am_white);
piece_t* find_king(std::vector<piece_t*>& board, bool my_king);
std::string convert_chessboard_position(int column, int row);
piece_t* return_piece(std::vector<piece_t *>& board, int column, int row);
bool check_at_pos(std::vector<piece_t*>& board,bool am_white,int column,int row,std::map<std::string, std::string>& our_map, std::map<std::string, std::string>& enemy_map); //checks if in a position column and row the king is in check assuming we change the kings position so the old position of the king cant block the check

std::tuple<
    std::map<std::string, bool>, 
    std::map<std::string, bool>, 
    std::map<std::string, std::string>, 
    std::map<std::string, std::string>
> create_chessboard_all_maps(std::vector<piece_t*>& board);
std::tuple<std::map<std::string, bool>, std::map<std::string, bool> > create_chessboard_maps_bools(std::vector<piece_t*>& board);


//functions to use

//creations of maps
std::map<std::string,bool> create_our_map(std::vector<piece_t*>& board); // creer map avec true or false, true si une de nos positions a cette endroit
std::map<std::string,bool> create_enemy_map(std::vector<piece_t*>& board); // creer map avec true or false mais pour ennemi
std::map<std::string,std::string> create_our_map_string(std::vector<piece_t*>& board,std::map<std::string,bool>& our_map);// creer map avec "none" partourt a part la ou ya une de nos pieces et c'est le nom de la piece, prends en argument la map avec true or false donc in faut la creer avant
std::map<std::string,std::string> create_enemy_map_string(std::vector<piece_t*>& board,std::map<std::string,bool>& enemy_map);// meme chose mais puor l'ennemi


piece_t* find_king(std::vector<piece_t*>& board, bool my_king);//my_king is if we are searching for our king then true or enemy king then false

//checking if a team is in chekc
bool enemy_team_check(std::vector<piece_t*>& board, bool am_white,std::map<std::string, std::string>& our_map_string, std::map<std::string, std::string>& enemy_map_string); // prends en argument tout les trucs ecrit, c'est les arguments de la facon intuitive donc am_white c'est bien si nous on est am_white pareil puor les maps, our_maps cest bien notr maps, les inversion sont faits dans la fonction vous inquitiew pas
bool our_team_check(std::vector<piece_t*>& board, bool am_white,std::map<std::string, std::string>& our_map_string, std::map<std::string, std::string>& enemy_map_string); // evidemt



//all legal moves
std::vector<std::string> all_enemy_moves_final(std::vector<piece_t*>& board,bool am_white,std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string); // move final de lennemi, ca regarde si il est en check et tt, comme la fonction enemy_check, les arguments sont intuitif, donc am_white c'est si on est white, ours maps c'est bien nos maps
std::vector<std::string> all_our_moves_final(std::vector<piece_t*>& board,bool am_white,std::map<std::string,bool>& my_map, std::map<std::string,bool>& enemy_map, std::map<std::string,std::string>& my_map_string, std::map<std::string,std::string>& enemy_map_string); // toutes les moves legals, prends en compte si on est en check


#endif