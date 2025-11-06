#ifndef GAME_H
#define GAME_H

#define TELA_MENU_PRINCIPAL   0
#define TELA_TOP_SCORES       1
#define TELA_PERGUNTA_PERFIL  2
#define TELA_CARREGAR_PERFIL  3
#define TELA_REGISTRAR_PERFIL 4
#define TELA_JOGO             5
#define TELA_GAME_OVER        6

typedef struct {
    int x;
    int y;
}
Posicao;

typedef struct {
    int dx;
    int dy;
} Velocidade;

typedef struct {
    Posicao pos;
    Velocidade vel;
    char simbolo;
} Bola;

typedef struct {
    Posicao pos;
    int largura;
    char* simbolo;
} Jogador;

typedef struct {
    Posicao pos;
    int largura;
    int resistencia;
    char* simbolo;
}
Bloco;

typedef struct NoBloco {
    Bloco bloco;
    struct NoBloco* proximo;
} NoBloco;

typedef struct {
    char iniciais[4];
    int recorde;
} Perfil;

typedef struct {
    Jogador jogador;
    Bola bola;
    NoBloco* listaDeBlocos;

    int pontuacao;
    int vidas;
    int nivel;
    int timerSpeed;

    int telaLargura;
    int telaAltura;

    int telaAtual;
    int cursorMenu;

    Perfil perfis[10];
    int numPerfis;
    int perfilSelecionado;
    int deveSair;

} EstadoJogo;

EstadoJogo* criarEstadoInicial(int largura, int altura);

void liberarEstado(EstadoJogo* estado);

void carregarNivel(EstadoJogo* estado, int nivel);

void atualizarJogador(EstadoJogo* estado, int tecla);

void atualizarBola(EstadoJogo* estado);

void verificarColisoes(EstadoJogo* estado);

void desenharTudo(EstadoJogo* estado);

void carregarTopScores(EstadoJogo* estado);
void salvarTopScores(EstadoJogo* estado);

#endif