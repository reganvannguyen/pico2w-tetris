#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "piece.h"
#include "game.h"
#include "board.h"
#include "UI.h"
#include "highScoreStorage.h"

// Waveshare Pico-LCD-1.3 pin mapping
#define TFT_DC    8
#define TFT_CS    9
#define TFT_SCLK 10
#define TFT_MOSI 11
#define TFT_RST  12
#define TFT_BL   13

int joystick[] = {2, 20, 18, 16, 3};  // up, right, down, left, center
int keys[] = {15, 17, 19, 21};         // a, b, x, y

Adafruit_ST7789 tft = Adafruit_ST7789(&SPI1, TFT_CS, TFT_DC, TFT_RST);
UI ui(tft);
Board board;
Game game;
HighScoreStorage highScores;

enum GameState {
  START_SCREEN,
  PLAYING,
  PAUSED,
  NAME_ENTRY,
  GAME_OVER
};

GameState gameState = START_SCREEN;

unsigned long lastFallTime = 0;
unsigned long fallDelay = 500;
unsigned long lastInputTime = 0;
unsigned long inputDelay = 125;
unsigned long pauseStartedAt = 0;
unsigned long lastHardDropTime = 0;
const unsigned long hardDropDebounce = 250;

bool previousA = false;
bool previousB = false;
bool previousY = false;
bool previousUp = false;
bool previousRight = false;
bool previousDown = false;
bool previousLeft = false;

char initials[4] = {'A', 'A', 'A', '\0'};
int selectedInitial = 0;

void showGameOverOrNameEntry() {
  ui.invalidateGameFrame();
  if (highScores.qualifies(game.score)) {
    initials[0] = 'A';
    initials[1] = 'A';
    initials[2] = 'A';
    selectedInitial = 0;
    gameState = NAME_ENTRY;
    ui.drawNameEntry(initials, selectedInitial, game.score);
  } else {
    gameState = GAME_OVER;
    ui.drawGameOver(game);
  }
}

void startNewGame(unsigned long now) {
  board.clear();
  game.reset();
  game.add_piece_board(board.grid);
  lastFallTime = now;
  lastInputTime = now;
  lastHardDropTime = now;
  fallDelay = 500;
  gameState = PLAYING;
  ui.invalidateGameFrame();
}

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  SPI1.setSCK(TFT_SCLK);
  SPI1.setTX(TFT_MOSI);

  for (int i = 0; i < sizeof(joystick) / sizeof(joystick[0]); i++) {
    pinMode(joystick[i], INPUT_PULLUP);
  }

  for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
    pinMode(keys[i], INPUT_PULLUP);
  }

  delay(250);

  tft.init(240, 240);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  std::srand(millis());
  highScores.begin();
  ui.drawStartScreen(highScores);
}

void loop() {
  unsigned long now = millis();

  bool aDown = digitalRead(keys[0]) == LOW;
  bool bDown = digitalRead(keys[1]) == LOW;
  bool yDown = digitalRead(keys[3]) == LOW;
  bool upDown = digitalRead(joystick[0]) == LOW;
  bool rightDown = digitalRead(joystick[1]) == LOW;
  bool downDown = digitalRead(joystick[2]) == LOW;
  bool leftDown = digitalRead(joystick[3]) == LOW;
  bool aPressed = aDown && !previousA;
  bool bPressed = bDown && !previousB;
  bool yPressed = yDown && !previousY;
  bool upPressed = upDown && !previousUp;
  bool rightPressed = rightDown && !previousRight;
  bool downPressed = downDown && !previousDown;
  bool leftPressed = leftDown && !previousLeft;

  previousA = aDown;
  previousB = bDown;
  previousY = yDown;
  previousUp = upDown;
  previousRight = rightDown;
  previousDown = downDown;
  previousLeft = leftDown;

  if (gameState == START_SCREEN) {
    if (aPressed) {
      startNewGame(now);
    }
    return;
  }

  if (gameState == GAME_OVER) {
    if (aPressed) {
      startNewGame(now);
    } else if (bPressed) {
      gameState = START_SCREEN;
      ui.invalidateGameFrame();
      ui.drawStartScreen(highScores);
    }
    return;
  }

  if (gameState == NAME_ENTRY) {
    bool changed = false;
    if (upPressed) {
      initials[selectedInitial] = initials[selectedInitial] == 'Z'
          ? 'A' : initials[selectedInitial] + 1;
      changed = true;
    } else if (downPressed) {
      initials[selectedInitial] = initials[selectedInitial] == 'A'
          ? 'Z' : initials[selectedInitial] - 1;
      changed = true;
    } else if (leftPressed) {
      selectedInitial = (selectedInitial + 2) % 3;
      changed = true;
    } else if (rightPressed) {
      selectedInitial = (selectedInitial + 1) % 3;
      changed = true;
    } else if (aPressed) {
      highScores.insert(initials, game.score);
      highScores.save();
      gameState = GAME_OVER;
      ui.drawGameOver(game);
      return;
    }

    if (changed) {
      ui.drawNameEntry(initials, selectedInitial, game.score);
    }
    return;
  }

  if (gameState == PAUSED) {
    if (yPressed) {
      unsigned long pausedDuration = now - pauseStartedAt;
      game.offset_lock_delay(pausedDuration);
      lastFallTime += pausedDuration;
      lastInputTime = now;
      gameState = PLAYING;
      ui.invalidateGameFrame();
      ui.drawGame(board, game);
    }
    return;
  }

  if (yPressed) {
    pauseStartedAt = now;
    gameState = PAUSED;
    ui.drawPaused(board, game);
    return;
  }

  if (upPressed && now - lastHardDropTime >= hardDropDebounce) {
    game.hard_drop(board);
    lastHardDropTime = now;
    lastFallTime = now;
    lastInputTime = now;

    if (game.is_game_over) {
      showGameOverOrNameEntry();
    } else {
      ui.drawGame(board, game);
    }
    return;
  }

  if (bPressed) {
    if (game.hold_current_piece(board)) {
      lastFallTime = now;
      lastInputTime = now;
    }

    if (game.is_game_over) {
      showGameOverOrNameEntry();
    } else {
      ui.drawGame(board, game);
    }
    return;
  }

  if (now - lastFallTime >= fallDelay) {
    game.down(game.current_piece, board.grid);
    lastFallTime = now;
  }

  if (now - lastInputTime >= inputDelay) {
    if (rightDown) {
      if (game.right(game.current_piece, board.grid)) {
        game.register_lock_delay_movement(now);
      }
      lastInputTime = now;
    } else if (downDown) {
      game.down(game.current_piece, board.grid);
      lastInputTime = now;
    } else if (leftDown) {
      if (game.left(game.current_piece, board.grid)) {
        game.register_lock_delay_movement(now);
      }
      lastInputTime = now;
    } else if (aPressed) {
      if (game.rotate(game.current_piece, board.grid)) {
        game.register_lock_delay_movement(now);
      }
      lastInputTime = now;
    }
  }

  game.decide_when_to_lock_piece(game.current_piece, board, now);

  if (game.is_game_over) {
    showGameOverOrNameEntry();
    return;
  }

  fallDelay = 500UL - ((game.level - 1) * 75UL);
  ui.drawGame(board, game);
}
