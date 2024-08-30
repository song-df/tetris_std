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
    gc->board.nextShape = shapes[rand() % 7];  // Generate the next shape
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
            //cancel check at here, and check exit updateGame
            // if(checkCollision(&gc->board, gc->board.currentShape.x, gc->board.currentShape.y, &gc->board.currentShape)) {
            //     gc->running = false; // Game over
            // }
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
    bool gameOver = false;

    while (gc->running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                gc->running = false;
            } else if (e.type == SDL_KEYDOWN) {
                if (!gameOver) {
                    switch (e.key.keysym.sym) {
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
                } else {
                    switch (e.key.keysym.sym) {
                        case SDLK_r:  // Restart the game
                            resetBoard(&gc->board);
                            gc->board.currentShape = shapes[rand() % 7];
                            gc->board.nextShape = shapes[rand() % 7];
                            gc->board.score = 0;
                            gameOver = false;
                            break;
                        case SDLK_q:  // Quit the game
                            gc->running = false;
                            break;
                    }
                }
            }
        }

        //printf("===gc->running:%d\n",gc->running);
        if (!gameOver) {
            updateGame(gc);
            if (checkCollision(&gc->board, gc->board.currentShape.x, gc->board.currentShape.y + 1, &gc->board.currentShape)) {
                placeShape(&gc->board);  // Place the shape on the board
                gc->board.currentShape = gc->board.nextShape;
                gc->board.nextShape = shapes[rand() % 7];
                if (checkCollision(&gc->board, gc->board.currentShape.x, gc->board.currentShape.y, &gc->board.currentShape)) {
                    printf("Game Over detected.\n");
                    gameOver = true;
                }
            } else {
                gc->board.currentShape.y++;  // Move the shape down
            }
        }

        drawBoard(&gc->renderer, &gc->board, gameOver);
        //printf("###gc->running:%d\n",gc->running);
        SDL_Delay(200);
    }
}