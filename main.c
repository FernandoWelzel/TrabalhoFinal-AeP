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
int bloco_eh_imovel(pFASE fase, char orientacao, int y, int x) {    
    switch (fase->elementos[y/48][x/48]) {
        case 'T':
        case 'P':
            return 1;
        default:
            return 0;
    }
}

int testar_pontos_imoveis(pFASE fase, char orientacao, PONTO ponto1, PONTO ponto2);

int eh_inimigo_imovel (pFASE fase, char orientacao, int y, int x) {
    int i, retorno = 0;
    PONTO ponto1, ponto2;
    
    for (i = 0; i < fase->num_inimigos; i++) {
        if ((fase->inimigos + i)->bola == 'S') {
            if (x - (fase->inimigos + i)->posicao.x > 0 &&  x -(fase->inimigos + i)->posicao.x < 48 &&
                y - (fase->inimigos + i)->posicao.y > 0 &&  y - (fase->inimigos + i)->posicao.y < 48) {
                if (orientacao == 'U') {
                    ponto1.x = (fase->inimigos + i)->posicao.x; ponto1.y = (fase->inimigos + i)->posicao.y - 1;
                    ponto2.x = (fase->inimigos + i)->posicao.x + 47; ponto2.y = (fase->inimigos + i)->posicao.y - 1;
                    
                    if (testar_pontos_imoveis(fase, 'U', ponto1, ponto2) && ponto1.y > 0) {
                        (fase->inimigos + i)->posicao.y -= 2;
                        retorno = 0;
                    }
                    else {
                        retorno = 1;
                    }
                }
                else if (orientacao == 'D') {
                    ponto1.x = (fase->inimigos + i)->posicao.x; ponto1.y = (fase->inimigos + i)->posicao.y + 48;
                    ponto2.x = (fase->inimigos + i)->posicao.x + 47; ponto2.y = (fase->inimigos + i)->posicao.y + 48;
                    
                    if (testar_pontos_imoveis(fase, 'D', ponto1, ponto2) && ponto1.y < 528) {
                        (fase->inimigos + i)->posicao.y += 2;
                        retorno = 0;
                    }
                    else {
                        retorno = 1;
                    }
                }
                else if (orientacao == 'L') {
                    ponto1.x = (fase->inimigos + i)->posicao.x - 1; ponto1.y = (fase->inimigos + i)->posicao.y;
                    ponto2.x = (fase->inimigos + i)->posicao.x - 1; ponto2.y = (fase->inimigos + i)->posicao.y + 47;
                    
                    if (testar_pontos_imoveis(fase, 'L', ponto1, ponto2) && ponto1.x > 0) {
                        (fase->inimigos + i)->posicao.x -= 2;
                        retorno = 0;
                    }
                    else {
                        retorno = 1;
                    }
                }
                else if (orientacao == 'R') {
                    ponto1.x = (fase->inimigos + i)->posicao.x + 48; ponto1.y = (fase->inimigos + i)->posicao.y;
                    ponto2.x = (fase->inimigos + i)->posicao.x + 48; ponto2.y = (fase->inimigos + i)->posicao.y + 47;
                    
                    if (testar_pontos_imoveis(fase, 'R', ponto1, ponto2) && ponto1.x < 528) {
                        (fase->inimigos + i)->posicao.x += 2;
                        retorno = 0;
                    }
                    else {
                        retorno = 1;
                    }
                }
            }
        }
        else if ((fase->inimigos + i)->bola == 'N') {
            if (x - (fase->inimigos + i)->posicao.x > 0 &&  x -(fase->inimigos + i)->posicao.x < 48 &&
                y - (fase->inimigos + i)->posicao.y > 0 &&  y - (fase->inimigos + i)->posicao.y < 48) {
                     retorno = 1;
            }
        }
    }
    
    return retorno;
}

int testar_pontos_imoveis(pFASE fase, char orientacao, PONTO ponto1, PONTO ponto2) {
    if (!bloco_eh_imovel(fase, orientacao, ponto1.y, ponto1.x) &&
        !bloco_eh_imovel(fase, orientacao, ponto2.y, ponto2.x) &&
        !eh_inimigo_imovel(fase, orientacao, ponto1.y, ponto1.x) &&
        !eh_inimigo_imovel(fase, orientacao, ponto2.y, ponto2.x)) {
        return 1;
    }
    else {
        return 0;
    }
}

