#include "piece.h"

namespace {
const uint16_t COLOR_BLUE = 0x001F;
const uint16_t COLOR_GREEN = 0x07E0;
const uint16_t COLOR_CYAN = 0x07FF;
const uint16_t COLOR_RED = 0xF800;
const uint16_t COLOR_MAGENTA = 0xF81F;
const uint16_t COLOR_ORANGE = 0xFD20;
const uint16_t COLOR_YELLOW = 0xFFE0;
}

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
    color = COLOR_ORANGE;
    int temp[4][4] = {{0,0,0,0},
                      {0,1,0,0},
                      {0,1,0,0},
                      {0,1,1,0}};
    copyShape(temp);
  }else if(piece_id == 1){ //reverse l shape
    color = COLOR_BLUE;
    int temp[4][4] = {{0,0,0,0},
                      {0,0,1,0},
                      {0,0,1,0},
                      {0,1,1,0}};
    copyShape(temp);
  }else if(piece_id == 2){ // z shape
    color = COLOR_RED;
    int temp[4][4] = {{0,0,0,0},
                      {1,1,0,0},
                      {0,1,1,0},
                      {0,0,0,0}};
    copyShape(temp);
  }else if(piece_id == 3){ // reverse shape
    color = COLOR_GREEN;
    int temp[4][4] = {{0,0,0,0},
                      {0,0,1,1},
                      {0,1,1,0},
                      {0,0,0,0}};
    copyShape(temp);
  }else if(piece_id == 4){ // t shape
    color = COLOR_MAGENTA;
    int temp[4][4] = {{0,0,0,0},
                      {0,1,1,1},
                      {0,0,1,0},
                      {0,0,0,0}};
    copyShape(temp);
  }else if(piece_id == 5){ // square shapre
    color = COLOR_YELLOW;
    int temp[4][4] = {{0,0,0,0},
                      {0,1,1,0},
                      {0,1,1,0},
                      {0,0,0,0}};
    copyShape(temp);
  }else if(piece_id == 6){ // square shapre
    color = COLOR_CYAN;
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



