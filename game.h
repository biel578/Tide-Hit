#ifndef GAME_H
#define GAME_H

#include "raylib.h" 

#define TELA_MENU_PRINCIPAL 0
#define TELA_JOGO 1
#define TELA_TOP_SCORES 2
#define TELA_PERGUNTA_PERFIL 3
#define TELA_REGISTRAR_PERFIL 4
#define TELA_SELECIONAR_PERFIL 5

typedef struct {
    int x;
    int y;
} Posicao;

typedef struct {
    int dx;
    int dy;
} Velocidade;

typedef struct {
    Posicao pos;
    int largura;
    char* simbolo;
} Jogador;

typedef struct {
    Posicao pos;
    Velocidade vel;
    char simbolo;
} Bola;

typedef struct {
    char iniciais[4];
    int recorde;
} Perfil;

typedef struct EstadoJogo {
    int telaLargura;
    int telaAltura;
    int pontuacao;
    int vidas;
    int nivel;
    int timerSpeed;
    int deveSair;
    int telaAtual;
    int cursorMenu;
    Jogador jogador;
    Bola bola;
    void* listaDeBlocos;
    Perfil perfis[10];
    int numPerfis;
    int perfilSelecionado;
    char registroIniciais[4];
    int registroCursor;
} EstadoJogo;

EstadoJogo* criarEstadoInicial(int largura, int altura);
void liberarEstado(EstadoJogo* estado);
void atualizarJogador(EstadoJogo* estado); 
void atualizarBola(EstadoJogo* estado);
void verificarColisoes(EstadoJogo* estado);
void desenharTudo(EstadoJogo* estado, Texture2D logo); 
void carregarNivel(EstadoJogo* estado, int nivel);
void carregarTopScores(EstadoJogo* estado);
void salvarTopScores(EstadoJogo* estado);

#endif