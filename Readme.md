# Connect-N Game

This project is a Connect-N game implemented in C++. The game allows two players to play a Connect-N game on a customizable board. The game includes features for saving and loading game states, checking for win conditions, and displaying the game board.

## Features

1. **Customizable Game Board**: Set the number of rows, columns, and the connectivity limit to win.
2. **Player Setup**: Allows setting player names and marks.
3. **Game Play**: Players take turns to drop their marks into the columns.
4. **Win Conditions**: Checks for vertical, horizontal, diagonal, and anti-diagonal wins.
5. **Save and Load Game**: Save the current game state to a file and load a previously saved game.

## Usage

### Compilation

To compile the program, use a C++ compiler like g++:

```sh
g++ -o connect_n connect_n.cpp
