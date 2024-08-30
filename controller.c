#include "controller.h"
/* 
	•	Purpose: Initializes the game controller, including setting up the game board, current shape, and rendering system.
	•	Parameters:
	•	GameController* gc: Pointer to the GameController structure.
	•	Return Value: None.
*/
void initGameController(GameController* gc) {
    srand(time(NULL));
    resetBoard(&gc->board);
    gc->board.currentShape = shapes[rand() % 7];
    gc->board.score = 0;
    initRenderer(&gc->renderer);
    gc->running = true;
    gc->left = gc->right = gc->down = gc->rotate = false;
}
/* 

	•	Purpose: Handles user input via the keyboard, setting flags for movement, rotation, or exiting the game.
	•	Parameters:
	•	GameController* gc: Pointer to the GameController structure.
	•	Return Value: None.
    */
void handleInput(GameController* gc) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            gc->running = false;
        }
        if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_LEFT:
                    gc->left = true;
                    break;
                case SDLK_RIGHT:
                    gc->right = true;
                    break;
                case SDLK_DOWN:
                    gc->down = true;
                    break;
                case SDLK_UP:
                    gc->rotate = true;
                    break;
                case SDLK_ESCAPE:
                    gc->running = false;
                    break;
            }
        }
    }
}
/**
 * 	•	Purpose: Updates the game state based on user input, including moving the current shape, checking for collisions, and placing the shape on the board if it can no longer move down.
	•	Parameters:
	•	GameController* gc: Pointer to the GameController structure.
	•	Return Value: None.
 *
 */
void updateGame(GameController* gc) {
    if(gc->left && !checkCollision(&gc->board, gc->board.currentShape.x - 1, gc->board.currentShape.y, &gc->board.currentShape)) {
        gc->board.currentShape.x--;
    }
    if(gc->right && !checkCollision(&gc->board, gc->board.currentShape.x + 1, gc->board.currentShape.y, &gc->board.currentShape)) {
        gc->board.currentShape.x++;
    }
    if(gc->down && !checkCollision(&gc->board, gc->board.currentShape.x, gc->board.currentShape.y + 1, &gc->board.currentShape)) {
        gc->board.currentShape.y++;
    }
    if(gc->rotate) {
        Shape rotated = rotateShape(gc->board.currentShape);
        if(!checkCollision(&gc->board, rotated.x, rotated.y, &rotated)) {
            gc->board.currentShape = rotated;
        }
    }

    if(!gc->down && !gc->right && !gc->left && !gc->rotate) {
        if(checkCollision(&gc->board, gc->board.currentShape.x, gc->board.currentShape.y + 1, &gc->board.currentShape)) {
            placeShape(&gc->board);
            clearLines(&gc->board);
            gc->board.currentShape = shapes[rand() % 7];
            if(checkCollision(&gc->board, gc->board.currentShape.x, gc->board.currentShape.y, &gc->board.currentShape)) {
                gc->running = false; // Game over
            }
        } else {
            gc->board.currentShape.y++;
        }
    }

    gc->left = gc->right = gc->down = gc->rotate = false;
}
/* 
	•	Purpose: Runs the main game loop, which continually handles input, updates the game state, and renders the board until the game is over.
	•	Parameters:
	•	GameController* gc: Pointer to the GameController structure.
	•	Return Value: None.
*/
void runGame(GameController* gc) {
    while(gc->running) {
        handleInput(gc);
        updateGame(gc);
        drawBoard(&gc->renderer, &gc->board);
        SDL_Delay(100);
    }
}