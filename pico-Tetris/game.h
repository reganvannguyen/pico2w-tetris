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

    static const unsigned long LOCK_DELAY_MS = 500;
    static const int MAX_LOCK_RESETS = 15;

    void set_fall_timing();
    bool can_move_down(const Piece& piece, int board[20][10]);
    bool can_move_left(const Piece& piece, int board[20][10]);
    bool can_move_right(const Piece& piece, int board[20][10]);
    bool can_rotate(const Piece& piece, int board[20][10]);

    void down(Piece& piece, int board[20][10]);
    bool left(Piece& piece, int board[20][10]);
    bool right(Piece& piece, int board[20][10]);
    bool rotate(Piece& piece, int board[20][10]);

    void register_lock_delay_movement(unsigned long now);
    void decide_when_to_lock_piece(Piece& piece, Board& board, unsigned long now);

    void create_piece();
    void add_piece_board(int board[20][10]);
    void set_score(int scr);
    void set_level();
    bool game_over(const Piece& piece, int board[20][10]);

private:
    bool lock_delay_active = false;
    unsigned long lock_delay_started_at = 0;
    int lock_reset_count = 0;
    void reset_lock_delay();
};

#endif
