#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "piece.h"
#include "game.h"
#include "board.h"
#include "UI.h"


// Waveshare Pico-LCD-1.3 pin mapping
#define TFT_DC    8   // GP8  LCD_DC  indicates whether Data or commands writes 
#define TFT_CS    9   // GP9  LCD_CS  chip select
#define TFT_SCLK 10   // GP10 LCD_CLK SPI clock input
#define TFT_MOSI 11   // GP11 LCD_DIN SPI Data input 
#define TFT_RST  12   // GP12 LCD_RST Reset, low active
#define TFT_BL   13   // GP13 LCD_BL  Backlight

int joystick[] = {2, 20, 18, 16, 3};  //up, right, down, left, center


int keys[] = {15,17, 19, 21}; //a, b, x, y


// Use software SPI so we can explicitly choose the pins.
// Constructor order: CS, DC, MOSI, SCLK, RST
// Adafruit_ST7789 tft = Adafruit_ST7789(
//   TFT_CS,
//   TFT_DC,
//   TFT_MOSI,
//   TFT_SCLK,
//   TFT_RST
// );

Adafruit_ST7789 tft = Adafruit_ST7789(&SPI1, TFT_CS, TFT_DC, TFT_RST); //uses hardware SPI now which is faster
UI ui(tft);

Board board; //craetion of board
Game game;

unsigned long lastFallTime = 0;
unsigned long fallDelay = 800;

unsigned long lastInputTime = 0;
unsigned long inputDelay = 125;




void setup() {

  // Turn on backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  //telling the adafruit_ST7789 to use these pins as the SCLK and MOSI 
  SPI1.setSCK(TFT_SCLK);
  SPI1.setTX(TFT_MOSI);
  
  //initalize joystick
  for(int i =0; i < sizeof(joystick)/sizeof(joystick[0]); i++){
    pinMode(joystick[i], INPUT_PULLUP);
  }

  //initlaize keys
  for(int i =0; i < sizeof(keys)/sizeof(keys[0]); i++){
    pinMode(keys[i], INPUT_PULLUP);
  }


  delay(250);

  // Initialize ST7789 display at 240x240
  tft.init(240, 240);
  // Try different rotations if the screen is sideways
  tft.setRotation(3);
  //screen colors
  tft.fillScreen(ST77XX_BLACK);
 
//----------------------------------------------------------------------------------------------------------
  //serial.begin(9600);
  std::srand(millis());

  game.add_piece_board(board.board);




}

void loop() {

  if (game.is_game_over) {
    ui.drawGameOver(game);
    return;
  }
  
  if (millis() - lastFallTime >= fallDelay) {
      game.down(game.current_piece, board.board); 
      lastFallTime = millis();
  }
    
  if (millis() - lastInputTime >= inputDelay) {
    if(digitalRead(joystick[4]) == LOW ){ // center
      game.rotate(game.current_piece, board.board);
      lastInputTime = millis();
    }
    else if(digitalRead(joystick[0]) == LOW ){ // up
      game.down(game.current_piece, board.board);
      lastInputTime = millis();
    }
    else if(digitalRead(joystick[1]) == LOW ){ // right
      game.right(game.current_piece, board.board);
      lastInputTime = millis();
    }
    else if(digitalRead(joystick[2]) == LOW ){ // down
      game.down(game.current_piece, board.board);
      lastInputTime = millis();
    }
    else if(digitalRead(joystick[3]) == LOW ){ // left
      game.left(game.current_piece, board.board);
      lastInputTime = millis();
    }
    else if(digitalRead(keys[0]) == LOW){ // a rotate
      game.rotate(game.current_piece, board.board);
      lastInputTime = millis();
    }
  }

  game.decide_when_to_lock_piece(game.current_piece, board);

  fallDelay =  500UL - ((game.level - 1) * 75UL);

  ui.drawGame(board, game);
}