// Função que atuliza a posição do lolo no jogo baseado na sua posição atual, nos blocos a sua volta e na entrada do usuário
void atualiza_pos_lolo(pLOLO lolo, pFASE fase) {
    // Declaração dos pontos a serem testados para cada caso
    PONTO ponto1, ponto2;

    if (IsKeyDown(KEY_UP)) {
        lolo->direcao = 'U';
        ponto1.x = (lolo->posicao.x); ponto1.y = (lolo->posicao.y - 1);
        ponto2.x = (lolo->posicao.x + 47); ponto2.y = (lolo->posicao.y - 1);
        
        if (testar_pontos_imoveis(fase, 'U', ponto1, ponto2) && ponto1.y > 0) {
            lolo->posicao.y -= 2;
        }
    }

    if (IsKeyDown(KEY_DOWN)) {
        lolo->direcao = 'D';
        ponto1.x = (lolo->posicao.x); ponto1.y = (lolo->posicao.y + 49);
        ponto2.x = (lolo->posicao.x + 47); ponto2.y = (lolo->posicao.y + 49);
        
        if (testar_pontos_imoveis(fase, 'D', ponto1, ponto2) && ponto1.y < 528) {
            lolo->posicao.y += 2;
        }
    }

    if (IsKeyDown(KEY_RIGHT)) {
        lolo->direcao = 'R';
        ponto1.x = (lolo->posicao.x + 49); ponto1.y = (lolo->posicao.y);
        ponto2.x = (lolo->posicao.x + 49); ponto2.y = (lolo->posicao.y + 47);
        
        if (testar_pontos_imoveis(fase, 'R', ponto1, ponto2) && ponto1.x < 528) {
            lolo->posicao.x += 2;
        }
    }

    if (IsKeyDown(KEY_LEFT)) {
        lolo->direcao = 'L';
        ponto1.x = (lolo->posicao.x - 1); ponto1.y = (lolo->posicao.y);
        ponto2.x = (lolo->posicao.x - 1); ponto2.y = (lolo->posicao.y + 47);
        
        if (testar_pontos_imoveis(fase, 'L', ponto1, ponto2) && ponto1.x > 0) {
            lolo->posicao.x -= 2;
        }
    }
}

char atualiza_pos_tiro(pTIRO Ptiro, pFASE fase) {
    PONTO ponto1, ponto2;
    
    if (Ptiro->direcao == 'R') {
        ponto1.x = (Ptiro->posicao.x + 43); ponto1.y = (Ptiro->posicao.y);
        ponto2.x = (Ptiro->posicao.x + 43); ponto2.y = (Ptiro->posicao.y + 30);
        
        if (testar_pontos_imoveis(fase, 'R', ponto1, ponto2) && ponto1.x < 528) {
            Ptiro->posicao.x += 8;
            return 'L';
        }
        else {
            return 'B';
        }
    }
    if (Ptiro->direcao == 'L') {
        ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y);
        ponto2.x = (Ptiro->posicao.x); ponto2.y = (Ptiro->posicao.y + 30);
        
        if (testar_pontos_imoveis(fase, 'L', ponto1, ponto2) && ponto1.x > 0) {
            Ptiro->posicao.x -= 8;
            return 'L';
        }
        else {
            return 'B';
        }
    }
    if (Ptiro->direcao == 'U') {
        ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y);
        ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y);
        
        if (testar_pontos_imoveis(fase, 'U', ponto1, ponto2) && ponto1.y > 0) {
            Ptiro->posicao.y -= 8;
            return 'L';
        }
        else {
            return 'B';
        }
    }
    if (Ptiro->direcao == 'D') {
        ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y + 45);
        ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y + 45);
        
        if (testar_pontos_imoveis(fase, 'D', ponto1, ponto2) && ponto1.y < 528) {
            Ptiro->posicao.y += 8;
            return 'L';
        }
        else {
            return 'B';
        }
    }
}

