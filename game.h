#ifndef GAME_H
#define GAME_H

#define MAX_PERFIS 10

typedef enum {
    TELA_MENU_PRINCIPAL = 0,
    TELA_JOGO,
    TELA_PAUSE,
    TELA_TOP_SCORES,
    TELA_PERGUNTA_PERFIL,
    TELA_REGISTRAR_PERFIL,
    TELA_SELECIONAR_PERFIL
} TelaJogo;

typedef struct {
    float x, y;
} Vetor2;

typedef struct {
    float dx, dy;
} Velocidade;

typedef struct {
    Vetor2 pos;
    float largura;
    const char* simbolo; 
} Raquete;

typedef struct {
    Vetor2 pos;
    Velocidade vel;
    char simbolo;
} Bola;

typedef struct Bloco {
    int x, y, hp;
    struct Bloco *proximo;
} Bloco;

typedef struct {
    char iniciais[4];
    int recorde;
} Perfil;

typedef struct {
    int telaLargura;
    int telaAltura;
    int timerSpeed;
    Bloco *listaDeBlocos;
    int pontuacao;
    int vidas;
    int nivel;
    TelaJogo telaAtual;
    int cursorMenu;
    Raquete jogador;
    Bola bola;
    int deveSair;

    int numPerfis; 
    Perfil perfis[MAX_PERFIS];
    int perfilSelecionado; 
    int registroCursor;
    char registroIniciais[4];
  
    bool mostrarDicaControle;
    float timerDicaControle;

    int cursorPause; 
} EstadoJogo;

EstadoJogo* criarEstadoInicial(int largura, int altura);

void liberarEstado(EstadoJogo* estado);

void atualizarJogador(EstadoJogo* e);

void atualizarBola(EstadoJogo* e);

void verificarColisoes(EstadoJogo* e);

void desenharTudo(EstadoJogo* e, Texture2D logo);

void carregarNivel(EstadoJogo* e, int nivel);

void carregarTopScores(EstadoJogo* e);

void salvarTopScores(EstadoJogo* e);

void initGame(EstadoJogo* e);

#endif