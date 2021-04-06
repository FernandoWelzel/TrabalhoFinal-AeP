#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#define MAX_FONTS   1

// Estruturas
typedef struct gravacao {
    char identificador;
    char totalpts;
    char ultimafase;
    char vidas;
    char nomejogador[9];
} GRAVACAO;

typedef struct fase {
    char tamanhox;
    char tamanhoy;
} FASE;

typedef struct ponto {
    int x;
    int y;
} PONTO;

typedef struct status {
    char parte[5];
}STATUS;

typedef struct lolo{
    char vidas;
    PONTO ponto;
    int poder[3];
    
} Lolo;


int main(void)
{
    int screen_width = 800;
    int screen_height = 600;
    
    STATUS status_jogo = {"MENU"};

    InitWindow(screen_width, screen_height, "Adventures of Lolo");

    //Iniciar audio device
    InitAudioDevice();
    
    // Creditos (return menu)
    const char message[128] = "PRESS ENTER TO RETURN MENU";
    int framesCounter = 0;
    
    
    SetTargetFPS(60);
    
    //Musica
    Music music = LoadMusicStream("./resources/musiquinha.mp3");
    music.looping = false;
    PlayMusicStream(music);
    
    //Fonte
     Font Fonte_principal = LoadFont("./resources/fonte.ttf");


    
    
    Texture2D menu_texture = LoadTexture("./resources/Menu.png");
    Texture2D lolo_texture = LoadTexture("./resources/Lolo-cortado.png"); 
    Texture2D cred_texture = LoadTexture("./resources/Creditos.png"); 
    Texture2D arvore_texture = LoadTexture("./resources/arvore.png");
    Texture2D fundo_texture = LoadTexture("./resources/Fundo sem nada.png");

    
    int ponto_x_lolo_menu = (screen_width - menu_texture.width)/2 + 45;
    int ponto_y_inic_lolo_menu = (screen_height - menu_texture.height)/2 + 85;
    int desloc_y_lolo_menu = 60;
    Vector2 position={235,200};
    
    
    
    PONTO lolo_sel_ponto = {ponto_x_lolo_menu, ponto_y_inic_lolo_menu};
    
    // Main game loop
    while (!WindowShouldClose() && strcmp(status_jogo.parte, "SAIR") != 0)    // Detect window close button or ESC key
    {
        UpdateMusicStream(music); //Tocar a musica quando abre o menu
        
        if (strcmp(status_jogo.parte, "INIC") == 0) {
            BeginDrawing();
        
                ClearBackground(BLACK);

            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "MENU") == 0) {
            
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto.y != ponto_y_inic_lolo_menu) {
                lolo_sel_ponto.y -= desloc_y_lolo_menu;
            }
            
            if (IsKeyPressed(KEY_DOWN) && lolo_sel_ponto.y != ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                lolo_sel_ponto.y += desloc_y_lolo_menu;
            }
            
            if (IsKeyPressed(KEY_ENTER)) {
                if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu) {
                    strcpy(status_jogo.parte, "GAME");
                }
                else if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu + 1*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "LOAD");
                }
                else if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu + 2*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "CRED");
                }
                else if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "SAIR");
                }
            }
            
            BeginDrawing();
        
                ClearBackground(BLACK);
            
                DrawTexture(menu_texture, (screen_width - menu_texture.width)/2, (screen_height - menu_texture.height)/2, WHITE);
                DrawTexture(lolo_texture, lolo_sel_ponto.x, lolo_sel_ponto.y, WHITE);
           

            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "GAME") == 0) {
            BeginDrawing();
                
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                DrawTextEx(Fonte_principal, "Nome do jogador...", position, 25, 1, BLACK);
                
                ClearBackground(BLACK);
                   
            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "LOAD") == 0) {
            BeginDrawing();
            
                ClearBackground(BLACK);
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                
            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "CRED") == 0) {
            BeginDrawing();
        
                ClearBackground(BLACK);
                DrawTexture(cred_texture, (screen_width - cred_texture.width)/2, (screen_height - cred_texture.height)/2, WHITE);
                
            
                framesCounter++;
                DrawText(TextSubtext(message, 0, framesCounter/5), 230, 445, 20, BLACK);
                             
                
                if(IsKeyPressed(KEY_ENTER)){
                    framesCounter = 0;
                    strcpy(status_jogo.parte,"MENU");
                }


            EndDrawing();
        }
    }
    CloseAudioDevice();

    return 0;
}