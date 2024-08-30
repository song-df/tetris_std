#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include "view.h"
#include <stdbool.h>

/** 
	•	GameBoard board:
	•	Description: A GameBoard structure containing the current state of the game board and the current falling shape.
	•	Purpose: Manages the game’s data, including the grid of blocks and the active shape.
	•	Renderer renderer:
	•	Description: A Renderer structure responsible for rendering the game board and shapes.
	•	Purpose: Handles all rendering tasks, drawing the game state to the screen.
	•	bool running:
	•	Description: A boolean flag indicating whether the game loop is running.
	•	Purpose: Used to control the main game loop, allowing the game to continue or exit.
	•	bool left, right, down, rotate:
	•	Description: Boolean flags corresponding to the player’s input for moving left, right, down, or rotating the current shape.
	•	Purpose: Track the player’s input and apply the corresponding actions to the current shape during the game update.
 */

typedef struct {
    GameBoard board;
    Renderer renderer;
    bool running;
    bool left, right, down, rotate;
    GameState state;
} GameController;

void initGameController(GameController* gc);
void updateGame(GameController* gc);
void runGame(GameController* gc);

#endif