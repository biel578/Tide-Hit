#include <stdlib.h>
#include <string.h>
#include "game.h"

#include "screen.h"
#include <stdio.h>

EstadoJogo* criarEstadoInicial(int largura, int altura) {

    EstadoJogo* e = malloc(sizeof(EstadoJogo));
    if(!e) return NULL;
//esse "e" é o estado
    e->telaLargura = largura;
    e->telaAltura = altura;
    e->pontuacao = 0;
    e->vidas = 3;
    e->nivel = 1;
    e->timerSpeed = 50;
    e->listaDeBlocos = NULL;

    e->jogador.largura = 7;
    e->jogador.pos.x = largura/2 - 3;
    e->jogador.pos.y = altura - 2;
    e->jogador.simbolo = "=======";

    e->bola.pos.x = largura/2;
    e->bola.pos.y = altura - 3;
    e->bola.vel.dx = 1;
    e->bola.vel.dy = -1;
    e->bola.simbolo = 'O';

    return e;
}

void liberarEstado(EstadoJogo* estado){
// esses voids eu vou atualzando dps
    free(estado);
}

void atualizarJogador(EstadoJogo* e, int tecla){

}

void atualizarBola(EstadoJogo* e){

}

void verificarColisoes(EstadoJogo* e){

}

void desenharTudo(EstadoJogo* estado){

    int x_meio = estado->telaLargura/ 2;
    int y_meio = estado->telaAltura / 2;

    screenSetColor(YELLOW, BLACK);


    screenGotoxy(x_meio - 10,y_meio - 2);
    printf("====================");

    screenGotoxy(x_meio - 5, y_meio);
    printf("TIDE HIT!");

    screenGotoxy(x_meio - 10, y_meio + 2);
    printf("====================");
    screenSetColor(WHITE, BLACK);

    screenGotoxy(x_meio - 12, y_meio + 5);
    printf("Pressione 'q' para sair");
}

void carregarNivel(EstadoJogo* e, int nivel){

}
