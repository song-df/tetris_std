#include "controller.h"

int main() {
    GameController gameController;
    initGameController(&gameController);
    runGame(&gameController);
    cleanupRenderer(&gameController.renderer);
    printf("game exit\n");
    return 0;
}