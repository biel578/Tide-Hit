#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "game.h"
#include <stdio.h> 

#define SCORE_FILE "scores.dat"

void desenharTelaJogo(EstadoJogo* e);

void initGame(EstadoJogo* e) {
    e->timerSpeed = 50;
    e->pontuacao = 0;
    e->vidas = 3;
    e->nivel = 1;
    
    e->jogador.pos.x = e->telaLargura / 2 - (e->jogador.largura / 2);
    e->jogador.pos.y = e->telaAltura - 60;
    e->bola.pos.x = e->telaLargura / 2;
    e->bola.pos.y = e->telaAltura - 70;
    e->bola.vel.dx = 1;
    e->bola.vel.dy = -1;
    
    e->mostrarDicaControle = true;
    e->timerDicaControle = 3.0f;
    e->cursorPause = 0;
}

EstadoJogo* criarEstadoInicial(int largura, int altura) {

    EstadoJogo* e = malloc(sizeof(EstadoJogo));
    if (!e) return NULL;

    e->telaLargura = largura;
    e->telaAltura = altura;
    e->jogador.largura = 100;
    e->jogador.simbolo = "=======";
    e->bola.simbolo = 'O';
    e->deveSair = 0;
    e->cursorMenu = 0;
    e->listaDeBlocos = NULL;
    
    e->telaAtual = TELA_MENU_PRINCIPAL;
    e->numPerfis = 0;
    e->perfilSelecionado = -1;
    
    carregarTopScores(e);
    initGame(e);

    return e;
}

void liberarEstado(EstadoJogo* estado) {
    salvarTopScores(estado);
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
                e->telaAtual = TELA_TOP_SCORES;
            } else if (e->cursorMenu == 2) {
                e->deveSair = 1;
            }
        }
    }
    else if (e->telaAtual == TELA_JOGO) {
        if (IsKeyPressed(KEY_P)) {
            e->telaAtual = TELA_PAUSE;
            e->cursorPause = 0;
        }

        if (e->mostrarDicaControle) {
            e->timerDicaControle -= GetFrameTime();
            if (e->timerDicaControle <= 0.0f) {
                e->mostrarDicaControle = false;
            }
        }

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
    else if (e->telaAtual == TELA_PAUSE) {
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            e->cursorPause++;
            if (e->cursorPause > 1) e->cursorPause = 0;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            e->cursorPause--;
            if (e->cursorPause < 0) e->cursorPause = 1;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (e->cursorPause == 0) {
                e->telaAtual = TELA_JOGO;
            } else if (e->cursorPause == 1) {
                salvarTopScores(e);
                e->telaAtual = TELA_MENU_PRINCIPAL;
                initGame(e);
            }
        }
    }
    else if (e->telaAtual == TELA_TOP_SCORES) {
        if (IsKeyPressed(KEY_Q)) {
            e->telaAtual = TELA_MENU_PRINCIPAL;
        }
    }
    else if (e->telaAtual == TELA_PERGUNTA_PERFIL) {
        if (IsKeyPressed(KEY_Q)) {
            e->telaAtual = TELA_MENU_PRINCIPAL;
        }

        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            e->cursorMenu++;
            if (e->cursorMenu > 1) e->cursorMenu = 0;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            e->cursorMenu--;
            if (e->cursorMenu < 0) e->cursorMenu = 1;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (e->cursorMenu == 0) {
                e->telaAtual = TELA_SELECIONAR_PERFIL;
                e->cursorMenu = 0; 
            } else if (e->cursorMenu == 1) { 
                e->telaAtual = TELA_REGISTRAR_PERFIL; 
                e->registroCursor = 0;
                strcpy(e->registroIniciais, "___");
            }
        }
    }
    else if (e->telaAtual == TELA_REGISTRAR_PERFIL) {
        if (IsKeyPressed(KEY_Q)) {
            e->telaAtual = TELA_PERGUNTA_PERFIL;
        }

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
                if (e->numPerfis < MAX_PERFIS) {
                    strcpy(e->perfis[e->numPerfis].iniciais, e->registroIniciais);
                    e->perfis[e->numPerfis].recorde = 0;
                    e->perfilSelecionado = e->numPerfis;
                    e->numPerfis++;
                } else {
                    e->perfilSelecionado = 0;
                }
                
                initGame(e);
                e->telaAtual = TELA_JOGO;
                
            }
        }
    }
    else if (e->telaAtual == TELA_SELECIONAR_PERFIL) {
        if (IsKeyPressed(KEY_Q)) {
            e->telaAtual = TELA_PERGUNTA_PERFIL;
        }

        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            e->cursorMenu++;
            if (e->cursorMenu >= e->numPerfis) e->cursorMenu = 0;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            e->cursorMenu--;
            if (e->cursorMenu < 0) e->cursorMenu = e->numPerfis - 1;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            e->perfilSelecionado = e->cursorMenu;
            initGame(e);
            e->telaAtual = TELA_JOGO;
        }
    }
}

void atualizarBola(EstadoJogo* e) { }
void verificarColisoes(EstadoJogo* e) { }

