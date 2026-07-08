#include "board.h"

 void Board::lock_piece(Piece& piece) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece.shape[i][j] == 1) {
                int piece_location_x = j + piece.x;
                int piece_location_y = i + piece.y;

                if (piece_location_y >= 0 && piece_location_y < 20 &&
                    piece_location_x >= 0 && piece_location_x < 10) {
                    board[piece_location_y][piece_location_x] = 1;
                }
            }
        }
    }
}


  
bool Board::is_line_full(int row) {
    for (int col = 0; col < 10; col++) {
        if (board[row][col] == 0) {
            return false;
        }
    }

    return true;
}

int Board::clear_full_lines() {
    int cleared = 0;

    for (int row = 19; row >= 0; row--) {
        if (is_line_full(row)) {
            cleared++;

            for (int moveRow = row; moveRow > 0; moveRow--) {
                for (int col = 0; col < 10; col++) {
                    board[moveRow][col] = board[moveRow - 1][col];
                }
            }

            for (int col = 0; col < 10; col++) {
                board[0][col] = 0;
            }

            row++;
        }
    }

    return cleared;
}