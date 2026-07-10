#ifndef UI_H
#define UI_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "board.h"
#include "piece.h"
#include "game.h"

class UI {
public:
    UI(Adafruit_ST7789& display);

    void drawBoard(Board& board);
    void drawPiece(Piece& piece);
    void drawGame(Board& board, Game& game);
    void drawCell(int row, int col, uint16_t fillColor, uint16_t outlineColor);
    void clearScreen();
    void drawStartScreen();
    void drawPaused(Board& board, Game& game);
    void drawGameOver(Game& game);

private:
    Adafruit_ST7789& tft;

    void drawCenteredText(const char* text, int y, uint8_t textSize, uint16_t color);

    int cellSize = 10;
    int boardStartX = 20;
    int boardStartY = 20;
};

#endif
