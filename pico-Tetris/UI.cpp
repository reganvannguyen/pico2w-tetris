#include "UI.h"
#include <algorithm>
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

void UI::drawPreview(const Piece& piece, int areaX, int areaY, int areaWidth, int areaHeight) {
    const int previewCellSize = 8;
    int minRow = 4;
    int maxRow = -1;
    int minCol = 4;
    int maxCol = -1;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (piece.shape[row][col] == 1) {
                minRow = std::min(minRow, row);
                maxRow = std::max(maxRow, row);
                minCol = std::min(minCol, col);
                maxCol = std::max(maxCol, col);
            }
        }
    }

    if (maxRow < 0 || maxCol < 0) {
        return;
    }

    int pieceWidth = (maxCol - minCol + 1) * previewCellSize;
    int pieceHeight = (maxRow - minRow + 1) * previewCellSize;
    int startX = areaX + (areaWidth - pieceWidth) / 2;
    int startY = areaY + (areaHeight - pieceHeight) / 2;

    for (int row = minRow; row <= maxRow; row++) {
        for (int col = minCol; col <= maxCol; col++) {
            if (piece.shape[row][col] == 1) {
                int x = startX + (col - minCol) * previewCellSize;
                int y = startY + (row - minRow) * previewCellSize;
                tft.fillRect(x, y, previewCellSize, previewCellSize, piece.color);
                tft.drawRect(x, y, previewCellSize, previewCellSize, piece.color);
            }
        }
    }
}

void UI::drawGame(Board& board, Game& game) {
    drawBoard(board);
    drawPiece(game.current_piece);

    tft.fillRect(0, 0, 70, 240, ST77XX_BLACK);
    tft.fillRect(170, 0, 70, 240, ST77XX_BLACK);

    tft.setCursor(180, 20);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("Score");
    tft.setCursor(180, 32);
    tft.println(game.score);

    tft.setCursor(180, 50);
    tft.print("Level");
    tft.setCursor(180, 62);
    tft.println(game.level);

    tft.setCursor(180, 80);
    tft.print("Lines");
    tft.setCursor(180, 92);
    tft.println(game.line_cleared);

    tft.setTextColor(ST77XX_ORANGE);
    tft.setCursor(15, 20);
    tft.print("HOLD");
    tft.drawRect(10, 38, 50, 50, ST77XX_WHITE);
    if (game.has_held_piece) {
        drawPreview(game.held_piece, 10, 38, 50, 50);
    }

    tft.setCursor(188, 115);
    tft.print("NEXT");
    tft.drawRect(180, 131, 50, 50, ST77XX_WHITE);
    const Piece* upcoming = game.next_piece();
    if (upcoming != 0) {
        drawPreview(*upcoming, 180, 131, 50, 50);
    }
}

void UI::drawStartScreen() {
    tft.fillScreen(ST77XX_BLACK);

    drawCenteredText("TETRIS", 70, 3, ST77XX_ORANGE);
    drawCenteredText("Press A to Start", 135, 2, ST77XX_ORANGE);
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
