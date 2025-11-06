#include <stdlib.h>
#include <string.h>
#include "game.h"

#include "screen.h"
#include <stdio.h>

EstadoJogo* criarEstadoInicial(int largura, int altura) {

    EstadoJogo* e = malloc(sizeof(EstadoJogo));
    if (!e) return NULL;
//esse "e" é o estado

    e->telaLargura = largura;
    e->telaAltura = altura;
    e->timerSpeed = 50;
    e->listaDeBlocos = NULL;

    e->pontuacao = 0;
    e->vidas = 3;
    e->nivel = 1;

    e->telaAtual = TELA_MENU_PRINCIPAL;
    e->cursorMenu = 0;
    e->numPerfis = 0;
    e->perfilSelecionado = -1;
    e->deveSair = 0;

    e->jogador.largura = 7;
    e->jogador.pos.x = largura / 2 - 3;
    e->jogador.pos.y = altura - 2;
    e->jogador.simbolo = "=======";

    e->bola.pos.x = largura / 2;
    e->bola.pos.y = altura - 3;
    e->bola.vel.dx = 1;
    e->bola.vel.dy = -1;
    e->bola.simbolo = 'O';

    return e;
}

void liberarEstado(EstadoJogo* estado) {
// esses voids eu vou atualzando dps
    free(estado);
}

void atualizarJogador(EstadoJogo* e, int tecla) {

    if (e->telaAtual == TELA_MENU_PRINCIPAL) {

        if (tecla == 's') {
            e->cursorMenu++;
            if (e->cursorMenu > 2) {
                e->cursorMenu = 0;
            }
        }
        else if (tecla == 'w') {
            e->cursorMenu--;
            if (e->cursorMenu < 0) {
                e->cursorMenu = 2;
            }
        }
        else if (tecla == 13 || tecla == 10) {

            if (e->cursorMenu == 0) {
                e->telaAtual = TELA_PERGUNTA_PERFIL;
                e->cursorMenu = 0;
            }
            else if (e->cursorMenu == 1) {
                carregarTopScores(e);
                e->telaAtual = TELA_TOP_SCORES;
            }
            else if (e->cursorMenu == 2) {
                e->deveSair = 1;
            }
        }
    }
    else if (e->telaAtual == TELA_JOGO) {

        if (tecla == 'a') {
            if (e->jogador.pos.x > 2) {
                e->jogador.pos.x--;
            }
        }
        else if (tecla == 'd') {
            int x_maximo = e->telaLargura - e->jogador.largura - 1;
            if (e->jogador.pos.x < x_maximo) {
                e->jogador.pos.x++;
            }
        }
    }
    else if (e->telaAtual == TELA_TOP_SCORES) {

        if (tecla == 'q' || tecla == 13 || tecla == 10) {
            e->telaAtual = TELA_MENU_PRINCIPAL;
        }
    }
    else if (e->telaAtual == TELA_PERGUNTA_PERFIL) {
        
        if (tecla == 's') {
            e->cursorMenu++;
            if (e->cursorMenu > 2) { // 2 é a última opção ("Voltar")
                e->cursorMenu = 0;
            }
        }
        else if (tecla == 'w') {
            e->cursorMenu--;
            if (e->cursorMenu < 0) {
                e->cursorMenu = 2;
            }
        }
        else if (tecla == 13 || tecla == 10) { // ENTER
            
            if (e->cursorMenu == 0) { // "Sim (Carregar)"
                // (Para o Dia 5: e->telaAtual = TELA_CARREGAR_PERFIL;)
                e->telaAtual = TELA_MENU_PRINCIPAL; // Volta ao menu por enquanto
            }
            else if (e->cursorMenu == 1) { // "Não (Novo)"
                // (Para o Dia 5: e->telaAtual = TELA_REGISTRAR_PERFIL;)
                e->telaAtual = TELA_MENU_PRINCIPAL; // Volta ao menu por enquanto
            }
            else if (e->cursorMenu == 2) { // "Voltar"
                e->telaAtual = TELA_MENU_PRINCIPAL;
            }
        }
    }
}

void atualizarBola(EstadoJogo* e) {

}

void verificarColisoes(EstadoJogo* e) {

}

void desenharTudo(EstadoJogo* e) {

    if (e->telaAtual == TELA_MENU_PRINCIPAL) {
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;

        screenSetColor(YELLOW, BLACK);
        screenGotoxy(x_meio - 5, y_meio - 4);
        printf("TIDE HIT!");
        screenSetColor(WHITE, BLACK);

        screenGotoxy(x_meio - 7, y_meio + 0);
        printf("%s Iniciar Jogo", (e->cursorMenu == 0) ? ">" : " ");

        screenGotoxy(x_meio - 7, y_meio + 1);
        printf("%s Top Scores", (e->cursorMenu == 1) ? ">" : " ");

        screenGotoxy(x_meio - 7, y_meio + 2);
        printf("%s Sair", (e->cursorMenu == 2) ? ">" : " ");

        screenGotoxy(x_meio - 12, y_meio + 5);
        printf("Use 'w' e 's' para mover");

    }
    else if (e->telaAtual == TELA_JOGO) {

        screenSetColor(WHITE, BLACK);
        screenGotoxy(e->jogador.pos.x, e->jogador.pos.y);
        printf("%s", e->jogador.simbolo);

        int recordeAtual = 0;
        if (e->perfilSelecionado != -1) {
            recordeAtual = e->perfis[e->perfilSelecionado].recorde;
        }

        screenSetColor(WHITE, BLACK);
        screenGotoxy(2, e->telaAltura - 1);

        printf("RECORDE: %d    PONTOS: %d", recordeAtual, e->pontuacao);

        screenGotoxy(e->telaLargura - 10, e->telaAltura - 1);
        printf("VIDAS: %d", e->vidas);

    }
    else if (e->telaAtual == TELA_TOP_SCORES) {

        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;

        screenSetColor(YELLOW, BLACK);
        screenGotoxy(x_meio - 5, y_meio - 5);
        printf("TOP SCORES");
        screenSetColor(WHITE, BLACK);

        for (int i = 0; i < e->numPerfis; i++) {
            screenGotoxy(x_meio - 8, y_meio - 3 + i);
            printf("%d. %s   %d", i + 1, e->perfis[i].iniciais, e->perfis[i].recorde);
        }

        screenGotoxy(x_meio - 12, y_meio + 3);
        printf("Pressione 'q' ou ENTER para voltar");
    }
    else if (e->telaAtual == TELA_PERGUNTA_PERFIL) {
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;
        
        screenSetColor(WHITE, BLACK);
        screenGotoxy(x_meio - 14, y_meio - 2);
        printf("Você já tem um perfil cadastrado?");

        screenGotoxy(x_meio - 10, y_meio + 1);
        printf("%s Sim (Carregar Perfil)", (e->cursorMenu == 0) ? ">" : " ");

        screenGotoxy(x_meio - 10, y_meio + 2);
        printf("%s Não (Novo Perfil)", (e->cursorMenu == 1) ? ">" : " ");

        screenGotoxy(x_meio - 10, y_meio + 3);
        printf("%s Voltar ao Menu", (e->cursorMenu == 2) ? ">" : " ");
    }

}

void carregarNivel(EstadoJogo* e, int nivel) {

}

void carregarTopScores(EstadoJogo* e) {

    strcpy(e->perfis[0].iniciais, "FEL");
    e->perfis[0].recorde = 1000;

    strcpy(e->perfis[1].iniciais, "NAT");
    e->perfis[1].recorde = 500;

    e->numPerfis = 2;
}

void salvarTopScores(EstadoJogo* e) {

}