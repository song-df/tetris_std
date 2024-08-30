#include "file.h"
#include <stdio.h>
#include <stdlib.h>

// 检查保存文件是否存在
bool checkSaveFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

void saveGameToTXT(GameBoard *board, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for saving.\n");
        return;
    }

    // 保存分数
    fprintf(file, "%d\n", board->score);

    // 保存游戏板状态
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            fprintf(file, "%d ", board->board[x][y].active ? 1 : 0);
        }
        fprintf(file, "\n");
    }

    // 保存当前形状
    fprintf(file, "%d %d %d\n", board->currentShape.x, board->currentShape.y, board->currentShape.size);
    fprintf(file, "%d %d %d\n", board->currentShape.color.r, board->currentShape.color.g, board->currentShape.color.b);

    for (int i = 0; i < board->currentShape.size; i++) {
        for (int j = 0; j < board->currentShape.size; j++) {
            fprintf(file, "%d ", board->currentShape.matrix[i][j] ? 1 : 0);
        }
        fprintf(file, "\n");
    }

    // 保存下一个形状
    fprintf(file, "%d\n", board->nextShape.size);
    fprintf(file, "%d %d %d\n", board->nextShape.color.r, board->nextShape.color.g, board->nextShape.color.b);

    for (int i = 0; i < board->nextShape.size; i++) {
        for (int j = 0; j < board->nextShape.size; j++) {
            fprintf(file, "%d ", board->nextShape.matrix[i][j] ? 1 : 0);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game saved successfully.\n");
}

void loadGameFromTXT(GameBoard *board, const char *filename) {
    FILE *file = fopen(filename, "r");
    int a,b,c;
    if (!file) {
        printf("Failed to open file for loading.\n");
        return;
    }

    // 加载分数
    fscanf(file, "%d\n", &board->score);

    // 加载游戏板状态
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            int active;
            fscanf(file, "%d", &active);
            board->board[x][y].active = (active == 1);
        }
    }

    // 加载当前形状
    fscanf(file, "%d %d %d\n", &a, &b, &c);
    board->currentShape.x = a;
    board->currentShape.y = b;
    board->currentShape.size = c;

    fscanf(file, "%d %d %d\n", &a,&b,&c);
    board->currentShape.color.r = a;
    board->currentShape.color.g = b;
    board->currentShape.color.b = c;

    for (int i = 0; i < board->currentShape.size; i++) {
        for (int j = 0; j < board->currentShape.size; j++) {
            int value;
            fscanf(file, "%d", &value);
            board->currentShape.matrix[i][j] = (value == 1);
        }
    }

    // 加载下一个形状
    fscanf(file, "%d\n", &a);
    board->nextShape.size = a; 
    fscanf(file, "%d %d %d\n", &a,&b,&c);
    board->nextShape.color.r = a;
    board->nextShape.color.g = b;
    board->nextShape.color.b = c;

    for (int i = 0; i < board->nextShape.size; i++) {
        for (int j = 0; j < board->nextShape.size; j++) {
            int value;
            fscanf(file, "%d", &value);
            board->nextShape.matrix[i][j] = (value == 1);
        }
    }

    fclose(file);
    printf("Game loaded successfully.\n");
}