#ifndef GAME_H
#define GAME_H

#include "raylib.h" 

#define MAX_PERFIS 10

#define BLOCO_LINHAS 5
#define BLOCO_COLUNAS 10

#define TELA_MENU_PRINCIPAL 0
#define TELA_JOGO 1
#define TELA_TOP_SCORES 2
#define TELA_PERGUNTA_PERFIL 3
#define TELA_REGISTRAR_PERFIL 4
#define TELA_SELECIONAR_PERFIL 5
#define TELA_PAUSE 6

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

typedef struct Bloco {
    Rectangle rect;
    bool ativo;
    Color cor;
    int hp;
    struct Bloco *proximo;
} Bloco;

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
    Bloco *listaDeBlocos;
    
    int blocosAtivos;
    int blocosParaRespawnar;
    float timerAceleracao;
    float timerRespawn;

    Perfil perfis[MAX_PERFIS];
    int numPerfis;
    int perfilSelecionado;
    char registroIniciais[4];
    int registroCursor;
    bool mostrarDicaControle;
    float timerDicaControle;
    int cursorPause;
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
void initGame(EstadoJogo* e);
void respawnarBlocoAleatorio(EstadoJogo* e);

#endif