PONTO pos_tiro_bateu(pTIRO Ptiro, pFASE fase) {
    PONTO retorno, ponto1, ponto2;
    
    if (Ptiro->direcao == 'R') {
        ponto1.x = (Ptiro->posicao.x + 43); ponto1.y = (Ptiro->posicao.y);
        ponto2.x = (Ptiro->posicao.x + 43); ponto2.y = (Ptiro->posicao.y + 30);
        
        if (bloco_eh_imovel(fase, 'R', ponto1.y, ponto1.x)) {
            return ponto1;
        }
        else if (bloco_eh_imovel(fase, 'R', ponto2.y, ponto2.x)) {
            return ponto2;
        }
        else if (eh_inimigo_imovel(fase, 'R', ponto1.y, ponto1.x)) {
            return ponto1;
        }
        else if (eh_inimigo_imovel(fase, 'R', ponto2.y, ponto2.x)) {
            return ponto2;
        }
        else if ((Ptiro->posicao.x + 43) > 528) {
            retorno.x = -1;
            retorno.y = -1;
            return retorno;
        }
    }
    
    if (Ptiro->direcao == 'L') {
        ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y);
        ponto2.x = (Ptiro->posicao.x); ponto2.y = (Ptiro->posicao.y + 30);
        
        if (bloco_eh_imovel(fase, 'L', ponto1.y, ponto1.x)) {
            return ponto1;
        }
        else if (bloco_eh_imovel(fase, 'L', ponto2.y, ponto2.x)) {
            return ponto2;
        }
        else if (eh_inimigo_imovel(fase, 'L', ponto1.y, ponto1.x)) {
            return ponto1;
        }
        else if (eh_inimigo_imovel(fase, 'L', ponto2.y, ponto2.x)) {
            return ponto2;
        }
        else if ((Ptiro->posicao.x) < 528) {
            retorno.x = -1;
            retorno.y = -1;
            return retorno;
        }
    }
    
    if (Ptiro->direcao == 'U') {
        ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y);
        ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y);
        
        if (bloco_eh_imovel(fase, 'U', ponto1.y, ponto1.x)) {
            return ponto1;
        }
        else if (bloco_eh_imovel(fase, 'U', ponto2.y, ponto2.x)) {
            return ponto2;
        }
        else if (eh_inimigo_imovel(fase, 'U', ponto1.y, ponto1.x)) {
            return ponto1;
        }
        else if (eh_inimigo_imovel(fase, 'U', ponto2.y, ponto2.x)) {
            return ponto2;
        }
        else if ((Ptiro->posicao.y) < 0) {
            retorno.x = -1;
            retorno.y = -1;
            return retorno;
        }
    }
    
    if (Ptiro->direcao == 'D') {
        ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y + 45);
        ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y + 45);
        
        if (bloco_eh_imovel(fase, 'D', ponto1.y, ponto1.x)) {
            return ponto1;
        }
        else if (bloco_eh_imovel(fase, 'D', ponto2.y, ponto2.x)) {
            return ponto2;
        }
        else if (eh_inimigo_imovel(fase, 'D', ponto1.y, ponto1.x)) {
            return ponto1;
        }
        else if (eh_inimigo_imovel(fase, 'D', ponto2.y, ponto2.x)) {
            return ponto2;
        }
        else if ((Ptiro->posicao.y) > 528) {
            retorno.x = -1;
            retorno.y = -1;
            return retorno;
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
    char message[28] = "PRESS ENTER TO RETURN MENU";
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
    Texture2D bau_texture = LoadTexture("./resources/Blocos/Bau.png");
    Texture2D bau_aberto_cheio_texture = LoadTexture("./resources/Blocos/Bau_aberto_cheio.png");
    Texture2D bau_aberto_vazio_texture = LoadTexture("./resources/Blocos/Bau_aberto_vazio.png");
    Texture2D porta_fechada_texture = LoadTexture("./resources/Blocos/Porta_fechada.png");
    Texture2D porta_aberta_texture = LoadTexture("./resources/Blocos/Porta_aberta.png");
    Texture2D coracao_texture = LoadTexture("./resources/Blocos/Coracao.png");
    //Texture2D bloco_esquerda_texture = LoadTexture("./resources/Blocos/Lateral-esquerda.png");
    Texture2D lolo_D_texture = LoadTexture("./resources/Lolo/Lolo-D.png");
    Texture2D lolo_R_texture = LoadTexture("./resources/Lolo/Lolo-R.png");
    Texture2D lolo_L_texture = LoadTexture("./resources/Lolo/Lolo-L.png");
    Texture2D lolo_U_texture = LoadTexture("./resources/Lolo/Lolo-U.png");
    Texture2D inimigo_texture = LoadTexture("./resources/Inimigos/Larva.png");
    Texture2D tiro_L_R_texture = LoadTexture("./resources/Poderes/tiro_L_R.png");
    Texture2D tiro_U_D_texture = LoadTexture("./resources/Poderes/tiro_U_D.png");
    Texture2D ovo_texture = LoadTexture("./resources/Inimigos/ovo.png");

    // Variáveis de posicionamento
    int ponto_x_lolo_menu = (screen_width - menu_texture.width)/2 + 45;
    int ponto_x_lolo_load = (screen_width - menu_texture.width)/2 + 40;
    int ponto_y_inic_lolo_menu = (screen_height - menu_texture.height)/2 + 85;
    int ponto_y_inic_lolo_load = (screen_height - menu_texture.height)/2 + 75;
    int desloc_y_lolo_menu = 60;
    int desloc_y_lolo_load = 50;
    PONTO lolo_sel_ponto_menu = {ponto_x_lolo_menu, ponto_y_inic_lolo_menu};
    PONTO lolo_sel_ponto_load = {ponto_x_lolo_menu, ponto_y_inic_lolo_load};
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
    Vector2 position_num_especiais = {BordaMapax + 585, 300};

    // Declaração das variáveis da caixa de texto com o nome do jogador
    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;
    int nome_n_unico = 0;


    // Declaração de Gravação e fase atuais
    GRAVACAO jogo_atual;
    FASE fase_atual;
    LOLO lolo_atual;
    
    // Variáveis para iteração
    int i;
    int j;
    
    // Variáveis estilo booleanas
    char bau_cheio = 'S';
    
    // Tiro
    TIRO tiro_atual;
    tiro_atual.posicao.x = 0;
    tiro_atual.posicao.y = 0;
    tiro_atual.mostrar = 'N';
    PONTO pos_tiro_bateu_atual;
    
    // Gravações jogos antigos
    GRAVACAO gravacoes_salvas[5];
    
    int numero_de_gravacoes = numero_gravacoes(ARQ_GRAVACAO);
    
    // Main game loop
    while (!WindowShouldClose() && strcmp(status_jogo.parte, "SAIR") != 0)    // Detect window close button or ESC key
    {
        UpdateMusicStream(music); //Tocar a musica quando abre o menu
        /*
        Nessa parte do código é definido a parte do jogo que deve ser
        exibida na tela. Para isso fazemos comparações com a string que
        quarda a parte do jogo que estamos.
        */

        if (strcmp(status_jogo.parte, "MENU") == 0) {
            
           // Detecta se o usuário apertou uma das setas para se mover no menu e muda a posição do lolo 
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto_menu.y != ponto_y_inic_lolo_menu) {
                lolo_sel_ponto_menu.y -= desloc_y_lolo_menu;
            }

            if (IsKeyPressed(KEY_DOWN) && lolo_sel_ponto_menu.y != ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                lolo_sel_ponto_menu.y += desloc_y_lolo_menu;
            }
            
            // Detectas se o usuário apertou enter, indicando que ele quer entrar em outra área. Dessa maneira o direciona baseado na posição do lolo do menu
            if (IsKeyPressed(KEY_ENTER)) {
                if (lolo_sel_ponto_menu.y == ponto_y_inic_lolo_menu) {
                    strcpy(status_jogo.parte, "NAME");
                }
                else if (lolo_sel_ponto_menu.y == ponto_y_inic_lolo_menu + 1*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "LOAD");
                    for (i = 0; i < 5; i++) {
                        gravacoes_salvas[i] = le_gravacao_por_pos(ARQ_GRAVACAO, i);
                    }                    
                }
                else if (lolo_sel_ponto_menu.y == ponto_y_inic_lolo_menu + 2*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "CRED");
                }
                else if (lolo_sel_ponto_menu.y == ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                    strcpy(status_jogo.parte, "SAIR");
                }
            }
            
            // Mostra na tela a imagem do menu e o lolo que indica para que área o usuário quer ir
            BeginDrawing();

                ClearBackground(BLACK);

                DrawTexture(menu_texture, (screen_width - menu_texture.width)/2, (screen_height - menu_texture.height)/2, WHITE);
                DrawTexture(lolo_texture, lolo_sel_ponto_menu.x, lolo_sel_ponto_menu.y, WHITE);


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
                        lolo_atual.posicao.x = fase_atual.pos_i_jogador.x*48;
                        lolo_atual.posicao.y = fase_atual.pos_i_jogador.y*48;
                        lolo_atual.direcao = 'D';
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
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto_load.y > ponto_y_inic_lolo_load) {
                lolo_sel_ponto_load.y -= 58;
            }
            
            if ((IsKeyPressed(KEY_DOWN)) && (lolo_sel_ponto_load.y < ponto_y_inic_lolo_load + (4 * 58))) {
                lolo_sel_ponto_load.y += 58;
            }
            
            if (IsKeyPressed(KEY_ENTER)) {
                for (i = 0; i < 5; i++) {
                    if ((lolo_sel_ponto_load.y == ponto_y_inic_lolo_load + i*58)) { // Número_arquivos
                        strcpy(jogo_atual.ident, gravacoes_salvas[i].ident);
                        strcpy(jogo_atual.totalpts, gravacoes_salvas[i].totalpts);
                        strcpy(jogo_atual.num_ult_fase, gravacoes_salvas[i].num_ult_fase);
                        strcpy(jogo_atual.vidas, gravacoes_salvas[i].vidas);
                        strcpy(jogo_atual.nomejogador, gravacoes_salvas[i].nomejogador);
                        strcpy(status_jogo.parte, "TEXT");
                        fase_atual = le_fase_por_pos(ARQ_FASE , atoi(jogo_atual.num_ult_fase));
                        lolo_atual.posicao.x = fase_atual.pos_i_jogador.x*48;
                        lolo_atual.posicao.y = fase_atual.pos_i_jogador.y*48;
                        lolo_atual.direcao = 'D';
                    }
                }
            }
            
            if (IsKeyPressed(KEY_Q)) {
                strcpy(status_jogo.parte, "MENU");
            }
            
            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                DrawTextEx(Fonte_principal, "Escolha um jogo salvo", position5, 24, 2, BLACK);                
                DrawTexture(lolo_texture, ponto_x_lolo_load, lolo_sel_ponto_load.y, WHITE);
                
                for (i = 0; i < 5; i++) {
                    DrawText(gravacoes_salvas[i].nomejogador, ponto_x_lolo_load + 60, ponto_y_inic_lolo_load + 58*i, 40, BLACK);
                    DrawText(gravacoes_salvas[i].num_ult_fase, ponto_x_lolo_load + 300, ponto_y_inic_lolo_load + 58*i, 40, BLACK);
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
            atualiza_pos_lolo(&lolo_atual, &fase_atual);
            
            if (tiro_atual.mostrar == 'S') {
                if (atualiza_pos_tiro(&tiro_atual, &fase_atual) == 'B') {
                    tiro_atual.mostrar = 'N';
                    pos_tiro_bateu_atual = pos_tiro_bateu(&tiro_atual, &fase_atual);
                    printf("%d %d\n", pos_tiro_bateu_atual.x, pos_tiro_bateu_atual.y);
                    if (pos_tiro_bateu_atual.x != -1) {
                        for (i = 0; i < fase_atual.num_inimigos; i++) {
                            printf("%d %d\n", fase_atual.inimigos[i].posicao.x, fase_atual.inimigos[i].posicao.y);
                            if (pos_tiro_bateu_atual.x - fase_atual.inimigos[i].posicao.x > 0 &&  pos_tiro_bateu_atual.x - fase_atual.inimigos[i].posicao.x < 48 &&
                                pos_tiro_bateu_atual.y - fase_atual.inimigos[i].posicao.y > 0 &&  pos_tiro_bateu_atual.y - fase_atual.inimigos[i].posicao.y < 48) {
                                fase_atual.inimigos[i].bola = 'S';
                                printf("Passou");
                            }
                        }
                    }
                }
            }        
            
            // Compara se a posição do lolo é a mesma de um coração, para ele poder pegar o coração
            if (fase_atual.elementos[lolo_atual.posicao.y/48][lolo_atual.posicao.x/48] == 'C') {
                fase_atual.elementos[lolo_atual.posicao.y/48][lolo_atual.posicao.x/48] = 'L';
                fase_atual.num_coracoes--;
                itoa(atoi(fase_atual.num_especiais) + 1, fase_atual.num_especiais, 10);
            }
            else if (fase_atual.elementos[(lolo_atual.posicao.y + 47)/48][lolo_atual.posicao.x/48] == 'C') {
                fase_atual.elementos[(lolo_atual.posicao.y + 47)/48][lolo_atual.posicao.x/48] = 'L';
                fase_atual.num_coracoes--;
                itoa(atoi(fase_atual.num_especiais) + 1, fase_atual.num_especiais, 10);
            }
            else if (fase_atual.elementos[lolo_atual.posicao.y/48][(lolo_atual.posicao.x + 47)/48] == 'C') {
                fase_atual.elementos[lolo_atual.posicao.y/48][(lolo_atual.posicao.x + 47)/48] = 'L';
                fase_atual.num_coracoes--;
                itoa(atoi(fase_atual.num_especiais) + 1, fase_atual.num_especiais, 10);
            }
            else if (fase_atual.elementos[(lolo_atual.posicao.y + 47)/48][(lolo_atual.posicao.x + 47)/48] == 'C') {
                fase_atual.elementos[(lolo_atual.posicao.y + 47)/48][(lolo_atual.posicao.x + 47)/48] = 'L';
                fase_atual.num_coracoes--;
                itoa(atoi(fase_atual.num_especiais) + 1, fase_atual.num_especiais, 10);
            }
            
            if (fase_atual.num_coracoes == 0) {
                if (fase_atual.elementos[lolo_atual.posicao.y/48][lolo_atual.posicao.x/48] == 'B' ||
                    fase_atual.elementos[(lolo_atual.posicao.y + 47)/48][lolo_atual.posicao.x/48] == 'B' ||
                    fase_atual.elementos[lolo_atual.posicao.y/48][(lolo_atual.posicao.x + 47)/48] == 'B' ||
                    fase_atual.elementos[(lolo_atual.posicao.y + 47)/48][(lolo_atual.posicao.x + 47)/48] == 'B') {

                    if (bau_cheio == 'S') {
                        bau_cheio = 'N';
                        fase_atual.porta_estado = 'A';
                    }
                }
            }
            
            if (IsKeyPressed(KEY_F) && atoi(fase_atual.num_especiais) > 0 && tiro_atual.mostrar == 'N') {
                tiro_atual.direcao = lolo_atual.direcao;
                
                switch (tiro_atual.direcao) {
                    case 'U':
                        tiro_atual.posicao.x = lolo_atual.posicao.x + (48 - tiro_U_D_texture.width)/2;
                        tiro_atual.posicao.y = lolo_atual.posicao.y - tiro_U_D_texture.height - 1;
                        break;
                    case 'D':
                        tiro_atual.posicao.x = lolo_atual.posicao.x + (48 - tiro_U_D_texture.width)/2;
                        tiro_atual.posicao.y = lolo_atual.posicao.y + 48;
                        break;
                    case 'R':
                        tiro_atual.posicao.x = lolo_atual.posicao.x + 48;
                        tiro_atual.posicao.y = lolo_atual.posicao.y + ((48 - tiro_L_R_texture.height)/2);
                        break;
                    case 'L':
                        tiro_atual.posicao.x = lolo_atual.posicao.x - tiro_L_R_texture.width;
                        tiro_atual.posicao.y = lolo_atual.posicao.y + ((48 - tiro_L_R_texture.height)/2);
                        break;
                }
                tiro_atual.mostrar = 'S';
                itoa(atoi(fase_atual.num_especiais) - 1, fase_atual.num_especiais, 10);
            }
            
            if (IsKeyPressed(KEY_Q)) {
                strcpy(status_jogo.parte, "MENU");
                escreve_gravacao(ARQ_GRAVACAO, &jogo_atual);
            }
            
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
                            case 'B':
                                if (fase_atual.num_coracoes != 0) {
                                    DrawTexture(bau_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                }
                                else {
                                    if (bau_cheio == 'S') {
                                        DrawTexture(bau_aberto_cheio_texture, BordaMapax + 48*i, BordaMapay + 48*j - (bau_aberto_cheio_texture.height - 48), WHITE);
                                    } else {
                                        DrawTexture(bau_aberto_vazio_texture, BordaMapax + 48*i, BordaMapay + 48*j - (bau_aberto_vazio_texture.height - 48), WHITE);
                                    }
                                }
                                break;
                            case 'C':
                                DrawTexture(coracao_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                break;
                            case 'I':
                                DrawTexture(inimigo_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                break;
                            case 'O':
                                DrawTexture(espaco_livre_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                DrawTexture(ovo_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                break;
                        }
                    }
                }
                
                if (fase_atual.porta_estado == 'F') {
                    DrawTexture(porta_fechada_texture, BordaMapax + 48*atoi(fase_atual.pos_porta) - 4, BordaMapay - 36, WHITE);
                }
                else if (fase_atual.porta_estado == 'A') {
                    DrawTexture(porta_aberta_texture, BordaMapax + 48*atoi(fase_atual.pos_porta) - 4, BordaMapay - 36, WHITE);
                }
                
                if (tiro_atual.mostrar == 'S') {
                    switch (tiro_atual.direcao) {
                        case 'D':
                        case 'U':
                            DrawTexture(tiro_U_D_texture, BordaMapax + tiro_atual.posicao.x, BordaMapay + tiro_atual.posicao.y, WHITE);
                            break;
                        case 'R':
                        case 'L':
                            DrawTexture(tiro_L_R_texture, BordaMapax + tiro_atual.posicao.x, BordaMapay + tiro_atual.posicao.y, WHITE);
                            break;
                    }
                }
                
               
                DrawTextEx(Fonte_principal, fase_atual.num_especiais, position_num_especiais, 40, 1, WHITE);

                // Imprime o lolo baseado em sua posição atualizada e a direção que ele está olhando
                switch (lolo_atual.direcao) {
                    case 'D':
                        DrawTexture(lolo_D_texture, BordaMapax + lolo_atual.posicao.x, BordaMapay + lolo_atual.posicao.y, WHITE);
                        break;
                    case 'U':
                        DrawTexture(lolo_U_texture, BordaMapax + lolo_atual.posicao.x, BordaMapay + lolo_atual.posicao.y, WHITE);
                        break;
                    case 'R':
                        DrawTexture(lolo_R_texture, BordaMapax + lolo_atual.posicao.x, BordaMapay + lolo_atual.posicao.y, WHITE);
                        break;
                    case 'L':
                        DrawTexture(lolo_L_texture, BordaMapax + lolo_atual.posicao.x, BordaMapay + lolo_atual.posicao.y, WHITE);
                        break;
                }
                
                for (i = 0; i < fase_atual.num_inimigos; i++) {
                    if (fase_atual.inimigos[i].bola == 'S') {
                        DrawTexture(ovo_texture, BordaMapax + fase_atual.inimigos[i].posicao.x, BordaMapay + fase_atual.inimigos[i].posicao.y, WHITE);
                    }
                    else {
                        switch (fase_atual.inimigos[i].tipo) {
                        case 'L':
                            DrawTexture(inimigo_texture, BordaMapax + fase_atual.inimigos[i].posicao.x, BordaMapay + fase_atual.inimigos[i].posicao.y, WHITE);
                            break;
                        }
                    }
                }
                

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
