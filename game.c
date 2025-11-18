#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "game.h"
#include <stdio.h> 

#define SCORE_FILE "scores.dat"

void desenharTelaJogo(EstadoJogo* e);

void ordenarPerfis(EstadoJogo* e) {
    for (int i = 0; i < e->numPerfis - 1; i++) {
        for (int j = 0; j < e->numPerfis - i - 1; j++) {
            if (e->perfis[j].recorde < e->perfis[j + 1].recorde) {
                Perfil temp = e->perfis[j];
                e->perfis[j] = e->perfis[j + 1];
                e->perfis[j + 1] = temp;
            }
        }
    }
}

void limparBlocos(EstadoJogo* e) {
    Bloco *atual = e->listaDeBlocos;
    while (atual != NULL) {
        Bloco *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    e->listaDeBlocos = NULL;
    e->blocosAtivos = 0;
}

void respawnarBlocoAleatorio(EstadoJogo* e) {
    int larguraBloco = 60;
    int alturaBloco = 20;
    int espacoHorizontal = 20;
    int espacoVertical = 10;
    int offsetHorizontal = (e->telaLargura - (BLOCO_COLUNAS * (larguraBloco + espacoHorizontal)) + espacoHorizontal) / 2;
    int offsetVertical = 50;

    Color corHP3 = (Color){ 140, 140, 140, 255 };
    Color corHP2 = (Color){  30,  30, 180, 255 };
    Color corHP1 = (Color){ 190, 160, 220, 255 };

    int tentativas = 0;
    while (tentativas < 50) { 
        int i = rand() % BLOCO_LINHAS;
        int j = rand() % BLOCO_COLUNAS;

        float newX = offsetHorizontal + j * (larguraBloco + espacoHorizontal);
        float newY = offsetVertical + i * (alturaBloco + espacoVertical);
        
        bool slotOcupado = false;
        Bloco *blocoAtual = e->listaDeBlocos;
        while (blocoAtual != NULL) {
            if (blocoAtual->rect.x == newX && blocoAtual->rect.y == newY) {
                slotOcupado = true;
                break;
            }
            blocoAtual = blocoAtual->proximo;
        }

        if (!slotOcupado) {
            Bloco *novoBloco = (Bloco*)malloc(sizeof(Bloco));
            if (novoBloco == NULL) return; 

            novoBloco->rect.x = newX;
            novoBloco->rect.y = newY;
            novoBloco->rect.width = larguraBloco;
            novoBloco->rect.height = alturaBloco;
            novoBloco->ativo = true;
            
            if (i < 2) { novoBloco->hp = 3; novoBloco->cor = corHP3; }
            else if (i < 4) { novoBloco->hp = 2; novoBloco->cor = corHP2; }
            else { novoBloco->hp = 1; novoBloco->cor = corHP1; }
            
            novoBloco->proximo = e->listaDeBlocos;
            e->listaDeBlocos = novoBloco;
            
            e->blocosAtivos++;
            return; 
        }
        tentativas++;
    }
}

void carregarNivel(EstadoJogo* e, int nivel) {
    limparBlocos(e);

    int mapaNivel[BLOCO_LINHAS][BLOCO_COLUNAS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    
    int larguraBloco = 60;
    int alturaBloco = 20;
    int espacoHorizontal = 20;
    int espacoVertical = 10;
    int offsetHorizontal = (e->telaLargura - (BLOCO_COLUNAS * (larguraBloco + espacoHorizontal)) + espacoHorizontal) / 2;
    int offsetVertical = 50;
    
    Color corHP3 = (Color){ 140, 140, 140, 255 };
    Color corHP2 = (Color){  30,  30, 180, 255 };
    Color corHP1 = (Color){ 190, 160, 220, 255 };

    for (int i = 0; i < BLOCO_LINHAS; i++) {
        for (int j = 0; j < BLOCO_COLUNAS; j++) {
            if (mapaNivel[i][j] == 1) {
                Bloco *novoBloco = (Bloco*)malloc(sizeof(Bloco));
                if (novoBloco == NULL) return;
                
                novoBloco->rect.x = offsetHorizontal + j * (larguraBloco + espacoHorizontal);
                novoBloco->rect.y = offsetVertical + i * (alturaBloco + espacoVertical);
                novoBloco->rect.width = larguraBloco;
                novoBloco->rect.height = alturaBloco;
                novoBloco->ativo = true;
                
                if (i < 2) {
                    novoBloco->hp = 3;
                    novoBloco->cor = corHP3;
                } else if (i < 4) {
                    novoBloco->hp = 2;
                    novoBloco->cor = corHP2;
                } else {
                    novoBloco->hp = 1;
                    novoBloco->cor = corHP1;
                }
                
                novoBloco->proximo = e->listaDeBlocos;
                e->listaDeBlocos = novoBloco;
                e->blocosAtivos++;
            }
        }
    }
}

void initGame(EstadoJogo* e) {
    e->timerSpeed = 50;
    e->pontuacao = 0;
    e->vidas = 3;
    e->nivel = 1;
    
    e->jogador.pos.x = e->telaLargura / 2 - (e->jogador.largura / 2);
    e->jogador.pos.y = e->telaAltura - 60;
    e->bola.pos.x = e->telaLargura / 2;
    e->bola.pos.y = e->telaAltura - 70;
    
    if (rand() % 2 == 0) {
        e->bola.vel.dx = -4;
    } else {
        e->bola.vel.dx = 4;
    }
    e->bola.vel.dy = -4;
    
    e->mostrarDicaControle = true;
    e->timerDicaControle = 3.0f;
    e->cursorPause = 0;
    
    e->timerAceleracao = 60.0f;
    e->timerRespawn = 0.2f;
    e->blocosParaRespawnar = 0;
    
    carregarNivel(e, 1);
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
    
    e->blocosAtivos = 0;
    e->blocosParaRespawnar = 0;
    e->timerAceleracao = 0.0f;
    e->timerRespawn = 0.0f;

    e->telaAtual = TELA_MENU_PRINCIPAL;
    e->numPerfis = 0;
    e->perfilSelecionado = -1;
    
    carregarTopScores(e);
    initGame(e);

    return e;
}

void liberarEstado(EstadoJogo* estado) {
    salvarTopScores(estado);
    limparBlocos(estado);
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
            e->cursorMenu--;
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

void atualizarBola(EstadoJogo* e) {
    e->bola.pos.x += e->bola.vel.dx;
    e->bola.pos.y += e->bola.vel.dy;

    if (e->bola.pos.y <= 10) {
        e->bola.pos.y = 10;
        e->bola.vel.dy *= -1;
    }
    if (e->bola.pos.x <= 10) {
        e->bola.pos.x = 10;
        e->bola.vel.dx *= -1;
    }
    if (e->bola.pos.x >= e->telaLargura - 10) {
        e->bola.pos.x = e->telaLargura - 10;
        e->bola.vel.dx *= -1;
    }

    if (e->bola.pos.y >= e->telaAltura) {
        e->vidas--;
        
        if (e->vidas <= 0) {
            if (e->perfilSelecionado != -1) {
                if (e->pontuacao > e->perfis[e->perfilSelecionado].recorde) {
                    e->perfis[e->perfilSelecionado].recorde = e->pontuacao;
                    ordenarPerfis(e);
                    salvarTopScores(e);
                }
            }
            e->telaAtual = TELA_MENU_PRINCIPAL;
            initGame(e);
        } else {
            e->jogador.pos.x = e->telaLargura / 2 - (e->jogador.largura / 2);
            e->jogador.pos.y = e->telaAltura - 60;
            e->bola.pos.x = e->telaLargura / 2;
            e->bola.pos.y = e->telaAltura - 70;
            
            if (rand() % 2 == 0) {
                e->bola.vel.dx = -4;
            } else {
                e->bola.vel.dx = 4;
            }
            e->bola.vel.dy = -4;
            
            if (e->vidas == 1) e->timerAceleracao = 40.0f;
            else e->timerAceleracao = 60.0f;
        }
    }

    e->timerAceleracao -= GetFrameTime();
    if (e->timerAceleracao <= 0.0f) {
        if (e->bola.vel.dx > 0) e->bola.vel.dx += 1;
        else e->bola.vel.dx -= 1;

        if (e->bola.vel.dy > 0) e->bola.vel.dy += 1;
        else e->bola.vel.dy -= 1;

        if (e->vidas == 1) e->timerAceleracao = 40.0f;
        else e->timerAceleracao = 60.0f;
    }

    int totalBlocos = BLOCO_LINHAS * BLOCO_COLUNAS;
    int limiteRespawn = totalBlocos * 0.30f; 

    if (e->blocosAtivos <= limiteRespawn && e->blocosParaRespawnar == 0) {
        e->blocosParaRespawnar = totalBlocos * 0.25f;
        e->timerRespawn = 0.2f;
    }

    if (e->blocosParaRespawnar > 0) {
        e->timerRespawn -= GetFrameTime();
        if (e->timerRespawn <= 0.0f) {
            respawnarBlocoAleatorio(e);
            e->blocosParaRespawnar--;
            e->timerRespawn = 0.2f;
        }
    }
}

void verificarColisoes(EstadoJogo* e) {
    Vector2 bolaCentro = { (float)e->bola.pos.x, (float)e->bola.pos.y };
    float bolaRaio = 10.0f;
    Rectangle jogadorRect = { (float)e->jogador.pos.x, (float)e->jogador.pos.y, (float)e->jogador.largura, 20.0f };

    if (CheckCollisionCircleRec(bolaCentro, bolaRaio, jogadorRect)) {
        if (e->bola.vel.dy > 0) {
            e->bola.vel.dy *= -1;
            e->bola.pos.y = e->jogador.pos.y - bolaRaio;
        }
    }
    
    Bloco *blocoAtual = e->listaDeBlocos;
    Bloco *blocoAnterior = NULL;

    while (blocoAtual != NULL) {
        Bloco *proximoBloco = blocoAtual->proximo;

        if (blocoAtual->ativo) {
            if (CheckCollisionCircleRec(bolaCentro, bolaRaio, blocoAtual->rect)) {
                e->bola.vel.dy *= -1;
                e->pontuacao += 10;
                blocoAtual->hp--;
                
                if (blocoAtual->hp == 2) {
                    blocoAtual->cor = (Color){  30,  30, 180, 255 }; 
                } else if (blocoAtual->hp == 1) {
                    blocoAtual->cor = (Color){ 190, 160, 220, 255 };
                }

                if (blocoAtual->hp <= 0) {
                    if (blocoAnterior == NULL) {
                        e->listaDeBlocos = proximoBloco;
                    } else {
                        blocoAnterior->proximo = proximoBloco;
                    }
                    
                    free(blocoAtual);
                    blocoAtual = NULL;
                    e->blocosAtivos--;
                }
                
                return; 
            }
        }
        
        if (blocoAtual != NULL) {
            blocoAnterior = blocoAtual;
            blocoAtual = proximoBloco;
        }
    }
}

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

    DrawCircleV((Vector2){(float)e->bola.pos.x, (float)e->bola.pos.y}, 10, WHITE);

    Bloco *blocoAtual = e->listaDeBlocos;
    while (blocoAtual != NULL) {
        if (blocoAtual->ativo) {
            DrawRectangleRec(blocoAtual->rect, blocoAtual->cor);
        }
        blocoAtual = blocoAtual->proximo;
    }

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
        DrawText("Pressione P para pausar", 10, e->telaAltura - 30, 20, DARKGRAY);
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
        
        int limite = e->numPerfis;
        if (limite > 8) limite = 8;

        for (int i = 0; i < limite; i++) {
            sprintf(textoPerfil, "%d. %s   %d", i + 1, e->perfis[i].iniciais, e->perfis[i].recorde);
            DrawText(textoPerfil, x_meio - MeasureText(textoPerfil, 20)/2, y_meio - 30 + (i*30), 20, WHITE);
        }
        DrawText("Pressione Q para voltar", x_meio - MeasureText("Pressione Q para voltar", 20)/2, e->telaAltura - 80, 20, GRAY);
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
    ordenarPerfis(e);
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