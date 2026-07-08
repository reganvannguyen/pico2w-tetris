#ifndef PIECE_H
#define PIECE_H

class Piece {
public:
    Piece();      // default piece
    Piece(int id);

    int piece_id;
    int rotation = 0;
    int x = 3;
    int y = 0;
    int shape[4][4] = {};

    void setShapeFromId();
    void copyShape(int source[4][4]);
};


#endif