#ifndef MODEL_H
#define MODEL_H

#include <SDL.h>
#include <stdbool.h>

//#define WIDTH 300
//#define HEIGHT 600
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define TILE_SIZE 30

typedef enum {
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_OVER
} GameState;

typedef struct {
    SDL_Color color;
    bool active;
} Block;

typedef struct {
    SDL_Color color;
    bool matrix[4][4];
    int x, y;
    int size;
} Shape;

typedef struct {
    Block board[BOARD_WIDTH][BOARD_HEIGHT];
    Shape currentShape;
    Shape nextShape;
    int score;
    int gameLevel;
} GameBoard;

extern Shape shapes[7];

void resetBoard(GameBoard* board);
bool checkCollision(GameBoard* board, int newX, int newY, Shape* shape);
void placeShape(GameBoard* board);
void clearLines(GameBoard* board);
Shape rotateShape(Shape s);

#endif