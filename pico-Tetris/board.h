#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

class Board{
  
  public:
     //board(); //idk
     int grid[20][10] = {};
     void clear();
     void lock_piece(Piece& piece); // store the piece color in the grid
     bool is_line_full( int row); 
     int clear_full_lines();    
};


#endif
