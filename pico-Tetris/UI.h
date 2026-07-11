#ifndef UI_H
#define UI_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "board.h"
#include "piece.h"
#include "game.h"
#include "highScoreStorage.h"

class UI {
public:
    UI(Adafruit_ST7789& display);

    void drawBoard(Board& board);
    void drawPiece(Piece& piece);
    void drawGame(Board& board, Game& game);
    void invalidateGameFrame();
    void drawCell(int row, int col, uint16_t fillColor, uint16_t outlineColor);
    void clearScreen();
    void drawStartScreen(const HighScoreStorage& highScores);
    void drawNameEntry(const char initials[4], int selectedIndex, int score);
    void drawPaused(Board& board, Game& game);
    void drawGameOver(Game& game);

private:
    Adafruit_ST7789& tft;

    void drawCenteredText(const char* text, int y, uint8_t textSize, uint16_t color);
    void drawPreview(const Piece& piece, int areaX, int areaY, int areaWidth, int areaHeight);
    void drawStaticHud();

    bool frameValid = false;
    uint16_t previousFrame[20][10] = {};
    int cachedScore = -1;
    int cachedLevel = -1;
    int cachedLines = -1;
    int cachedHeldPieceId = -2;
    int cachedNextPieceId = -2;

    int cellSize = 10;
    int boardStartX = 70;
    int boardStartY = 20;
};

#endif
