#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "game.h"

#define TELA_LARGURA 80
#define TELA_ALTURA 24

int main() {

    screenInit(1);
    keyboardInit(1);

    EstadoJogo* estado = criarEstadoInicial(TELA_LARGURA, TELA_ALTURA);

    if (estado == NULL) {
        printf("Falha ao alocar memoria!\n");
        return 1;
    }

    timerInit(estado->timerSpeed);

    int ch = 0;

    while (ch != 'q' && estado->deveSair == 0) {

        if (keyhit()) {
            ch = readch();

            if (ch == 'q') {
                break;
            }

            atualizarJogador(estado, ch);
        }

        if (timerTimeOver() == 1) {

        }

        screenClear();
        desenharTudo(estado);
        screenUpdate();
    }

    keyboardDestroy();
    screenDestroy();

    liberarEstado(estado);

    return 0;
}