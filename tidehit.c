#include "raylib.h"
#include "game.h"
#include <stdlib.h> 

#define TELA_LARGURA 800
#define TELA_ALTURA 600

int main(void)
{
    InitWindow(TELA_LARGURA, TELA_ALTURA, "Tide Hit!");

    Texture2D logo = LoadTexture("IMG_3363.jpeg");

    EstadoJogo* estado = criarEstadoInicial(TELA_LARGURA, TELA_ALTURA);
    if (estado == NULL) return 1; 

    SetTargetFPS(60); 
    
    while (!WindowShouldClose() && estado->deveSair == 0) 
    {
        atualizarJogador(estado);
        
        BeginDrawing();
            ClearBackground(BLACK); 
            
            desenharTudo(estado, logo);

        EndDrawing();
    }

    UnloadTexture(logo); 
    liberarEstado(estado); 
    CloseWindow();       

    return 0;
}