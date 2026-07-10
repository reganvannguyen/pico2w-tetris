#include "UI.h"
#include <stdio.h>

UI::UI(Adafruit_ST7789& display) : tft(display) {
}

void UI::clearScreen() {
    tft.fillScreen(ST77XX_BLACK);
}

void UI::drawCenteredText(const char* text, int y, uint8_t textSize, uint16_t color) {
    int16_t x1;
    int16_t y1;
    uint16_t width;
    uint16_t height;

    tft.setTextSize(textSize);
    tft.setTextColor(color);
    tft.getTextBounds(text, 0, y, &x1, &y1, &width, &height);
    tft.setCursor((240 - width) / 2, y);
    tft.print(text);
}

void UI::drawCell(int row, int col, uint16_t fillColor, uint16_t outlineColor) {
    int x = boardStartX + col * cellSize;
    int y = boardStartY + row * cellSize;

    tft.fillRect(x, y, cellSize, cellSize, fillColor);
    tft.drawRect(x, y, cellSize, cellSize, outlineColor);
}

void UI::drawBoard(Board& board) {
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 10; col++) {
            if (board.grid[row][col] != 0) {
                uint16_t pieceColor = static_cast<uint16_t>(board.grid[row][col]);
                drawCell(row, col, pieceColor, pieceColor);
            } else {
                drawCell(row, col, ST77XX_BLACK, ST77XX_WHITE);
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
                    drawCell(boardRow, boardCol, piece.color, piece.color);
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

void UI::drawStartScreen() {
    tft.fillScreen(ST77XX_BLACK);

    tft.setTextColor(ST77XX_ORANGE);
    tft.setTextSize(3);
    tft.setCursor(35, 70);
    tft.println("TETRIS");

    tft.setTextSize(2);
    tft.setCursor(25, 135);
    tft.println("Press A to Start");
}

void UI::drawPaused(Board& board, Game& game) {
    drawGame(board, game);

    tft.fillRect(0, 80, 240, 80, ST77XX_BLACK);
    tft.setTextColor(ST77XX_ORANGE);
    tft.setTextSize(3);
    tft.setCursor(55, 90);
    tft.println("PAUSED");

    tft.setTextSize(2);
    tft.setCursor(25, 130);
    tft.println("Press Y to Resume");
}


void UI::drawGameOver(Game& game) {
    tft.fillScreen(ST77XX_BLACK);

    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", game.score);

    drawCenteredText("GAME OVER", 55, 3, ST77XX_RED);
    drawCenteredText(scoreText, 105, 2, ST77XX_WHITE);
    drawCenteredText("A: RETRY", 155, 2, ST77XX_ORANGE);
    drawCenteredText("B: MENU", 185, 2, ST77XX_ORANGE);

}
