# Pico Tetris

A Tetris clone for the Raspberry Pi Pico 2 W with a 240×240 ST7789 display and physical joystick/button controls.

## Hardware

- Raspberry Pi Pico 2 W
- 240×240 ST7789 TFT display
- Five-direction joystick
- A, B, X, and Y buttons

The display uses SPI1 with SCLK on GP10, MOSI on GP11, CS on GP9, DC on GP8, reset on GP12, and backlight on GP13.

## Features

- 20×10 board and all seven Tetrominoes in standard colors
- Movement, rotation, soft drop, hard drop, hold, and next-piece preview
- Collision detection, 500 ms lock delay, line clearing, scoring, and level progression
- Start, pause, and interactive game-over screens
- Persistent top-three leaderboard with three-letter initials in emulated EEPROM
- Cell-level dirty rendering and cached HUD updates to reduce display flicker

## Controls

| Input | Action |
| --- | --- |
| Joystick left/right | Move piece |
| Joystick down | Soft drop |
| Joystick up | Hard drop |
| Joystick center | No action |
| A | Rotate / confirm initials / start or retry |
| B | Hold piece / return to menu after game over |
| Y | Pause or resume |

During initials entry, joystick up/down changes the selected letter and left/right selects a position.

## Project Structure

```text
pico-Tetris/
├── pico-Tetris.ino       Application states, timing, and hardware input
├── game.*                Tetris rules and current game state
├── board.*               Locked-cell grid and line clearing
├── piece.*               Tetromino shape, position, and color
├── UI.*                  ST7789 screens and optimized rendering
└── highScoreStorage.*    Top-three EEPROM leaderboard
```

The board stores RGB565 colors for locked cells. The active piece remains separate until locking, and the renderer combines both into a cached 20×10 frame. Only cells and HUD panels whose values change are sent to the display.

High scores use the Arduino-Pico emulated EEPROM API. Stored data includes a magic value, format version, entry count, and checksum. EEPROM is written only after a player confirms a qualifying score.

## Building

Install the Earle Philhower Arduino-Pico core and the Adafruit GFX and ST7789 libraries, then select **Raspberry Pi Pico 2W** and compile `pico-Tetris/pico-Tetris.ino`.

With Arduino CLI:

```sh
arduino-cli compile --fqbn rp2040:rp2040:rpipico2w pico-Tetris
```

## Learning Goals

This project demonstrates embedded C++, non-blocking timing, physical input handling, game-state design, TFT rendering, flash-backed persistence, and separation of game rules from hardware-specific code.
