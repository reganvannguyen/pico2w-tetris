#include "piece.h"
Piece::Piece() {
    piece_id = 0;   // default piece type
    x = 3;
    y = 0;
    setShapeFromId();
}

Piece::Piece(int id) {
    piece_id = id;
    x = 3;
    y = 0;
    setShapeFromId();
}


void Piece::setShapeFromId(){

  if(piece_id == 0){ // L shape
    int temp[4][4] = {{0,0,0,0},
                      {0,1,0,0},
                      {0,1,0,0},
                      {0,1,1,0}};
    copyShape(temp);
  }else if(piece_id == 1){ //reverse l shape
    int temp[4][4] = {{0,0,0,0},
                      {0,0,1,0},
                      {0,0,1,0},
                      {0,1,1,0}};
    copyShape(temp);
  }else if(piece_id == 2){ // z shape
    int temp[4][4] = {{0,0,0,0},
                      {1,1,0,0},
                      {0,1,1,0},
                      {0,0,0,0}};
    copyShape(temp);
  }else if(piece_id == 3){ // reverse shape
    int temp[4][4] = {{0,0,0,0},
                      {0,0,1,1},
                      {0,1,1,0},
                      {0,0,0,0}};
    copyShape(temp);
  }else if(piece_id == 4){ // t shape
    int temp[4][4] = {{0,0,0,0},
                      {0,1,1,1},
                      {0,0,1,0},
                      {0,0,0,0}};
    copyShape(temp);
  }else if(piece_id == 5){ // square shapre
    int temp[4][4] = {{0,0,0,0},
                      {0,1,1,0},
                      {0,1,1,0},
                      {0,0,0,0}};
    copyShape(temp);
  }else if(piece_id == 6){ // square shapre
    int temp[4][4] = {{0,1,0,0},
                      {0,1,0,0},
                      {0,1,0,0},
                      {0,1,0,0}};
    copyShape(temp);
  }

}


void Piece::copyShape(int source[4][4]) {
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      shape[row][col] = source[row][col];
    }
  }
}



