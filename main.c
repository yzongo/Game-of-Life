#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "life.h"

int main(void) {
    printf("Hello, World!\n");
    srand(time(NULL));
    Board* test = initBoard();
    renderBoard(test);
    printf("####################\n");
    test= random_board();
    renderBoard(test);
    test = nextState(test);
    printf("####################\n");
    infiniteLife(test);

    return 0;
}
