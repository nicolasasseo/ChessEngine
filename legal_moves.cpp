#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include "pieces.h"
#include "initialize.h"
#include "print.h"
#include "utility.h"
#include "tree.h"
#include <map>

piece_t* return_piece(std::vector<piece_t *>& board, int column, int row)
{ // returns piece in board at position asked
  for (piece_t *value : board)
  {
    if (column == value->column && row == value->row)
    {
      return value;
    }
  }
  writeMoveToFile(greedy_move_external,file_name_external);
  error("piece asked at position was not found");
}

bool piece_t::pined_in_direct( std::vector<piece_t*>& board,bool am_white, std::string direction, int column_king,int row_king, std::map<std::string,std::string>& our_map, std::map<std::string,std::string>& enemy_map){// if true, cannot move in another direction that this direction
    int i;
    int j;
    if(direction=="column"){
        if(column_king==column){
            if(row_king>row){
                for(int i=row+1;i<row_king-row;i++){
                    std::string position=convert_chessboard_position(column,i);
                    if(our_map[position]!="none" || enemy_map[position]!="none"){
                        return false;
                    }

                }
                for(int i=row-1;i>0;i--){
                    std::string position=convert_chessboard_position(column,i);
                    if(our_map[position]!="none"){
                        return false;
                    }
                    if(enemy_map[position]!="none"){
                        if(enemy_map[position]=="queen" || enemy_map[position]=="rook"){
                            return true;
                        }
                        return false;

                    }
                }
            }
            if(row_king<row){
                for(int i=row_king+1;i<row-row_king;i++){
                    std::string position=convert_chessboard_position(column,i);
                    if(our_map[position]!="none" || enemy_map[position]!="none"){
                        return false;
                    }
                }
                for(int i=row+1;i<9;i++){
                    std::string position=convert_chessboard_position(column,i);
                    if(our_map[position]!="none"){
                        return false;
                    }
                    if(enemy_map[position]!="none"){
                        piece_t* piece= return_piece(board,column,i);
                        if(enemy_map[position]=="queen" || enemy_map[position]=="rook"){
                            return true;
                        }
                        return false;

                    }
                }
            }


        }
    }
    if(direction=="row"){
        if(row_king==row){
            if(column_king>column){
                for(int i=column+1;i<column_king-column;i++){
                    std::string position=convert_chessboard_position(i,row);
                    if(our_map[position]!="none" || enemy_map[position]!="none"){
                        return false;
                    }

                }
                for(int i=column-1;i>0;i--){
                    std::string position=convert_chessboard_position(i,row);
                    if(our_map[position]!="none"){
                        return false;
                    }
                    if(enemy_map[position]!="none"){
                        if(enemy_map[position]=="queen" || enemy_map[position]=="rook"){
                            return true;
                        }
                        return false;

                    }
                }
            }
            if(column_king<column){
                for(int i=column_king+1;i<column-column_king;i++){
                    std::string position=convert_chessboard_position(i,row);
                    if(our_map[position]!="none" || enemy_map[position]!="none"){
                        return false;
                    }
                }
                for(int i=column+1;i<9;i++){
                    std::string position=convert_chessboard_position(i,row);
                    if(our_map[position]!="none"){
                        return false;
                    }
                    if(enemy_map[position]!="none"){
                        if(enemy_map[position]=="queen" || enemy_map[position]=="rook"){
                            return true;
                        }
                        return false;

                    }
                }
            }


        }
    }
    if(direction=="diag1"){//diag en allant vers le haut quand on va vers la droite
        if(row-column==row_king-column_king){//check if both pieces on this diag
            if(row<row_king){
                for(int i=1;i<row_king-row;i++){
                    std::string position=convert_chessboard_position(column+i,row+i);
                    if(our_map[position]!="none" || enemy_map[position]!="none"){
                        return false;
                    }
                }
                for(int i=1;i<std::min(row,column);i++){
                    std::string position=convert_chessboard_position(column-i,row-i);
                    if(our_map[position]!="none"){
                        return false;
                    }
                    if(enemy_map[position]!="none"){
                         if(enemy_map[position]=="queen" || enemy_map[position]=="bishop"){
                            return true;
                        }
                        return false;
                    }
                }
            }
            if(row>row_king){
                for(int i=1;i<row-row_king;i++){
                    std::string position=convert_chessboard_position(column_king+i,row_king+i);
                    if(our_map[position]!="none" || enemy_map[position]!="none"){
                        return false;
                    }
                }
                for(int i=1;i<9-std::max(row,column);i++){
                    std::string position=convert_chessboard_position(column+i,row+i);
                    if(our_map[position]!="none"){
                        return false;
                    }
                    if(enemy_map[position]!="none"){
                        if(enemy_map[position]=="queen" || enemy_map[position]=="bishop"){
                            return true;
                        }
                        return false;
                    }
                }
            }
        }
    }
    if(direction=="diag2"){//diag going down when we go to right
        if(row+column==row_king+column_king){//check if both pieces on same diage
            if(row<row_king){
                for(int i=1;i<row_king-row;i++){
                    std::string position=convert_chessboard_position(column-i,row+i);
                    if(our_map[position]!="none" || enemy_map[position]!="none"){
                        return false;
                    }
                }
                for(int i=1; i< std::min(row,9-column);i++){
                    std::string position=convert_chessboard_position(column+i,row-i);
                    if(our_map[position]!="none"){
                        return false;
                    }
                    if(enemy_map[position]!="none"){
                        if(enemy_map[position]=="queen" || enemy_map[position]=="bishop"){
                            return true;
                        }
                        return false;
                    }
                }
            }
            if(row>row_king){
                for(int i=1;i<row-row_king;i++){
                    std::string position=convert_chessboard_position(column_king-i,column_king+i);
                    if(our_map[position]!="none" || enemy_map[position]!="none"){
                        return false;
                    }
                }
                for(int i=1; i< std::min(column,9-row);i++){
                    std::string position=convert_chessboard_position(column-i,row+i);
                    if(our_map[position]!="none"){
                        return false;
                    }
                    if(enemy_map[position]!="none"){
                        if(enemy_map[position]=="queen" || enemy_map[position]=="bishop"){
                            return true;
                        }
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

bool check_at_pos(std::vector<piece_t*>& board,bool am_white,int column,int row,std::map<std::string, std::string>& our_map, std::map<std::string, std::string>& enemy_map){//check at column and row if their is check at that position for our team or other team depending on the order of our map and enemey map
    

    int column_king;
    int row_king;
    for(auto pair:enemy_map){
      if(pair.second=="king"){
        std::string position=change_format(pair.first);
        column_king =  position[0] - '0';
        row_king = position[1] - '0';
      }
    }
    
    if((std::abs(column_king-column-row)<2)&&(std::abs(row_king)<2)){//check not opposing king in periphery
      return true;
    }
    
    std::vector<std::pair<int, int>> offsets = {
      {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
    for (auto offset : offsets){//check for knights
    
    int new_column = column + offset.first;
    int new_row = row + offset.second;
    if(1<=new_column && new_column<=8){
      if(1<=new_row && new_column<=8){
        std::string new_pos=convert_chessboard_position(new_column,new_row);
        if(enemy_map[new_pos]=="knight"){//if opposing knight in one of positions
          return true;
        }
      }
    }
    }
    
    
    
    for(int i=row+1;i<9;i++){ //check on columns and rows
      std::string pos=convert_chessboard_position(column,i);
      if(our_map[pos]!="none" && our_map[pos]!="king"){
        break;
      }
      if(enemy_map[pos]!="none"){
        if(enemy_map[pos]=="queen" || enemy_map[pos]=="rook"){
          return true;
        }
        break;
      }
    }
    for(int i=row-1;i>0;i--){
      std::string pos=convert_chessboard_position(column,i);
      if(our_map[pos]!="none" && our_map[pos]!="king"){
        break;
      }
      if(enemy_map[pos]!="none"){
        if(enemy_map[pos]=="queen" || enemy_map[pos]=="rook"){
          return true;
        }
        break;
      }
    }
    for(int i=column+1;i<9;i++){
      std::string pos=convert_chessboard_position(i,row);
      if(our_map[pos]!="none" && our_map[pos]!="king"){
        break;
      }
      if(enemy_map[pos]!="none"){
        if(enemy_map[pos]=="queen" || enemy_map[pos]=="rook"){
          return true;
        }
        break;
      }
    }
    
    
    for(int i=column-1;i>0;i--){
      std::string pos=convert_chessboard_position(i,row);
      if(our_map[pos]!="none" && our_map[pos]!="king"){
        break;
      }
      if(enemy_map[pos]!="none"){
        if(enemy_map[pos]=="queen" || enemy_map[pos]=="rook"){
          return true;
        }
        break;
      }
    }
    
    int i=row+1;
    int j=column+1;

    while(j<9 && i<9){
      std::string pos=convert_chessboard_position(j,i);
      if(our_map[pos]!="none" && our_map[pos]!="king"){
        break;
      }
      if(enemy_map[pos]!="none"){
        if(enemy_map[pos]=="queen" || enemy_map[pos]=="bishop"){
          return true;
        }
        break;
      }
      i++;
      j++;
    }
    
    i=row-1;
    j=column+1;
    while(0<j && j<9 && 0<i && i<9){
      std::string pos=convert_chessboard_position(j,i);
      if(our_map[pos]!="none" && our_map[pos]!="king"){
        break;
      }
      if(enemy_map[pos]!="none"){
        if(enemy_map[pos]=="queen" || enemy_map[pos]=="bishop"){
          return true;
        }
        break;
      }
      i--;
      j++;
    }
    i=row+1;
    j=column-1;
    while(0<j && j<9 && 0<i && i<9){
      std::string pos=convert_chessboard_position(j,i);
      if(our_map[pos]!="none" && our_map[pos]!="king"){
        break;
      }
      if(enemy_map[pos]!="none"){
        if(enemy_map[pos]=="queen" || enemy_map[pos]=="bishop"){
          return true;
        }
        break;
      }
      i++;
      j--;
    }
    
    i=row-1;
    j=column-1;
    while(0<j && j<9 && 0<i && i<9){
      std::string pos=convert_chessboard_position(j,i);
      if(our_map[pos]!="none" && our_map[pos]!="king"){
        break;
      }
      if(enemy_map[pos]!="none"){
        if(enemy_map[pos]=="queen" || enemy_map[pos]=="bishop"){
          return true;
        }
        break;
      }
      i--;
      j--;
    } 
    
    //check for pawns
    if(am_white){
      if(0<column-1 && 9>row+1){
      std::string bottom_left=convert_chessboard_position(column-1,row+1);
      if(enemy_map[bottom_left]=="pawn"){
        return true;
      }
      }
      if(9>column+1 && 9>row+1){
      std::string bottom_right=convert_chessboard_position(column+1,row+1);
      if(enemy_map[bottom_right]=="pawn"){
        return true;
      }
      }
    }
    if(!am_white){
      if(0<column-1 && 0<row-1){
      std::string bottom_left=convert_chessboard_position(column-1,row-1);
      if(enemy_map[bottom_left]=="pawn"){
        return true;
      }
      }
      if(9>column+1 && 0<row-1){
      std::string bottom_right=convert_chessboard_position(column+1,row-1);
      if(enemy_map[bottom_right]=="pawn"){
        return true;
      }
      }
    }
    

    
  
  return false;


}


