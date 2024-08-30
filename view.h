#ifndef VIEW_H
#define VIEW_H

#include "model.h"
#include <SDL.h>
#include <SDL_ttf.h>

// Define the window dimensions
#define WIDTH 600
#define HEIGHT 600


typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Rect rect;
    TTF_Font* font;  // Font for rendering text
    SDL_Texture* background;  // Texture for the background image
} Renderer;

void initRenderer(Renderer* r);
void cleanupRenderer(Renderer* r);
void drawBlock(Renderer* r, int x, int y, SDL_Color color);
void drawBoard(Renderer* r, GameBoard* board, bool gameOver);
void renderText(Renderer* r, const char* text, int x, int y);
#endif