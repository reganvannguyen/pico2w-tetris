#include "UI.h"

UI::UI(Adafruit_ST7789& display) : tft(display) {
}

void UI::clearScreen() {
    tft.fillScreen(ST77XX_BLACK);
}

void UI::drawCell(int row, int col, uint16_t color) {
    int x = boardStartX + col * cellSize;
    int y = boardStartY + row * cellSize;

    tft.fillRect(x, y, cellSize, cellSize, color);
    tft.drawRect(x, y, cellSize, cellSize, ST77XX_WHITE);
}

void UI::drawBoard(Board& board) {
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 10; col++) {
            if (board.grid[row][col] != 0) {
                drawCell(row, col, static_cast<uint16_t>(board.grid[row][col]));
            } else {
                drawCell(row, col, ST77XX_BLACK);
            }
        }
    }
}

void UI::drawPiece(Piece& piece) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece.shape[i][j] == 1) {
                int boardCol = piece.x + j;
                int boardRow = piece.y + i;

                if (boardRow >= 0 && boardRow < 20 &&
                    boardCol >= 0 && boardCol < 10) {
                    drawCell(boardRow, boardCol, piece.color);
                }
            }
        }
    }
}

void UI::drawGame(Board& board, Game& game) {
    drawBoard(board);
    drawPiece(game.current_piece);

    tft.fillRect(140, 20, 90, 60, ST77XX_BLACK);

    tft.setCursor(140, 20);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("Score: ");
    tft.println(game.score);

    tft.setCursor(140, 40);
    tft.print("Level: ");
    tft.println(game.level);

    tft.setCursor(140, 60);
    tft.print("Lines: ");
    tft.println(game.line_cleared);
}


void UI::drawGameOver(Game& game) {
    tft.fillScreen(ST77XX_BLACK);

    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(2);
    tft.setCursor(55, 90);
    tft.println("GAME");

    tft.setCursor(55, 115);
    tft.println("OVER");

    tft.setCursor(55, 140);
    tft.print("Score: ");
    tft.println(game.score);

    tft.setCursor(55, 165);
    tft.print("level: ");
    tft.println(game.level);
     

}
