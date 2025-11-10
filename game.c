#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "game.h"
#include <stdio.h> 

EstadoJogo* criarEstadoInicial(int largura, int altura) {

    EstadoJogo* e = malloc(sizeof(EstadoJogo));
    if (!e) return NULL;

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
    e->registroCursor = 0;
    strcpy(e->registroIniciais, "___");
    e->jogador.largura = 100;
    e->jogador.pos.x = largura / 2 - (e->jogador.largura / 2);
    e->jogador.pos.y = altura - 40;
    e->jogador.simbolo = "=======";
    e->bola.pos.x = largura / 2;
    e->bola.pos.y = altura - 50;
    e->bola.vel.dx = 1;
    e->bola.vel.dy = -1;
    e->bola.simbolo = 'O';

    return e;
}

void liberarEstado(EstadoJogo* estado) {
    free(estado);
}

void atualizarJogador(EstadoJogo* e) {

    if (e->telaAtual == TELA_MENU_PRINCIPAL) {
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            e->cursorMenu++;
            if (e->cursorMenu > 2) e->cursorMenu = 0;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            e->cursorMenu--;
            if (e->cursorMenu < 0) e->cursorMenu = 2;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (e->cursorMenu == 0) {
                e->telaAtual = TELA_PERGUNTA_PERFIL;
                e->cursorMenu = 0;
            } else if (e->cursorMenu == 1) {
                carregarTopScores(e);
                e->telaAtual = TELA_TOP_SCORES;
            } else if (e->cursorMenu == 2) {
                e->deveSair = 1;
            }
        }
    }
    else if (e->telaAtual == TELA_JOGO) {
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            if (e->jogador.pos.x > 0) {
                e->jogador.pos.x -= 7;
            }
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            int x_maximo = e->telaLargura - e->jogador.largura;
            if (e->jogador.pos.x < x_maximo) {
                e->jogador.pos.x += 7;
            }
        }
    }
    else if (e->telaAtual == TELA_TOP_SCORES) {
        if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ENTER)) {
            e->telaAtual = TELA_MENU_PRINCIPAL;
        }
    }
    else if (e->telaAtual == TELA_PERGUNTA_PERFIL) {
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            e->cursorMenu++;
            if (e->cursorMenu > 2) e->cursorMenu = 0;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            e->cursorMenu--;
            if (e->cursorMenu < 0) e->cursorMenu = 2;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (e->cursorMenu == 0) {
                e->telaAtual = TELA_MENU_PRINCIPAL;
            } else if (e->cursorMenu == 1) { 
                e->telaAtual = TELA_REGISTRAR_PERFIL; 
                e->registroCursor = 0;
                strcpy(e->registroIniciais, "___");
            } else if (e->cursorMenu == 2) {
                e->telaAtual = TELA_MENU_PRINCIPAL;
            }
        }
    }
    else if (e->telaAtual == TELA_REGISTRAR_PERFIL) {
        int tecla = GetCharPressed();
        while (tecla > 0) {
            if (((tecla >= 'A' && tecla <= 'Z') || (tecla >= 'a' && tecla <= 'z')) && (e->registroCursor < 3)) {
                if (tecla >= 'a' && tecla <= 'z') tecla = tecla - 32; 
                e->registroIniciais[e->registroCursor] = (char)tecla;
                e->registroCursor++;
            }
            tecla = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (e->registroCursor > 0) {
                e->registroCursor--;
                e->registroIniciais[e->registroCursor] = '_';
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (e->registroCursor == 3) {
                if (e->numPerfis < 10) {
                    strcpy(e->perfis[e->numPerfis].iniciais, e->registroIniciais);
                    e->perfis[e->numPerfis].recorde = 0;
                    e->perfilSelecionado = e->numPerfis;
                    e->numPerfis++;
                } else {
                    e->perfilSelecionado = 9;
                }
                e->telaAtual = TELA_JOGO;
            }
        }
    }
}

void atualizarBola(EstadoJogo* e) { }
void verificarColisoes(EstadoJogo* e) { }

