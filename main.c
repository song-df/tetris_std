#include "controller.h"

int main() {
    GameController gameController;
    initGameController(&gameController);
    runGame(&gameController);
    cleanupRenderer(&gameController.renderer);
    return 0;
}