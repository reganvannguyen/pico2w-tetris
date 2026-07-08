#ifndef GAME_H
#define GAME_H

#include <queue>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "piece.h"
#include "board.h"

class Game {
public:
    std::queue<Piece> q_pieces;
    int fall_timing = 1;
    int score = 0;
    int level = 1;
    int line_cleared = 0;
    bool is_game_over = false;
    Piece current_piece;

    void set_fall_timing();
    bool can_move_down(const Piece& piece, int board[20][10]);
    bool can_move_left(const Piece& piece, int board[20][10]);
    bool can_move_right(const Piece& piece, int board[20][10]);
    bool can_rotate(const Piece& piece, int board[20][10]);

    void down(Piece& piece, int board[20][10]);
    void left(Piece& piece, int board[20][10]);
    void right(Piece& piece, int board[20][10]);
    void rotate(Piece& piece, int board[20][10]);

    void decide_when_to_lock_piece(Piece& piece, Board& board);

    void create_piece();
    void add_piece_board(int board[20][10]);
    void set_score(int scr);
    void set_level();
    bool game_over(const Piece& piece, int board[20][10]);
};

#endif