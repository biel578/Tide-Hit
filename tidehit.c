#include <stdio.h>
#include <unistd.h>


#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int main() {

    screenInit(1);
    keyboardInit(1);

    screenClear();

    screenGotoxy(30, 10);
    printf("Tide Hit! Pressione 'q' para sair.");

    screenUpdate();

    int ch = 0;
    while (ch != 'q') {
        if (keyhit()) {
            ch = readch();

            screenGotoxy(30, 12);
            printf("Tecla: %d  ", ch);
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();

    return 0;
}
