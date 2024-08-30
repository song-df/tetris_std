#include "controller.h"
#include "file.h"
/* 
	•	Purpose: Initializes the game controller, including setting up the game board, current shape, and rendering system.
	•	Parameters:
	•	GameController* gc: Pointer to the GameController structure.
	•	Return Value: None.
*/
void initGameController(GameController* gc) {
    srand(time(NULL));
    bool loadGame = false;

    // 检查是否有保存文件
    if (checkSaveFile("savegame.txt")) {
        printf("Save file detected. Do you want to load the saved game? (y/n): ");
        char choice;
        scanf(" %c", &choice);  // 获取用户输入
        if (choice == 'y' || choice == 'Y') {
            loadGame = true;
        }
    }

    if (loadGame) {
        // 如果用户选择加载，加载保存的进度
        loadGameFromTXT(&gc->board, "savegame.txt");
        printf("Game loaded from save file.\n");
    } else {
        // 否则初始化一个新游戏
        resetBoard(&gc->board);
        gc->board.currentShape = shapes[rand() % 7];
        gc->board.nextShape = shapes[rand() % 7];
        gc->board.score = 0;
        printf("Starting a new game.\n");
    }

    gc->state = GAME_RUNNING;
    initRenderer(&gc->renderer);
    gc->running = true;
    gc->left = gc->right = gc->down = gc->rotate = false;
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
    while (gc->running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                gc->running = false;
            } else if (e.type == SDL_KEYDOWN) {
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
                    case SDLK_p:  // 按 'p' 键暂停或恢复游戏
                        if (gc->state == GAME_RUNNING) {
                            gc->state = GAME_PAUSED;
                        } else if (gc->state == GAME_PAUSED) {
                            gc->state = GAME_RUNNING;
                        }
                        break;
                    case SDLK_s:  // 按 's' 键保存进度（仅在暂停时有效）
                        if (gc->state == GAME_PAUSED) {
                            saveGameToTXT(&gc->board, "savegame.txt");
                            printf("Game saved.\n");
                        }
                        break;
                    case SDLK_r:  // 重启游戏（在暂停或游戏结束时）
                        if (gc->state == GAME_PAUSED || gc->state == GAME_OVER) {
                            resetBoard(&gc->board);
                            gc->board.currentShape = shapes[rand() % 7];
                            gc->board.nextShape = shapes[rand() % 7];
                            gc->board.score = 0;
                            gc->state = GAME_RUNNING;
                        }
                        break;
                    case SDLK_q:  // 按 'q' 键退出游戏
                        gc->running = false;
                        break;
                }
            }
        }

        if (gc->state == GAME_RUNNING) {
            updateGame(gc);
            if (checkCollision(&gc->board, gc->board.currentShape.x, gc->board.currentShape.y + 1, &gc->board.currentShape)) {
                placeShape(&gc->board);
                gc->board.currentShape = gc->board.nextShape;
                gc->board.nextShape = shapes[rand() % 7];
                if (checkCollision(&gc->board, gc->board.currentShape.x, gc->board.currentShape.y, &gc->board.currentShape)) {
                    printf("Game Over detected.\n");
                    gc->state = GAME_OVER;
                }
            } else {
                gc->board.currentShape.y++;  // 向下移动方块
            }
        }

        drawBoard(&gc->renderer, &gc->board, gc->state);

        SDL_Delay(200);  // 控制游戏速度
    }
}