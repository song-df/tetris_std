#include "view.h"

void initRenderer(Renderer* r) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL\n");
        exit(1);
    }
    if(TTF_Init() < 0) {
        printf("Failed to initialize SDL_ttf\n");
        exit(1);
    }
    r->window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    r->renderer = SDL_CreateRenderer(r->window, -1, 0);
    r->rect.w = r->rect.h = TILE_SIZE;

    r->font = TTF_OpenFont("font/Inconsolata-Regular.ttf", 40);
    if (!r->font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }
}

void cleanupRenderer(Renderer* r) {
    TTF_CloseFont(r->font);  // Close the font
    TTF_Quit();  // Quit SDL_ttf
    SDL_DestroyRenderer(r->renderer);
    SDL_DestroyWindow(r->window);
    SDL_Quit();
}

void drawBlock(Renderer* r, int x, int y, SDL_Color color) {
    r->rect.x = x * TILE_SIZE;
    r->rect.y = y * TILE_SIZE;
    SDL_SetRenderDrawColor(r->renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(r->renderer, &r->rect);
    SDL_SetRenderDrawColor(r->renderer, 255, 255, 255, 255);//画小方块边线为白色
    SDL_RenderDrawRect(r->renderer, &r->rect);
}

void drawBoard(Renderer* r, GameBoard* board) {
    SDL_SetRenderDrawColor(r->renderer, 0, 0, 0, 255);
    SDL_RenderClear(r->renderer);

    // Draw blocks
    for(int i = 0; i < BOARD_WIDTH; i++) {
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            if(board->board[i][j].active) {
                drawBlock(r, i, j, board->board[i][j].color);
            }
        }
    }

    // Draw current shape
    for(int i = 0; i < board->currentShape.size; i++) {
        for(int j = 0; j < board->currentShape.size; j++) {
            if(board->currentShape.matrix[i][j]) {
                drawBlock(r, board->currentShape.x + i, board->currentShape.y + j, board->currentShape.color);
            }
        }
    }

    // Render the score
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", board->score);
    renderText(r, scoreText, 10, 10);  // Render the score at the top-left corner

    SDL_RenderPresent(r->renderer);
}

void renderText(Renderer* r, const char* text, int x, int y) {
    SDL_Color color = {255, 255, 255};  // White color for text
    SDL_Surface* surface = TTF_RenderText_Solid(r->font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(r->renderer, surface);
    
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(r->renderer, texture, NULL, &dstRect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
