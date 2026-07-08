#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

class Board{
  
  public:
     //board(); //idk
     int board [20][10] = {};
     void lock_piece(Piece& piece); //to lock the piece within board (change from 0 to 1)
     bool is_line_full( int row); 
     int clear_full_lines();    
};


#endif