void desenharTudo(EstadoJogo* e, Texture2D logo) { 
    DrawText("Pressione ESC para fechar", 10, 10, 20, DARKGRAY);

    if (e->telaAtual == TELA_MENU_PRINCIPAL) {
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;
        int logoX = x_meio - logo.width / 2;
        int logoY = y_meio - 300;
        DrawTexture(logo, logoX, logoY, WHITE);
        
        int fontSize = 30;
        int spacing = 40;
        DrawText(TextFormat("%s Iniciar Jogo", (e->cursorMenu == 0) ? ">" : " "), x_meio - MeasureText("> Iniciar Jogo", fontSize)/2, y_meio + 50, fontSize, WHITE);
        DrawText(TextFormat("%s Top Scores", (e->cursorMenu == 1) ? ">" : " "), x_meio - MeasureText("> Top Scores", fontSize)/2, y_meio + spacing + 50, fontSize, WHITE);
        DrawText(TextFormat("%s Sair", (e->cursorMenu == 2) ? ">" : " "), x_meio - MeasureText("> Sair", fontSize)/2, y_meio + spacing*2 + 50, fontSize, WHITE);
        DrawText("Use 'w' e 's' para mover", x_meio - MeasureText("Use 'w' e 's' para mover", 20)/2, y_meio + 200, 20, GRAY);
    }
    else if (e->telaAtual == TELA_JOGO) {
        DrawRectangle(e->jogador.pos.x, e->jogador.pos.y, e->jogador.largura, 20, WHITE);
        int recordeAtual = 0;
        if (e->perfilSelecionado != -1) {
            recordeAtual = e->perfis[e->perfilSelecionado].recorde;
        }
        DrawText(TextFormat("RECORDE: %d    PONTOS: %d", recordeAtual, e->pontuacao), 20, e->telaAltura - 30, 20, WHITE);
        DrawText(TextFormat("VIDAS: %d", e->vidas), e->telaLargura - 100, e->telaAltura - 30, 20, WHITE);
    }
    else if (e->telaAtual == TELA_TOP_SCORES) {
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;
        DrawText("TOP SCORES", x_meio - MeasureText("TOP SCORES", 40)/2, y_meio - 100, 40, YELLOW);
        char textoPerfil[32];
        for (int i = 0; i < e->numPerfis; i++) {
            sprintf(textoPerfil, "%d. %s   %d", i + 1, e->perfis[i].iniciais, e->perfis[i].recorde);
            DrawText(textoPerfil, x_meio - MeasureText(textoPerfil, 20)/2, y_meio - 30 + (i*30), 20, WHITE);
        }
        DrawText("Pressione ENTER para voltar", x_meio - MeasureText("Pressione ENTER para voltar", 20)/2, y_meio + 100, 20, GRAY);
    }
    else if (e->telaAtual == TELA_PERGUNTA_PERFIL) {
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;
        DrawText("Você já tem um perfil cadastrado?", x_meio - MeasureText("Você já tem um perfil cadastrado?", 30)/2, y_meio - 50, 30, WHITE);
        DrawText(TextFormat("%s Sim (Carregar Perfil)", (e->cursorMenu == 0) ? ">" : " "), x_meio - MeasureText("> Sim (Carregar Perfil)", 20)/2, y_meio + 10, 20, WHITE);
        DrawText(TextFormat("%s Não (Novo Perfil)", (e->cursorMenu == 1) ? ">" : " "), x_meio - MeasureText("> Não (Novo Perfil)", 20)/2, y_meio + 40, 20, WHITE);
        DrawText(TextFormat("%s Voltar ao Menu", (e->cursorMenu == 2) ? ">" : " "), x_meio - MeasureText("> Voltar ao Menu", 20)/2, y_meio + 70, 20, WHITE);
    }
    else if (e->telaAtual == TELA_REGISTRAR_PERFIL) {
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;
        DrawText("Digite suas 3 iniciais:", x_meio - MeasureText("Digite suas 3 iniciais:", 20)/2, y_meio - 50, 20, WHITE);
        DrawText(TextFormat("%c %c %c", e->registroIniciais[0], e->registroIniciais[1], e->registroIniciais[2]), 
                 x_meio - MeasureText("A B C", 40)/2, y_meio, 40, YELLOW);
        if (e->registroCursor < 3) {
            int x_cursor = x_meio - (MeasureText("A B C", 40)/2) + (e->registroCursor * 30) - 5;
            DrawRectangle(x_cursor, y_meio + 45, 25, 5, YELLOW);
        }
        DrawText("Use BACKSPACE para apagar.", x_meio - MeasureText("Use BACKSPACE para apagar.", 20)/2, y_meio + 100, 20, GRAY);
        DrawText("Aperte ENTER para confirmar (depois das 3)", x_meio - MeasureText("Aperte ENTER para confirmar (depois das 3)", 20)/2, y_meio + 130, 20, GRAY);
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