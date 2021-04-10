// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Bibliotecas adicionais
#include "raylib.h"

// Nossas bibliotecas
#include "Headers.h"

// Declaração de constantes
#define MAX_INPUT_CHARS 8
#define ARQ_GRAVACAO "gravacao.bin"
#define ARQ_FASE "fase.bin"

// Funções relacionadas a manipulação de texto e entrada do usuário
int IsAnyKeyPressed() {
    int keyPressed = 0;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = 1;

    return keyPressed;
}

char * string_to_lower (char * string, char * nova_string) {
    int i, tam_string = strlen(string);
    for (i = 0; i < tam_string; i++) {
        nova_string[i] = tolower(string[i]);
    }
    nova_string[i] = '\0';
    return nova_string;
}

// Funções relacionadas a a leitura e gravação em arquivos binários
void escreve_gravacao(char * nome_arquivo, pGRAVACAO gravacao) {
    FILE * arquivo;
    if (!(arquivo = fopen(nome_arquivo, "a+b"))) {
        printf("Erro ao abrir o arquivo de gravações");
    }
    else {
        fwrite(gravacao, sizeof(GRAVACAO), 1, arquivo);
        fclose(arquivo);
    }
}

int nome_unico(char * nome_arquivo, char * nome) {
    FILE * arquivo;
    GRAVACAO gravacao_corrente;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações");
    }
    else {
        while (!feof(arquivo)) {
            fread(&gravacao_corrente, sizeof(GRAVACAO), 1, arquivo);
            if (strcmp(nome, gravacao_corrente.nomejogador) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

int numero_gravacoes(char * nome_arquivo) {
    FILE * arquivo;
    int tamanho = 0;
    GRAVACAO gravacao_corrente;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações");
    }
    else {
        while (!feof(arquivo)) {
            fread(&gravacao_corrente, sizeof(GRAVACAO), 1, arquivo);
            tamanho++;
        }
    }
    return tamanho;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
}

GRAVACAO le_gravacao_por_pos(char * nome_arquivo, int pos) {
    FILE * arquivo;
    GRAVACAO gravacao_lida;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações");
    }
    else {
        fseek(arquivo, sizeof(GRAVACAO)*pos, SEEK_SET);
        fread(&gravacao_lida, sizeof(GRAVACAO), 1, arquivo);
    }
    return gravacao_lida;
}

int pos_por_nomejogador(char * nome_arquivo, char * nomejogador) {
    FILE * arquivo;
    GRAVACAO gravacao_lida;
    int pos = 0;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações");
    }
    else {
        while(!feof(arquivo)) {
            fread(&gravacao_lida, sizeof(GRAVACAO), 1, arquivo);
            if (strcmp(gravacao_lida.nomejogador, nomejogador) == 0) {
                return pos;
            }
            pos++;
        }
    }
    return -1;
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
}

