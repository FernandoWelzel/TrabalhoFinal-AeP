// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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
}

FASE le_fase_por_pos(char * nome_arquivo, int pos) {
    FILE * arquivo;
    FASE fase_lida;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de fases");
    }
    else {
        fseek(arquivo, sizeof(FASE)*pos, SEEK_SET);
        fread(&fase_lida, sizeof(FASE), 1, arquivo);
    }
    return fase_lida;
}

// Funções de movimentação

// Função que recebe uma posição em uma fase e retorna se o bloco nessa posição é imóvel
int bloco_eh_imovel(FASE fase, int x, int y) {
    switch (fase.elementos[x][y]) {
        case 'T':
        case 'P':
            return 1;
        default:
            return 0;
    }
}

// Função que atuliza a posição do lolo no jogo baseado na sua posição atual, nos blocos a sua volta e na entrada do usuário
void atualiza_pos_lolo(pPONTO Ponto_lolo, FASE fase) {
    if (IsKeyDown(KEY_UP)) {
        if (!bloco_eh_imovel(fase, ((Ponto_lolo->y - 1)/48), ((Ponto_lolo->x)/48)) &&
            !bloco_eh_imovel(fase, ((Ponto_lolo->y - 1)/48), ((Ponto_lolo->x + 47)/48)) &&
            (Ponto_lolo->y - 1) > 0) {
            Ponto_lolo->y -= 2;
        }
    }

    if (IsKeyDown(KEY_DOWN)) {
        if (!bloco_eh_imovel(fase, ((Ponto_lolo->y + 49)/48), ((Ponto_lolo->x)/48)) &&
        !bloco_eh_imovel(fase, ((Ponto_lolo->y + 49)/48), ((Ponto_lolo->x + 47)/48)) &&
        (Ponto_lolo->y + 49) < 528) {
            Ponto_lolo->y += 2;
        }
    }

    if (IsKeyDown(KEY_RIGHT)) {
        if (!bloco_eh_imovel(fase, ((Ponto_lolo->y)/48), ((Ponto_lolo->x + 49)/48)) &&
        !bloco_eh_imovel(fase, ((Ponto_lolo->y + 47)/48), ((Ponto_lolo->x + 49)/48)) &&
        (Ponto_lolo->x + 49) < 528) {
            Ponto_lolo->x += 2;
        }
    }

    if (IsKeyDown(KEY_LEFT)) {
        if (!bloco_eh_imovel(fase, ((Ponto_lolo->y)/48), ((Ponto_lolo->x - 1)/48)) &&
        !bloco_eh_imovel(fase, ((Ponto_lolo->y + 47)/48), ((Ponto_lolo->x - 1)/48)) &&
        (Ponto_lolo->x - 1) > 0) {
            Ponto_lolo->x -= 2;
        }
    }
}

