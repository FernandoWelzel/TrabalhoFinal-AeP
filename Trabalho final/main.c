#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"


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

int main(void)
{
    int screen_width = 800;
    int screen_height = 600;
    
    STATUS status_jogo = {"MENU"};

    InitWindow(screen_width, screen_height, "Adventures of Lolo");

    SetTargetFPS(60);

    Sound music = LoadSound("./resources/MainTheme.mp3");
    
    Texture2D menu_texture = LoadTexture("./resources/Menu.png");
    Texture2D lolo_texture = LoadTexture("./resources/Lolo-cortado.png"); 
    Texture2D cred_texture = LoadTexture("./resources/Creditos.png"); 
    
    int ponto_x_lolo_menu = (screen_width - menu_texture.width)/2 + 45;
    int ponto_y_inic_lolo_menu = (screen_height - menu_texture.height)/2 + 85;
    int desloc_y_lolo_menu = 60;
    
    int cred_timer = 0;
    
    
    PONTO lolo_sel_ponto = {ponto_x_lolo_menu, ponto_y_inic_lolo_menu};
    
    // Main game loop
    while (!WindowShouldClose() && strcmp(status_jogo.parte, "SAIR") != 0)    // Detect window close button or ESC key
    {
        PlaySound(music);
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
        
                ClearBackground(BLACK);

            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "LOAD") == 0) {
            BeginDrawing();
        
                ClearBackground(BLACK);

            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "CRED") == 0) {
            BeginDrawing();
        
                ClearBackground(BLACK);
                DrawTexture(cred_texture, (screen_width - cred_texture.width)/2, (screen_height - cred_texture.height)/2, WHITE);
                cred_timer += GetFrameTime();
                
                if (cred_timer >= 0.0001) {
                    strcpy(status_jogo.parte, "SAIR");
                    cred_timer = 0;
                }

            EndDrawing();
        }
    }

    return 0;
}
