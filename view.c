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
        

    // Load the background BMP image
    SDL_Surface* bgSurface = SDL_LoadBMP("background.bmp");
    if (!bgSurface) {
        printf("Failed to load background image: %s\n", SDL_GetError());
        exit(1);
    }
    
    r->background = SDL_CreateTextureFromSurface(r->renderer, bgSurface);
    SDL_FreeSurface(bgSurface);  // Free the surface after creating the texture
    if (!r->background) {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        exit(1);
    }
}

void cleanupRenderer(Renderer* r) {
    if (r->background) {
        SDL_DestroyTexture(r->background);
        r->background = NULL;
    }
    if (r->font) {
        TTF_CloseFont(r->font);
        r->font = NULL;
    }
    if (r->renderer) {
        SDL_DestroyRenderer(r->renderer);
        r->renderer = NULL;
    }
    if (r->window) {
        SDL_DestroyWindow(r->window);
        r->window = NULL;
    }
    TTF_Quit();
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

void renderGameOver(Renderer* r) {
    const char *message = "GAME OVER";
    const char *restartMessage = "Press R to Restart";
    const char *exitMessage = "Press Q to Quit";

    SDL_Color color = {255, 0, 0};  // Red color for "Game Over" text
    SDL_Color whiteColor = {255, 255, 255};  // White color for options text

    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect dstRect;

    // Render the "Game Over" message
    surface = TTF_RenderText_Solid(r->font, message, color);
    if (!surface) {
        printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(r->renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    // Get the size of the text
    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);

    // Center the text on the screen
    dstRect.x = (WIDTH - textWidth) / 2;
    dstRect.y = (HEIGHT - textHeight) / 3;  // Display it towards the top third
    dstRect.w = textWidth;
    dstRect.h = textHeight;

    // Render the "Game Over" text
    SDL_RenderCopy(r->renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);

    // Render the restart message
    surface = TTF_RenderText_Solid(r->font, restartMessage, whiteColor);
    if (!surface) {
        printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(r->renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    textWidth = surface->w;
    textHeight = surface->h;
    SDL_FreeSurface(surface);
    dstRect.x = (WIDTH - textWidth) / 2;
    dstRect.y = (HEIGHT - textHeight) / 2;  // Display it in the middle
    dstRect.w = textWidth;
    dstRect.h = textHeight;
    SDL_RenderCopy(r->renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);

    // Render the quit message
    surface = TTF_RenderText_Solid(r->font, exitMessage, whiteColor);
    if (!surface) {
        printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(r->renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    textWidth = surface->w;
    textHeight = surface->h;
    SDL_FreeSurface(surface);
    dstRect.x = (WIDTH - textWidth) / 2;
    dstRect.y = (HEIGHT + textHeight) / 2;  // Display it slightly below middle
    dstRect.w = textWidth;
    dstRect.h = textHeight;
    SDL_RenderCopy(r->renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

void drawBoard(Renderer* r, GameBoard* board, GameState state) {
    // 清空渲染器并绘制背景
    SDL_RenderClear(r->renderer);
    SDL_RenderCopy(r->renderer, r->background, NULL, NULL);

    // 绘制网格
    SDL_SetRenderDrawColor(r->renderer, 169, 169, 169, 255);  // 灰色
    for (int i = 0; i <= BOARD_WIDTH; i++) {
        int x = i * TILE_SIZE;
        SDL_RenderDrawLine(r->renderer, x, 0, x, BOARD_HEIGHT * TILE_SIZE);
    }
    for (int j = 0; j <= BOARD_HEIGHT; j++) {
        int y = j * TILE_SIZE;
        SDL_RenderDrawLine(r->renderer, 0, y, BOARD_WIDTH * TILE_SIZE, y);
    }

    // 绘制游戏板上的方块
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (board->board[i][j].active) {
                drawBlock(r, i, j, board->board[i][j].color);
            }
        }
    }

    // 绘制当前形状
    if (state == GAME_RUNNING) {
        for (int i = 0; i < board->currentShape.size; i++) {
            for (int j = 0; j < board->currentShape.size; j++) {
                if (board->currentShape.matrix[i][j]) {
                    drawBlock(r, board->currentShape.x + i, board->currentShape.y + j, board->currentShape.color);
                }
            }
        }
    }

    // 绘制下一个形状
    for (int i = 0; i < board->nextShape.size; i++) {
        for (int j = 0; j < board->nextShape.size; j++) {
            if (board->nextShape.matrix[i][j]) {
                drawBlock(r, BOARD_WIDTH + 2 + i, 2 + j, board->nextShape.color);
            }
        }
    }

    // 在下一个形状的下方显示分数
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", board->score);
    renderText(r, scoreText, (BOARD_WIDTH + 2) * TILE_SIZE, (2 + board->nextShape.size + 1) * TILE_SIZE);

    // 在分数下方显示操作提示
    renderText(r, "Press P to Pause", (BOARD_WIDTH + 2) * TILE_SIZE, (2 + board->nextShape.size + 3) * TILE_SIZE);
    renderText(r, "Press S to Save", (BOARD_WIDTH + 2) * TILE_SIZE, (2 + board->nextShape.size + 4) * TILE_SIZE);

    // 如果游戏暂停，显示暂停提示
    if (state == GAME_PAUSED) {
        renderText(r, "Game Paused", WIDTH / 2 - 50, HEIGHT / 2);
    }

    // 如果游戏结束，显示 "Game Over" 信息
    if (state == GAME_OVER) {
        renderGameOver(r);
    }

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