//Função principal MAIN
int main(void) {
    // Inicia a janela com as dimensões indicadas
    int screen_width = 800;
    int screen_height = 800;
    InitWindow(screen_width, screen_height, "Adventures of Lolo");

    // Créditos (return menu)
    const char message[28] = "PRESS ENTER TO RETURN MENU";
    int framesCounter = 0;
    int framesCounter2 = 0;

    // Configurações
    SetTargetFPS(60);
    STATUS status_jogo = {"MENU"};

    // Música
    InitAudioDevice();
    Music music = LoadMusicStream("./resources/Songs/Main-theme.mp3");
    music.looping = true;
    PlayMusicStream(music);

    // Fonte
    Font Fonte_principal = LoadFont("./resources/fonte.ttf");

    // Texturas
    Texture2D menu_texture = LoadTexture("./resources/Menus/Menu.png");
    Texture2D lolo_texture = LoadTexture("./resources/Menus/Menu_lolo.png");
    Texture2D cred_texture = LoadTexture("./resources/Menus/Creditos.png");
    Texture2D fundo_texture = LoadTexture("./resources/Menus/Fundo sem nada.png");
    Texture2D mapa_vazio_texture = LoadTexture("./resources/Mapa/Mapa_vazio.png");
    Texture2D espaco_livre_texture = LoadTexture("./resources/Blocos/Livre.png");
    Texture2D agua_texture = LoadTexture("./resources/Blocos/Agua.png");
    Texture2D pedra_texture = LoadTexture("./resources/Blocos/Pedra.png");
    Texture2D arvore_texture = LoadTexture("./resources/Blocos/Tree.png");
    Texture2D bloco_esquerda_texture = LoadTexture("./resources/Blocos/Lateral-esquerda.png");
    Texture2D lolo_F_texture = LoadTexture("./resources/Lolo/Lolo-F.png");

    // Variáveis de posicionamento
    int ponto_x_lolo_menu = (screen_width - menu_texture.width)/2 + 45;
    int ponto_x_lolo_load = (screen_width - menu_texture.width)/2 + 40;
    int ponto_y_inic_lolo_menu = (screen_height - menu_texture.height)/2 + 85;
    int ponto_y_inic_lolo_load = (screen_height - menu_texture.height)/2 + 75;
    int desloc_y_lolo_menu = 60;
    int desloc_y_lolo_load = 50;
    PONTO lolo_sel_ponto = {ponto_x_lolo_menu, ponto_y_inic_lolo_menu};
    Vector2 position11 = {275, 265};
    Vector2 position12 = {260, 320};
    Vector2 position2 = {250, 450};
    Vector2 position3 = {210, 500};
    Vector2 position5 = {210, 250};
    Rectangle textBox = {255, 385, 300, 50};
    Vector2 position4 = {textBox.x + 5, textBox.y + 8};
    Vector2 position6 = {215, 300};
    int BordaMapax = ((screen_width - mapa_vazio_texture.width)/2) + 45;
    int BordaMapay = ((screen_height - mapa_vazio_texture.height)/2) + 96;
    PONTO pos_lolo_game;

    // Declaração das variáveis da caixa de texto com o nome do jogador
    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;
    int nome_n_unico = 0;


    // Declaração de Gravação e fase atuais
    GRAVACAO jogo_atual;
    FASE fase_atual;

    // Variáveis para iteração
    int i;
    int j;

    // Main game loop
    while (!WindowShouldClose() && strcmp(status_jogo.parte, "SAIR") != 0)    // Detect window close button or ESC key
    {
        UpdateMusicStream(music); //Tocar a musica quando abre o menu
        /*
        Nessa parte do código é definido a parte do jogo que deve ser
        exibida na tela. Para isso fazemos comparações com a string que
        quarda a parte do jogo que estamos.
        */
        
        // MENU
        if (strcmp(status_jogo.parte, "MENU") == 0) {
            
           // Detecta se o usuário apertou uma das setas para se mover no menu e muda a posição do lolo 
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto.y != ponto_y_inic_lolo_menu) {
                lolo_sel_ponto.y -= desloc_y_lolo_menu;
            }

            if (IsKeyPressed(KEY_DOWN) && lolo_sel_ponto.y != ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                lolo_sel_ponto.y += desloc_y_lolo_menu;
            }
            
            // Detectas se o usuário apertou enter, indicando que ele quer entrar em outra área. Dessa maneira o direciona baseado na posição do lolo do menu
            if (IsKeyPressed(KEY_ENTER)) {
                if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu) {
                    strcpy(status_jogo.parte, "NAME");
                }
                else if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu + 1*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "LOAD");
                    lolo_sel_ponto.y = ponto_y_inic_lolo_load;
                }
                else if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu + 2*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "CRED");
                }
                else if (lolo_sel_ponto.y == ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "SAIR");
                }
            }
            
            // Mostra na tela a imagem do menu e o lolo que indica para que área o usuário quer ir
            BeginDrawing();

                ClearBackground(BLACK);

                DrawTexture(menu_texture, (screen_width - menu_texture.width)/2, (screen_height - menu_texture.height)/2, WHITE);
                DrawTexture(lolo_texture, lolo_sel_ponto.x, lolo_sel_ponto.y, WHITE);


            EndDrawing();
        }

        // NAME (Início de um novo jogo pegando o nome do usuário)
        if (strcmp(status_jogo.parte, "NAME") == 0) {

            // Implementação da caixa de texto para o jogador inserir o nome
            
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

            /* 
            Checa se foi apertado enter, nesse caso salva o nome do jogador se ele tiver
            colocado um nome válido e começa o jogo com parâmetros iniciais, mudando para
            uma tela com o texto inicial da fase.
            */
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
                        fase_atual = le_fase_por_pos(ARQ_FASE , atoi(jogo_atual.num_ult_fase));
                        escreve_gravacao(ARQ_GRAVACAO, &jogo_atual);
                        pos_lolo_game.x = fase_atual.pos_i_jogador.x*48;
                        pos_lolo_game.y = fase_atual.pos_i_jogador.y*48 ;
                    }
                    else {
                        nome_n_unico = 1;
                    }
                }
            }
            
            // Mostra a tela para escrever o nome do jogador
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
         int i;
            ClearBackground(BLACK);
            DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
            DrawTextEx(Fonte_principal, "Escolha um jogo salvo", position5, 24, 2, BLACK);
            DrawTexture(lolo_texture, ponto_x_lolo_load, lolo_sel_ponto.y, WHITE);


                for(i=1; i <= 4/*numero de arquivos*/; i++){
                   DrawText("CAVALOLO"/*nome do jogador*/, ponto_x_lolo_load + 60, ponto_y_inic_lolo_load + 5 + ((i-1)*58), 35, BLACK);
                   DrawText("-Fase 3"/*fase do save*/, ponto_x_lolo_load + 250, ponto_y_inic_lolo_load + 5 + ((i-1)*58), 28, BLACK);

               }
                for (i=4/*numero de arquivos*/; i<5; i++ ){
                      DrawText("Memoria vazia", ponto_x_lolo_load + 60, ponto_y_inic_lolo_load + (i*58), 35, LIGHTGRAY);
                }

                if (IsKeyPressed(KEY_UP) && lolo_sel_ponto.y != ponto_y_inic_lolo_load) {
                    lolo_sel_ponto.y -= 58;
                }

                if ((IsKeyPressed(KEY_DOWN)) && (lolo_sel_ponto.y != ponto_y_inic_lolo_load + (4 * 58))) {
                    lolo_sel_ponto.y += 58;
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    if ((lolo_sel_ponto.y == ponto_y_inic_lolo_load) && (4/*numero_arquivos*/>=1)) {
                            strcpy(status_jogo.parte, "GAME");
                    }
                    else if ((lolo_sel_ponto.y == ponto_y_inic_lolo_load + 1*58) && (4/*numero_arquivos*/>=2)) {
                        strcpy(status_jogo.parte, "GAME");
                    }
                    else if ((lolo_sel_ponto.y == ponto_y_inic_lolo_load + 2*58) && (4/*numero_arquivos*/>=3)) {
                        strcpy(status_jogo.parte, "GAME");
                    }
                    else if ((lolo_sel_ponto.y == ponto_y_inic_lolo_load + 3*58) && (4/*numero_arquivos*/>=4)) {
                        strcpy(status_jogo.parte, "GAME");
                    }
                    else if ((lolo_sel_ponto.y == ponto_y_inic_lolo_load + 4*58) && (4/*numero_arquivos*/>=5)) {
                        strcpy(status_jogo.parte, "GAME");
                    }
                }




            EndDrawing();
        }
        
        // TEXT (Tela que exibe o texto inicial do jogo)
        if (strcmp(status_jogo.parte, "TEXT") == 0) {
            
            // Faz a contagem do número de frames, para depois de 3 segundo sair da tela de texto
            framesCounter2++;
            if (framesCounter2/60 > 3) {
                strcpy(status_jogo.parte, "GAME");
            }
            
            // Mostra a tela com o fundo padrão e a mensagem de texto que está em fase_atual.texto_inic
            BeginDrawing();

                ClearBackground(BLACK);
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);

                DrawTextEx(Fonte_principal, fase_atual.texto_inic, position6, 30, 1, BLACK);

            EndDrawing();
        }

        // GAME (Jogo em si)
        if (strcmp(status_jogo.parte, "GAME") == 0) {
            
            // Atualiza a posição do lolo baseado na sua posição atual, na tecla que o usuário preciona e nos blocos a sua volta
            atualiza_pos_lolo(&pos_lolo_game, fase_atual);
            
            // Mostra a tela do jogo baseado nos blocos contidos em fase_atual.elementos e na posição do lolo
            BeginDrawing();
                
                // Mostra um mapa vazio
                ClearBackground(BLACK);
                DrawTexture(mapa_vazio_texture, (screen_width - mapa_vazio_texture.width)/2, (screen_height - mapa_vazio_texture.height)/2, WHITE);

                // Imprime cada um dos quadrados do mapa, no qual o lolo andará por cima, que estão em fase_atual.elementos
                for (i = 0; i < 11; i++) {
                    for (j = 0; j < 11; j++) {
                        /*
                        // Não funciona por que está gravando por baixo do outro (teria que arrumar a textura da maioria dos blocos também)
                        if (i == 0 && fase_atual.elementos[j][i] != 'A') {
                            DrawTexture(bloco_esquerda_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                        }*/
                        switch (fase_atual.elementos[j][i]) {
                            case 'L':
                                DrawTexture(espaco_livre_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                break;
                            case 'A':
                                DrawTexture(agua_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                break;
                            case 'P':
                                DrawTexture(pedra_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                break;
                            case 'T':
                                DrawTexture(arvore_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                break;
                        }
                    }
                }

                // Imprime o lolo baseado em sua posição atualizada
                DrawTexture(lolo_F_texture, BordaMapax + pos_lolo_game.x, BordaMapay + pos_lolo_game.y, WHITE);

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
