#include "model.h"

//SDL_Color COLORS[7] = {{255,165,0},{255,0,0},{224,165,255},{0,0,255},{255,255,0},{0,255,0},{128,0,128}};

Shape shapes[7] = {
    {{255,165,0}, {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, 5, 0, 3}, //L
    {{255,0,0},   {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, 5, 0, 3}, //o
    {{224,165,255}, {{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, 5, 0, 4},//I
    {{0,0,255},   {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, 5, 0, 3},//
    {{255,255,0}, {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, 5, 0, 2},
    {{0,255,0},   {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, 5, 0, 3},
    {{128,0,128}, {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, 5, 0, 3}
};
void resetBoard(GameBoard* board) {
    for(int i = 0; i < BOARD_WIDTH; i++) {
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            board->board[i][j] = (Block){ {0, 0, 0}, false };
        }
    }
}

bool checkCollision(GameBoard* board, int newX, int newY, Shape* shape) {
    // Iterate through each cell in the shape's matrix
    for(int i = 0; i < shape->size; i++) {
        for(int j = 0; j < shape->size; j++) {
            // Check if the current cell of the shape is filled
            if(shape->matrix[i][j]) {
                //printf("new[%d,%d],[%d,%d]\n",newX,newY,i,j);
                // Calculate the corresponding x and y coordinates on the game board
                int x = newX + i;
                int y = newY + j;

                // Check if the shape's cell would be outside the game board horizontally or vertically
                if(x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT) {
                    return true; // Collision detected (out of bounds)
                }

                // Check if the shape's cell overlaps with an already active block on the board
                if(y >= 0 && board->board[x][y].active) {
                    return true; // Collision detected (overlapping active block)
                }
            }
        }
    }
    // No collision detected
    return false;
}

void placeShape(GameBoard *board) {
    // Place the current shape's blocks into the game board
    for (int i = 0; i < board->currentShape.size; i++) {
        for (int j = 0; j < board->currentShape.size; j++) {
            if (board->currentShape.matrix[i][j]) {
                int x = board->currentShape.x + i;
                int y = board->currentShape.y + j;
                if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                    board->board[x][y].active = true;
                    board->board[x][y].color = board->currentShape.color;
                }
            }
        }
    }

    // After placing the shape, check for any full lines
    clearLines(board);
}

void clearLines(GameBoard* board) {
    int linesCleared = 0;  // Count how many lines are cleared

    for(int y = 0; y < BOARD_HEIGHT; y++) {
        bool full = true;
        for(int x = 0; x < BOARD_WIDTH; x++) {
            if(!board->board[x][y].active) {
                full = false;
                break;
            }
        }

        if(full) {
            linesCleared++;  // Increment the count for each cleared line

            // Shift all rows above down
            for(int i = y; i > 0; i--) {
                for(int x = 0; x < BOARD_WIDTH; x++) {
                    board->board[x][i] = board->board[x][i-1];
                }
            }

            // Clear the top row
            for(int x = 0; x < BOARD_WIDTH; x++) {
                board->board[x][0] = (Block){ {0, 0, 0}, false };
            }
        }
    }

    // Update the score based on the number of lines cleared
    switch(linesCleared) {
        case 1: board->score += 100; break;
        case 2: board->score += 300; break;
        case 3: board->score += 500; break;
        case 4: board->score += 800; break;
    }
    //printf("Score:%d\n",board->score);
}

Shape rotateShape(Shape s) {
    Shape temp = s;
    // Transpose the matrix
    for(int i = 0; i < s.size; i++) {
        for(int j = 0; j < s.size; j++) {
            temp.matrix[i][j] = s.matrix[j][i];
        }
    }
    // Reverse each row
    for(int i = 0; i < s.size; i++) {
        for(int j = 0; j < s.size / 2; j++) {
            bool t = temp.matrix[i][j];
            temp.matrix[i][j] = temp.matrix[i][s.size - j - 1];
            temp.matrix[i][s.size - j - 1] = t;
        }
    }
    return temp;
}

Shape rotateShape_(Shape s) {
    Shape temp = s;
    for(int i = 0; i < s.size; i++) {
        for(int j = s.size-1; j >= 0; j--) {
            //shape_matrix[i][3-j] = shapetemp[j][i];
            temp.matrix[i][j] = temp.matrix[3-j][i];
        }
    }
    return temp;
}