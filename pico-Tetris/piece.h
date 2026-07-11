#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

class Piece {
public:
    Piece();      // default piece
    Piece(int id);

    int piece_id;
    uint16_t color;
    int rotation = 0;
    int x = 3;
    int y = 0;
    int shape[4][4] = {};

    void setShapeFromId();
    void copyShape(int source[4][4]);
};


#endif