//Função principal MAIN
int main(void) {    
    // Inicia a janela com as dimensões indicadas
    int screen_width = 800;
    int screen_height = 800;
    InitWindow(screen_width, screen_height, "Adventures of Lolo");
    
    // Créditos (return menu)
    const char message[128] = "PRESS ENTER TO RETURN MENU";
    int framesCounter = 0;
    
    // Configurações
    SetTargetFPS(60);
    STATUS status_jogo = {"MENU"};
    
    // Música
    InitAudioDevice();
    Music music = LoadMusicStream("./resources/Songs/Main-theme.mp3");
    music.looping = false;
    PlayMusicStream(music);
    
    // Fonte
    Font Fonte_principal = LoadFont("./resources/fonte.ttf");

    // Texturas
    Texture2D menu_texture = LoadTexture("./resources/Menus/Menu.png");
    Texture2D lolo_texture = LoadTexture("./resources/Menus/Menu_lolo.png"); 
    Texture2D cred_texture = LoadTexture("./resources/Menus/Creditos.png"); 
    Texture2D fundo_texture = LoadTexture("./resources/Menus/Fundo sem nada.png");
    Texture2D mapa_vazio_texture = LoadTexture("./resources/Mapa/Mapa_vazio.png");

    // Variáveis de posicionamento
    int ponto_x_lolo_menu = (screen_width - menu_texture.width)/2 + 45;
    int ponto_y_inic_lolo_menu = (screen_height - menu_texture.height)/2 + 85;
    int desloc_y_lolo_menu = 60;
    PONTO lolo_sel_ponto = {ponto_x_lolo_menu, ponto_y_inic_lolo_menu};
    Vector2 position11 = {275, 265};
    Vector2 position12 = {260, 320};
    Vector2 position2 = {250, 450};
    Vector2 position3 = {210, 500};
    Rectangle textBox = { 255, 385, 300, 50 };
    Vector2 position4 = {textBox.x + 5, textBox.y + 8};
    
    // Declaração das variáveis da caixa de texto com o nome do jogador
    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;
    int nome_n_unico = 0;
    
    //Declaração de Gravação e fase atuais
    GRAVACAO jogo_atual;
    FASE fase_atual;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
    
    // Main game loop
    while (!WindowShouldClose() && strcmp(status_jogo.parte, "SAIR") != 0)    // Detect window close button or ESC key
    {
        UpdateMusicStream(music); //Tocar a musica quando abre o menu
        
        if (strcmp(status_jogo.parte, "MENU") == 0) {
            
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto.y != ponto_y_inic_lolo_menu) {
                lolo_sel_ponto.y -= desloc_y_lolo_menu;
            }
            
            if (IsKeyPressed(KEY_DOWN) && lolo_sel_ponto.y != ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                lolo_sel_ponto.y += desloc_y_lolo_menu;
            }
            
            if (IsKeyPressed(KEY_ENTER)) {
                if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu) {
                    strcpy(status_jogo.parte, "NAME");
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
        
        if (strcmp(status_jogo.parte, "NAME") == 0) {
            
            // Implementação da caixa de texto com o nome do jogador
            // Pega a tecla apertada
            int key = GetCharPressed();

            // Checa se mais de um caracter foi precionado por frame e adiciona o valor ao nome
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)) {
                    name[letterCount] = (char)key;
                    letterCount++;
                }
                
                key = GetCharPressed();  // Pega o próximo caracter se houver
            }
            
            // Checa se foi apertado o delete e nesse caso diminui o tamanho da string
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (letterCount > 0) {
                    letterCount--;
                };
                    
                name[letterCount] = '\0';
            }
            
            // Checa se foi apertado enter, nesse caso salva o nome do jogador se ele tiver colocado um e começa o jogo
            if (IsKeyPressed(KEY_ENTER)) {
                if (letterCount > 0) {
                    strcpy(jogo_atual.ident, "00");
                    strcpy(jogo_atual.totalpts, "00");
                    strcpy(jogo_atual.num_ult_fase, "00");
                    strcpy(jogo_atual.vidas, "05");
                    char string_interm[9];
                    strcpy(jogo_atual.nomejogador, string_to_lower(name, string_interm));
                    
                    if (nome_unico(ARQ_GRAVACAO, jogo_atual.nomejogador)) {
                        strcpy(status_jogo.parte, "TEXT");
<<<<<<< Updated upstream
<<<<<<< Updated upstream
                        //carregar_fase(jogo_atual.num_ult_fase, &fase_atual);
=======
                        carregar_fase(jogo_atual.num_ult_fase, &fase_atual);
>>>>>>> Stashed changes
=======
                        carregar_fase(jogo_atual.num_ult_fase, &fase_atual);
>>>>>>> Stashed changes
                        escreve_gravacao(ARQ_GRAVACAO, &jogo_atual);
                    }
                    else {
                        nome_n_unico = 1;
                    }
                }
            }
            
            BeginDrawing();
            
                ClearBackground(BLACK);

                // Imagem de fundo
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                
                // Texto a cima
                DrawTextEx(Fonte_principal, "Nome do", position11, 50, 1, BLACK);
                DrawTextEx(Fonte_principal, "jogador:", position12, 50, 1, BLACK);

                // Caixa para inserir o nome
                DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, BLACK);
                DrawTextEx(Fonte_principal, name, position4, 40, 1, BLACK);

                // Texto a baixo
                DrawTextEx(Fonte_principal, TextFormat("Caracteres %i de %i", letterCount, MAX_INPUT_CHARS), position2, 25, 1, BLACK);
                if (nome_n_unico) {
                    DrawTextEx(Fonte_principal, "  Insira outro nome", position3, 30, 1, MAROON);
                }
                else {
                    DrawTextEx(Fonte_principal, "Precione Enter para salvar", position3, 20, 1, BLACK);
                }
                
            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "LOAD") == 0) {
            BeginDrawing();
                
                ClearBackground(BLACK);
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                
            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "TEXT") == 0) {         
            
            BeginDrawing();
            
                ClearBackground(BLACK);
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                
                
                
            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "GAME") == 0) {
            BeginDrawing();
            
                ClearBackground(BLACK);
                DrawTexture(mapa_vazio_texture, (screen_width - mapa_vazio_texture.width)/2, (screen_height - mapa_vazio_texture.height)/2, WHITE);
                
            EndDrawing();
        }
        
        if (strcmp(status_jogo.parte, "CRED") == 0) {
            BeginDrawing();
        
                ClearBackground(BLACK);
                DrawTexture(cred_texture, (screen_width - cred_texture.width)/2, (screen_height - cred_texture.height)/2, WHITE);
                
            
                framesCounter++;
                DrawText(TextSubtext(message, 0, framesCounter/5), 230, 550, 20, BLACK);
                             
                
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