void desenharTelaJogo(EstadoJogo* e) {
    int recordeAtual = 0;
    char iniciaisJogador[4] = "???";
    if (e->perfilSelecionado != -1 && e->perfilSelecionado < e->numPerfis) {
        recordeAtual = e->perfis[e->perfilSelecionado].recorde;
        strcpy(iniciaisJogador, e->perfis[e->perfilSelecionado].iniciais);
    }
    
    DrawText(TextFormat("RECORDE(%s): %d", iniciaisJogador, recordeAtual), 20, 10, 20, WHITE);
    DrawText(TextFormat("PONTOS: %d", e->pontuacao), e->telaLargura / 2 - MeasureText(TextFormat("PONTOS: %d", e->pontuacao), 20)/2, 10, 20, WHITE);
    DrawText(TextFormat("VIDAS: %d", e->vidas), e->telaLargura - MeasureText(TextFormat("VIDAS: %d", e->vidas), 20) - 20, 10, 20, WHITE);

    DrawRectangle(e->jogador.pos.x, e->jogador.pos.y, e->jogador.largura, 20, WHITE);

    if (e->mostrarDicaControle) {
        const char* dicaControle = "Use 'a' e 'd' para mover";
        int dicaWidth = MeasureText(dicaControle, 20);
        DrawText(dicaControle, e->telaLargura / 2 - dicaWidth / 2, e->telaAltura / 2, 20, GRAY);
    }
}

void desenharTudo(EstadoJogo* e, Texture2D logo) { 
    const char* escText = "Pressione ESC para fechar";
    int escTextWidth = MeasureText(escText, 20);
    DrawText(escText, e->telaLargura - escTextWidth - 10, e->telaAltura - 30, 20, DARKGRAY);

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
    }
    else if (e->telaAtual == TELA_JOGO) {
        desenharTelaJogo(e);
    }
    else if (e->telaAtual == TELA_PAUSE) {
        desenharTelaJogo(e);
        
        DrawRectangle(0, 0, e->telaLargura, e->telaAltura, ColorAlpha(BLACK, 0.7f));
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;
        
        DrawText("PAUSADO", x_meio - MeasureText("PAUSADO", 40)/2, y_meio - 80, 40, WHITE);
        DrawText(TextFormat("%s Retornar", (e->cursorPause == 0) ? ">" : " "), x_meio - MeasureText("> Retornar", 30)/2, y_meio, 30, WHITE);
        DrawText(TextFormat("%s Voltar para o Menu", (e->cursorPause == 1) ? ">" : " "), x_meio - MeasureText("> Voltar para o Menu", 30)/2, y_meio + 40, 30, WHITE);
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
        DrawText("Pressione Q para voltar", x_meio - MeasureText("Pressione Q para voltar", 20)/2, y_meio + 100, 20, GRAY);
    }
    else if (e->telaAtual == TELA_PERGUNTA_PERFIL) {
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;
        DrawText("Você já tem um perfil cadastrado?", x_meio - MeasureText("Você já tem um perfil cadastrado?", 30)/2, y_meio - 50, 30, WHITE);
        DrawText(TextFormat("%s Sim (Carregar Perfil)", (e->cursorMenu == 0) ? ">" : " "), x_meio - MeasureText("> Sim (Carregar Perfil)", 20)/2, y_meio + 10, 20, WHITE);
        DrawText(TextFormat("%s Não (Novo Perfil)", (e->cursorMenu == 1) ? ">" : " "), x_meio - MeasureText("> Não (Novo Perfil)", 20)/2, y_meio + 40, 20, WHITE);
        DrawText("Pressione Q para voltar", x_meio - MeasureText("Pressione Q para voltar", 20)/2, y_meio + 100, 20, GRAY);
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
        DrawText("Pressione Q para voltar", x_meio - MeasureText("Pressione Q para voltar", 20)/2, y_meio + 160, 20, GRAY);
    }
    else if (e->telaAtual == TELA_SELECIONAR_PERFIL) {
        int x_meio = e->telaLargura / 2;
        int y_meio = e->telaAltura / 2;
        DrawText("SELECIONE SEU PERFIL", x_meio - MeasureText("SELECIONE SEU PERFIL", 40)/2, y_meio - 100, 40, YELLOW);
        
        char textoPerfil[32];
        int numOpcoes = e->numPerfis;
        
        for (int i = 0; i < numOpcoes; i++) {
            Color cor = (e->cursorMenu == i) ? YELLOW : WHITE;
            sprintf(textoPerfil, "%s %s (Recorde: %d)", (e->cursorMenu == i) ? ">" : " ", e->perfis[i].iniciais, e->perfis[i].recorde);
            DrawText(textoPerfil, x_meio - MeasureText(textoPerfil, 25)/2, y_meio - 30 + (i*35), 25, cor);
        }
        
        DrawText("Pressione ENTER para selecionar", x_meio - MeasureText("Pressione ENTER para selecionar", 20)/2, y_meio + (numOpcoes * 35) + 30, 20, GRAY);
        DrawText("Pressione Q para voltar", x_meio - MeasureText("Pressione Q para voltar", 20)/2, y_meio + (numOpcoes * 35) + 60, 20, GRAY);
    }
}

void carregarNivel(EstadoJogo* e, int nivel) {
}

void carregarTopScores(EstadoJogo* e) {
    FILE *f = fopen(SCORE_FILE, "rb");
    if (f == NULL) {
        e->numPerfis = 0;
        return;
    }

    if (fread(&(e->numPerfis), sizeof(int), 1, f) != 1) {
        e->numPerfis = 0;
    } else {
        if (e->numPerfis > MAX_PERFIS) {
            e->numPerfis = MAX_PERFIS;
        }
        fread(e->perfis, sizeof(Perfil), e->numPerfis, f);
    }
    
    fclose(f);
}

void salvarTopScores(EstadoJogo* e) {
    FILE *f = fopen(SCORE_FILE, "wb");
    if (f == NULL) {
        return;
    }

    fwrite(&(e->numPerfis), sizeof(int), 1, f);
    fwrite(e->perfis, sizeof(Perfil), e->numPerfis, f);
    
    fclose(f);
}