#ifndef FILE_H
#define FILE_H

#include "model.h"  // 包含游戏状态相关的结构体定义

// 检查保存文件是否存在
bool checkSaveFile(const char *filename);

// 保存游戏状态到TXT文件
void saveGameToTXT(GameBoard *board, const char *filename);

// 从TXT文件加载游戏状态
void loadGameFromTXT(GameBoard *board, const char *filename);

#endif // FILE_H