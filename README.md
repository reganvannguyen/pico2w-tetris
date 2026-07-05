# Pico Tetris

A Tetris clone built for the Raspberry Pi Pico 2 W using an ST7789 TFT display and physical button/joystick input.

The goal of this project is to build a small embedded game while learning how to structure game logic, input handling, display rendering, and persistent high score storage on a microcontroller.

## Hardware

Planned hardware:

- Raspberry Pi Pico 2 W
- ST7789 TFT display
- Buttons or joystick for input
- Breadboard and jumper wires
- Optional: buzzer for sound effects

## Features

Planned features:

- 20x10 Tetris board
- 7 standard Tetris pieces
- Piece movement and rotation
- Collision detection
- Piece locking
- Line clearing
- Score tracking
- Level progression
- Increasing fall speed based on level
- Next-piece preview
- Game over detection
- Persistent high score storage
- UI rendering on the ST7789 display

## Controls

Planned controls:

- Left: move piece left
- Right: move piece right
- Down: soft drop
- Rotate: rotate piece
- Optional: hard drop
- Optional: pause

## Project Structure

The project is planned around a few main classes:

```text
main
 ├── Input
 ├── Game
 │    ├── Board
 │    ├── Piece currentPiece
 │    ├── pieceQueue
 │    └── HighScoreStorage
 └── UI / Renderer
```

### Game

Controls the main Tetris rules and game state.

Responsibilities:

- Track the current falling piece
- Track the next-piece queue
- Handle falling timing
- Handle whole-piece collision checks
- Decide when pieces lock
- Spawn new pieces
- Update score and level
- Update fall speed
- Detect game over

### Board

Stores the locked blocks in a 20x10 grid.

Responsibilities:

- Store the board array
- Check if cells are empty or filled
- Check if cells are inside the board
- Place locked pieces into the board
- Clear full lines
- Shift rows downward

### Piece

Represents the active falling piece.

Responsibilities:

- Store piece type
- Store rotation state
- Store x position
- Store y position

The 7 pieces and their rotations will be stored using 4x4 shape layouts.

### Input

Handles reading the physical controls.

Responsibilities:

- Read button or joystick pins
- Track which controls are pressed
- Keep hardware input code separate from game logic

### UI / Renderer

Handles drawing to the ST7789 display.

Responsibilities:

- Draw the board
- Draw the current falling piece
- Draw next-piece preview
- Draw score and level
- Draw high score
- Draw game over screen

### HighScoreStorage

Handles saving and loading the high score.

Responsibilities:

- Load the saved high score when the game starts
- Save a new high score when the game ends
- Keep storage details separate from game logic

## Game Design

The board stores only locked pieces. The currently falling piece is tracked separately.

```text
Board = settled blocks
Current piece = moving block
Screen = board + current piece
```

When a piece can no longer move down, the game locks it into the board, clears any full lines, updates the score, and spawns the next piece.

The level increases based on the total number of cleared lines. As the level increases, the fall interval decreases, making pieces fall faster.

## Learning Goals

This project is meant to practice:

- Embedded C++ programming
- Working with microcontroller input
- Drawing graphics on a TFT display
- Game loop design
- Class-based code organization
- State management
- Collision detection
- Timing without blocking delays
- Persistent storage on embedded hardware
- Building a complete small game from scratch
