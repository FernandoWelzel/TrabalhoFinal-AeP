// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Bibliotecas adicionais
#include "include/raylib.h"

// Nossas bibliotecas
#include "include/headers.h"

// Declaração de constantes
#define MAX_INPUT_CHARS 8
#define ARQ_GRAVACAO "records/gravacao.bin"
#define ARQ_FASE "records/fase.bin"

// --->> Funções relacionadas a manipulação de texto e entrada do usuário <<--- //

// Função que recebe uma string e um ponteiro para uma nova string e salva a primeira string na posição da nova
char * string_to_lower (char * string, char * nova_string) {
    int i, tam_string = strlen(string);
    for (i = 0; i < tam_string; i++) {
        nova_string[i] = tolower(string[i]);
    }
    nova_string[i] = '\0';
    return nova_string;
}


// --->> Funções auxiliares para a tranferência de dados de uma variável para outra <<--- //

// Função que copia os dados do segundo ponteiro de gravação para o primeiro
void copiar_gravacao (pGRAVACAO gravacao1, pGRAVACAO gravacao2) {
    strcpy(gravacao1->ident, gravacao2->ident);
    strcpy(gravacao1->totalpts, gravacao2->totalpts);
    strcpy(gravacao1->num_ult_fase, gravacao2->num_ult_fase);
    strcpy(gravacao1->vidas, gravacao2->vidas);
    strcpy(gravacao1->nomejogador, gravacao2->nomejogador);
}

// Função que recebe ponteiros para uma gravação e uma fase e os popula com os dados da gravação passada para a função
void carregar_gravacao(pGRAVACAO gravacao_atual, pFASE fase_atual, GRAVACAO gravacao) {
    strcpy(gravacao_atual->ident, gravacao.ident);
    strcpy(gravacao_atual->totalpts, gravacao.totalpts);
    strcpy(gravacao_atual->num_ult_fase, gravacao.num_ult_fase);
    strcpy(gravacao_atual->vidas, gravacao.vidas);
    strcpy(gravacao_atual->nomejogador, gravacao.nomejogador);
    
    *fase_atual = le_fase_por_pos(ARQ_FASE, atoi(gravacao_atual->num_ult_fase));
}

// --->> Funções relacionadas a a leitura e gravação em arquivos binários <<--- //

// Função que recebe uma gravação e a escreve no final do arquivo de gravações
void escreve_gravacao(char * nome_arquivo, pGRAVACAO gravacao) {
    FILE * arquivo;
    if (!(arquivo = fopen(nome_arquivo, "a+b"))) {
        printf("Erro ao abrir o arquivo de gravações\n");
    }
    else {
        fwrite(gravacao, sizeof(GRAVACAO), 1, arquivo);
        fclose(arquivo);
    }
}

// Função que recebe um nome e retorna 1 se o nome não aparece no arquivo de gravações e 0 se ele aparece
int nome_unico(char * nome_arquivo, char * nome) {
    FILE * arquivo;
    GRAVACAO gravacao_corrente;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações\n");
    }
    else {
        while (!feof(arquivo)) {
            fread(&gravacao_corrente, sizeof(GRAVACAO), 1, arquivo);
            if (strcmp(nome, gravacao_corrente.nomejogador) == 0) {
                fclose(arquivo);
                return 0;
            }
        }
    }
    fclose(arquivo);
    return 1;
}

// Função que retorna o número total de gravações no arquivo de gravações
int numero_gravacoes(char * nome_arquivo) {
    FILE * arquivo;
    int tamanho = 0;
    GRAVACAO gravacao_corrente;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações\n");
    }
    else {
        while (!feof(arquivo)) {
            fread(&gravacao_corrente, sizeof(GRAVACAO), 1, arquivo);
            tamanho++;
        }
    }
    fclose(arquivo);
    
    if (tamanho != 0) {
        tamanho--;
    }
    
    return tamanho;
}

// Função que lê uma gravação no arquivo de gravações pela posição que ela se encontra nesse arquivo
GRAVACAO le_gravacao_por_pos(char * nome_arquivo, int pos) {
    FILE * arquivo;
    GRAVACAO gravacao_lida;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações\n");
    }
    else {
        fseek(arquivo, sizeof(GRAVACAO)*pos, SEEK_SET);
        fread(&gravacao_lida, sizeof(GRAVACAO), 1, arquivo);
    }
    fclose(arquivo);
    return gravacao_lida;
}

/* 
   Função que recebe um nome de jogador e retorna a posição em que há uma gravação
   com esse nome no arquivo de gravações. Se não encontrar uma gravação com esse
   nome, retorna -1.
*/
int pos_por_nomejogador(char * nome_arquivo, char * nomejogador) {
    FILE * arquivo;
    GRAVACAO gravacao_lida;
    int pos = 0;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações\n");
    }
    else {
        while(!feof(arquivo)) {
            fread(&gravacao_lida, sizeof(GRAVACAO), 1, arquivo);
            if (strcmp(gravacao_lida.nomejogador, nomejogador) == 0) {
                return pos;
            }
            pos++;
        }
        fclose(arquivo);
    }
    return -1;
}

// Função que apaga uma gravação do arquivo de gravações pela sua ident
int apagar_gravacao(char * nome_arquivo, char *ident) {
    int i = 0, k;
    FILE * arquivo;
    
    GRAVACAO gravacao_lida;
    GRAVACAO * gravacoes;
    gravacoes = (pGRAVACAO) malloc(numero_gravacoes(nome_arquivo) * sizeof(GRAVACAO));
    
    
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações\n");
        return 0;
    }
    else {
        for (k = 0; k < numero_gravacoes(nome_arquivo); k++) {
            fread(&gravacao_lida, sizeof(GRAVACAO), 1, arquivo);
            if (strcmp(gravacao_lida.ident, ident) != 0) {
                copiar_gravacao(gravacoes + i, &gravacao_lida);
                i++;
            }
        }
        fclose(arquivo);
    }
    if (i <= 0) {
        remove(nome_arquivo);
        
    }
    else {
        if (!(arquivo = fopen(nome_arquivo, "wb"))) {
            printf("Erro ao abrir o arquivo de gravações\n");
            return 0;
        }
        else {
            fwrite(gravacoes, i, sizeof(GRAVACAO), arquivo);
            fclose(arquivo);
        }
    }
    free (gravacoes);
    return 1;
}

// Função que recebe uma gravação e faz um loop pelo arquivo de gravações procurando a gravação por sua 'ident' e, se encontrar a gravação com essa 'ident', a substitui com novos valores
int substitui_gravacao(char * nome_arquivo, pGRAVACAO nova_gravacao) {
    int i = 0, k;
    FILE * arquivo;
    
    GRAVACAO gravacao_lida;
    GRAVACAO * gravacoes;
    gravacoes = (pGRAVACAO) malloc(numero_gravacoes(nome_arquivo) * sizeof(GRAVACAO));
    
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações\n");
        return 0;
    }
    else {
        for (k = 0; k < numero_gravacoes(nome_arquivo); k++) {
            fread(&gravacao_lida, sizeof(GRAVACAO), 1, arquivo);
            if (strcmp(gravacao_lida.ident, nova_gravacao->ident) != 0) {
                copiar_gravacao(gravacoes + i, &gravacao_lida);
            }
            else {
                copiar_gravacao(gravacoes + i, nova_gravacao);
            }
            i++;
        }
        fclose(arquivo);
    }
    
    if (i <= 0) {
        remove(nome_arquivo); 
    }
    
    else {
        if (!(arquivo = fopen(nome_arquivo, "wb"))) {
            printf("Erro ao abrir o arquivo de gravações\n");
            return 0;
        }
        else {
            fwrite(gravacoes, i, sizeof(GRAVACAO), arquivo);
            fclose(arquivo);
        }
    }
    
    free (gravacoes);
    return 1;
}

// Função que lê uma fase no arquivo de fases pela posição que ela se encontra nesse arquivo
FASE le_fase_por_pos(char * nome_arquivo, int pos) {
    FILE * arquivo;
    FASE fase_lida;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de fases\n");
    }
    else {
        fseek(arquivo, sizeof(FASE)*pos, SEEK_SET);
        fread(&fase_lida, sizeof(FASE), 1, arquivo);
    }
    return fase_lida;
}


// --->> Funções relacionadas a movimentação e colisão de objetos  <<--- //

// Função que recebe um ponto em uma fase e retorna se nesse ponto há um bloco imóvel
int bloco_eh_imovel(pFASE fase, char orientacao, int y, int x) {    
    switch (fase->elementos[y/48][x/48]) {
        case 'T':
        case 'P':
        case 'A':
            return 1;
        default:
            return 0;
    }
}

/* 
   Função que retorna se um ponto na fase é um inimigo imóvel.
   Inimigos em formato de "bola" serão móveis no caso de que o bloco ao 
   lado do inimigo e na direção em que o lolo pretende se mover não for imóvel.
   No caso do inimigo estar formato bola, e a direção que se pretende mover
   não posuir um bloco móvel, a posição do inimigo é atualizada com base na
   direção e a função retorna 0.
   OBS.: Nessa função temos um esquema de recursividade, pois a função acaba
   chamando ela mesma através da função testar_pontos_imoveis para observar
   se há um inimigo imóvel na posição em que se pretende mover. Dessa maneira,
   é possível que o lolo movimente, teoricamente, infinitos inimigos móveis (em fase
   de bola) se não houver nenhum inimigo ou bloco fixo na direção que se deseja mover. 
*/
int eh_inimigo_imovel (pFASE fase, char orientacao, int y, int x, char trigger) {
    int i, retorno = 0;
    PONTO ponto1, ponto2;
    
    for (i = 0; i < fase->num_inimigos; i++) {
        if ((fase->inimigos + i)->bola == 'S' && (fase->inimigos + i)->morto == 'N') {
            if (x - (fase->inimigos + i)->posicao.x > 0 &&  x -(fase->inimigos + i)->posicao.x < 48 &&
                y - (fase->inimigos + i)->posicao.y > 0 &&  y - (fase->inimigos + i)->posicao.y < 48) {
                if (orientacao == 'U') {
                    ponto1.x = (fase->inimigos + i)->posicao.x; ponto1.y = (fase->inimigos + i)->posicao.y - 1;
                    ponto2.x = (fase->inimigos + i)->posicao.x + 47; ponto2.y = (fase->inimigos + i)->posicao.y - 1;
                    
                    if (testar_pontos_imoveis(fase, 'U', ponto1, ponto2, trigger) && ponto1.y > 0) {
                        if (trigger == 'L') {
                            (fase->inimigos + i)->posicao.y -= 2;
                        }
                        retorno = 0;
                    }
                    else {
                        retorno = 1;
                    }
                }
                else if (orientacao == 'D') {
                    ponto1.x = (fase->inimigos + i)->posicao.x; ponto1.y = (fase->inimigos + i)->posicao.y + 48;
                    ponto2.x = (fase->inimigos + i)->posicao.x + 47; ponto2.y = (fase->inimigos + i)->posicao.y + 48;
                    
                    if (testar_pontos_imoveis(fase, 'D', ponto1, ponto2, trigger) && ponto1.y < 528) {
                        if (trigger == 'L') {
                            (fase->inimigos + i)->posicao.y += 2;
                        }
                        retorno = 0;
                    }
                    else {
                        retorno = 1;
                    }
                }
                else if (orientacao == 'L') {
                    ponto1.x = (fase->inimigos + i)->posicao.x - 1; ponto1.y = (fase->inimigos + i)->posicao.y;
                    ponto2.x = (fase->inimigos + i)->posicao.x - 1; ponto2.y = (fase->inimigos + i)->posicao.y + 47;
                    
                    if (testar_pontos_imoveis(fase, 'L', ponto1, ponto2, trigger) && ponto1.x > 0) {
                        if (trigger == 'L') {
                            (fase->inimigos + i)->posicao.x -= 2;
                        }
                        retorno = 0;
                    }
                    else {
                        retorno = 1;
                    }
                }
                else if (orientacao == 'R') {
                    ponto1.x = (fase->inimigos + i)->posicao.x + 48; ponto1.y = (fase->inimigos + i)->posicao.y;
                    ponto2.x = (fase->inimigos + i)->posicao.x + 48; ponto2.y = (fase->inimigos + i)->posicao.y + 47;
                    
                    if (testar_pontos_imoveis(fase, 'R', ponto1, ponto2, trigger) && ponto1.x < 528) {
                        if (trigger == 'L') {
                            (fase->inimigos + i)->posicao.x += 2;
                        }
                        retorno = 0;
                    }
                    else {
                        retorno = 1;
                    }
                }
            }
        }
        else if ((fase->inimigos + i)->bola == 'N' && (fase->inimigos + i)->morto == 'N') {
            if (x - (fase->inimigos + i)->posicao.x > 0 &&  x -(fase->inimigos + i)->posicao.x < 48 &&
                y - (fase->inimigos + i)->posicao.y > 0 &&  y - (fase->inimigos + i)->posicao.y < 48) {
                     retorno = 1;
            }
        }
    }
    
    return retorno;
}

// Função que teste se em um ponto há um inimigo móvel (em formato de bola)
int testar_inimigos_moveis(pFASE fase, char orientacao, int y, int x) {
    int i, retorno = 0;
    
    for (i = 0; i < fase->num_inimigos; i++) {
        if (x - (fase->inimigos + i)->posicao.x > 0 &&  x -(fase->inimigos + i)->posicao.x < 48 &&
            y - (fase->inimigos + i)->posicao.y > 0 &&  y - (fase->inimigos + i)->posicao.y < 48 &&
            (fase->inimigos + i)->morto == 'N') {
            retorno = 1;
        }
    }
    
    return retorno;
}

// Função que testa um ponto do mapa tanto para blocos imoveis quanto para inimigos imóveis
int testar_pontos_imoveis(pFASE fase, char orientacao, PONTO ponto1, PONTO ponto2, char trigger) {
    if (!bloco_eh_imovel(fase, orientacao, ponto1.y, ponto1.x) &&
        !bloco_eh_imovel(fase, orientacao, ponto2.y, ponto2.x) &&
        !eh_inimigo_imovel(fase, orientacao, ponto1.y, ponto1.x, trigger) &&
        !eh_inimigo_imovel(fase, orientacao, ponto2.y, ponto2.x, trigger)) {
        return 1;
    }
    else {
        return 0;
    }
}

// Função que testa dois pontos para inimigos móveis e retorna 1 se os dois pontos não tiverem inimigos imóveis
int testar_inimigos_moveis_pontos(pFASE fase, char orientacao, PONTO ponto1, PONTO ponto2) {
    if (!testar_inimigos_moveis(fase, orientacao, ponto1.y, ponto1.x) &&
        !testar_inimigos_moveis(fase, orientacao, ponto2.y, ponto2.x)) {
        return 1;
    }
    else {
        return 0;
    }
}

// Função que atualiza a posição do lolo no jogo baseado na sua posição atual, nos blocos a sua volta e na entrada do usuário
void atualiza_pos_lolo(pLOLO lolo, pFASE fase) {
    // Declaração dos pontos a serem testados para cada caso
    PONTO ponto1, ponto2;

    if (IsKeyDown(KEY_UP)) {
        lolo->direcao = 'U';
        ponto1.x = (lolo->posicao.x); ponto1.y = (lolo->posicao.y - 1);
        ponto2.x = (lolo->posicao.x + 47); ponto2.y = (lolo->posicao.y - 1);
        
        if (testar_pontos_imoveis(fase, 'U', ponto1, ponto2, 'L')) {
            if (ponto1.y > 0) {
                lolo->posicao.y -= 2;
            }
            else if (ponto1.y <= 0 && lolo->posicao.x == atoi(fase->pos_porta)*48 && fase->porta_estado == 'A') {
                lolo->posicao.y -= 2;
            }
        }
    }

    if (IsKeyDown(KEY_DOWN)) {
        lolo->direcao = 'D';
        ponto1.x = (lolo->posicao.x); ponto1.y = (lolo->posicao.y + 49);
        ponto2.x = (lolo->posicao.x + 47); ponto2.y = (lolo->posicao.y + 49);
        
        if (testar_pontos_imoveis(fase, 'D', ponto1, ponto2, 'L') && ponto1.y < 528) {
            lolo->posicao.y += 2;
        }
    }

    if (IsKeyDown(KEY_RIGHT)) {
        lolo->direcao = 'R';
        ponto1.x = (lolo->posicao.x + 49); ponto1.y = (lolo->posicao.y);
        ponto2.x = (lolo->posicao.x + 49); ponto2.y = (lolo->posicao.y + 47);
        
        if (testar_pontos_imoveis(fase, 'R', ponto1, ponto2, 'L') && ponto1.x < 528) {
            lolo->posicao.x += 2;
        }
    }

    if (IsKeyDown(KEY_LEFT)) {
        lolo->direcao = 'L';
        ponto1.x = (lolo->posicao.x - 1); ponto1.y = (lolo->posicao.y);
        ponto2.x = (lolo->posicao.x - 1); ponto2.y = (lolo->posicao.y + 47);
        
        if (testar_pontos_imoveis(fase, 'L', ponto1, ponto2, 'L') && ponto1.x > 0) {
            lolo->posicao.x -= 2;
        }
    }
}

/* 
   Função que atualiza a posição de um do "tiro" lançado pelo lolo
   retornando um char 'L' caso o tiro esteja livre e não tenha batido
   em nenhum bloco e 'B', caso o tiro tenha batido em um bloco.
*/
char atualiza_pos_tiro(pTIRO Ptiro, pFASE fase) {
    PONTO ponto1, ponto2;
    
    switch (Ptiro->direcao) {
        case 'R':
            ponto1.x = (Ptiro->posicao.x + 43); ponto1.y = (Ptiro->posicao.y);
            ponto2.x = (Ptiro->posicao.x + 43); ponto2.y = (Ptiro->posicao.y + 30);
        
            if (testar_pontos_imoveis(fase, 'R', ponto1, ponto2, 'T') && testar_inimigos_moveis_pontos(fase, 'R', ponto1, ponto2) && ponto1.x < 528) {
                Ptiro->posicao.x += 8;
                return 'L';
            }
            else {
                return 'B';
            }
            break;
        case 'L':
            ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y);
            ponto2.x = (Ptiro->posicao.x); ponto2.y = (Ptiro->posicao.y + 30);
        
            if (testar_pontos_imoveis(fase, 'L', ponto1, ponto2, 'T') && testar_inimigos_moveis_pontos(fase, 'L', ponto1, ponto2) && ponto1.x > 0) {
                Ptiro->posicao.x -= 8;
                return 'L';
            }
            else {
                return 'B';
            }
            break;
        case 'U':
            ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y);
            ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y);
        
            if (testar_pontos_imoveis(fase, 'U', ponto1, ponto2, 'T') && testar_inimigos_moveis_pontos(fase, 'L', ponto1, ponto2) && ponto1.y > 0) {
                Ptiro->posicao.y -= 8;
                return 'L';
            }
            else {
                return 'B';
            }
            break;
        case 'D':
            ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y + 45);
            ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y + 45);
        
            if (testar_pontos_imoveis(fase, 'D', ponto1, ponto2, 'T') && testar_inimigos_moveis_pontos(fase, 'L', ponto1, ponto2) && ponto1.y < 528) {
                Ptiro->posicao.y += 8;
                return 'L';
            }
            else {
                return 'B';
            }
            break;
        default:
            return 'L';
            break;
    }
}

// Função que testa um ponto para todos os possíveis casos que fazem um tiro parar (se há um bloco imóvel, um inimigo imóvel ou um inimigo móvel)
int teste_tiro_bateu(pFASE fase, char direcao, int y, int x) {
    if (bloco_eh_imovel(fase, direcao, y, x) || eh_inimigo_imovel(fase, direcao, y, x, 'T') || testar_inimigos_moveis(fase, direcao, y, x)) {
        return 1;
    }
    return 0;
}

/* 
   Posição que retorna um ponto em que o tiro bateu, testando se havia
   blocos ou inimigos imóveis na direção na qual o tiro estava se movendo   
*/
PONTO pos_tiro_bateu(pTIRO Ptiro, pFASE fase) {
    PONTO retorno, ponto1, ponto2;
    
    switch(Ptiro->direcao) {
        case 'R':
            ponto1.x = (Ptiro->posicao.x + 48); ponto1.y = (Ptiro->posicao.y);
            ponto2.x = (Ptiro->posicao.x + 48); ponto2.y = (Ptiro->posicao.y + 30);
        
            if (teste_tiro_bateu(fase, Ptiro->direcao, ponto1.y, ponto1.x)) {
                return ponto1;
            }
            else if (teste_tiro_bateu(fase, Ptiro->direcao, ponto2.y, ponto2.x)) {
                return ponto2;
            }
            else if ((Ptiro->posicao.x + 45) > 528) {
                retorno.x = -1;
                retorno.y = -1;
                return retorno;
            }
            break;
        case 'L':
            ponto1.x = (Ptiro->posicao.x - 3); ponto1.y = (Ptiro->posicao.y);
            ponto2.x = (Ptiro->posicao.x - 3); ponto2.y = (Ptiro->posicao.y + 30);
            
            if (teste_tiro_bateu(fase, Ptiro->direcao, ponto1.y, ponto1.x)) {
                return ponto1;
            }
            else if (teste_tiro_bateu(fase, Ptiro->direcao, ponto2.y, ponto2.x)) {
                return ponto2;
            }
            else if ((Ptiro->posicao.x) < 528) {
                retorno.x = -1;
                retorno.y = -1;
                return retorno;
            }
            break;
        case 'U':
            ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y - 3);
            ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y - 3);
            
            if (teste_tiro_bateu(fase, Ptiro->direcao, ponto1.y, ponto1.x)) {
                return ponto1;
            }
            else if (teste_tiro_bateu(fase, Ptiro->direcao, ponto2.y, ponto2.x)) {
                return ponto2;
            }
            else if ((Ptiro->posicao.y) < 0) {
                retorno.x = -1;
                retorno.y = -1;
                return retorno;
            }
            break;
        case 'D':
            ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y + 48);
            ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y + 48);
            
            if (teste_tiro_bateu(fase, Ptiro->direcao, ponto1.y, ponto1.x)) {
                return ponto1;
            }
            else if (teste_tiro_bateu(fase, Ptiro->direcao, ponto2.y, ponto2.x)) {
                return ponto2;
            }
            else if ((Ptiro->posicao.y) > 528) {
                retorno.x = -1;
                retorno.y = -1;
                return retorno;
            }
            break;
    }
    
    retorno.x = -1;
    retorno.y = -1;
    return retorno;
}

// Função que atualiza a posição do tiro do inimigo (denominado "chiclete")
char atualiza_pos_chiclete(pTIRO Ptiro, pLOLO Plolo) {
    PONTO ponto1, ponto2;
    
    switch (Ptiro->direcao) {
        case 'R':
            ponto1.x = (Ptiro->posicao.x + 48); ponto1.y = (Ptiro->posicao.y);
            ponto2.x = (Ptiro->posicao.x + 48); ponto2.y = (Ptiro->posicao.y + 30);
        
            if ((ponto1.x >= Plolo->posicao.x && ponto1.x <= Plolo->posicao.x + 47 && ponto1.y >= Plolo->posicao.y && ponto1.y <= Plolo->posicao.y + 47) ||
                (ponto2.x >= Plolo->posicao.x && ponto2.x <= Plolo->posicao.x + 47 && ponto2.y >= Plolo->posicao.y && ponto2.y <= Plolo->posicao.y + 47)) {
                return 'B';
            }
            else {
                return 'L';
            }
            break;
        case 'D':
            ponto1.x = (Ptiro->posicao.x); ponto1.y = (Ptiro->posicao.y + 48);
            ponto2.x = (Ptiro->posicao.x + 30); ponto2.y = (Ptiro->posicao.y + 48);
        
            if ((ponto1.x >= Plolo->posicao.x && ponto1.x <= Plolo->posicao.x + 47 && ponto1.y >= Plolo->posicao.y && ponto1.y <= Plolo->posicao.y + 47) ||
                (ponto2.x >= Plolo->posicao.x && ponto2.x <= Plolo->posicao.x + 47 && ponto2.y >= Plolo->posicao.y && ponto2.y <= Plolo->posicao.y + 47)) {
                return 'B';
            }
            else {
                return 'L';
            }
            break;
        default:
            return 'L';
            break;
    }
}

// --->> MAIN <<--- //
int main(void) {
    
    // Inicia a janela com as dimensões indicadas
    int screen_width = 800;
    int screen_height = 800;
    InitWindow(screen_width, screen_height, "Adventures of Lolo");

    // Créditos (return menu)
    char message[28] = "PRESS ENTER TO RETURN MENU";

    // Configurações
    SetTargetFPS(60);
    char status_jogo[5] = "MENU";

    // Música
    InitAudioDevice();
    Music music = LoadMusicStream("./resources/Songs/Main-theme.mp3");
    music.looping = true;
    PlayMusicStream(music);

    // Fonte
    Font Fonte_principal = LoadFont("./resources/fonte.ttf");

    // Texturas
    Texture2D menu_texture = LoadTexture("./resources/Menus/Menu.png");
    Texture2D load_vazio_texture = LoadTexture("./resources/Menus/Tela loads vazio.png");
    Texture2D lolo_texture = LoadTexture("./resources/Menus/Menu_lolo.png");
    Texture2D cred_texture = LoadTexture("./resources/Menus/Creditos.png");
    Texture2D fundo_texture = LoadTexture("./resources/Menus/Fundo sem nada.png");
    Texture2D mapa_vazio_texture = LoadTexture("./resources/Mapa/Mapa_vazio.png");
    Texture2D espaco_livre_texture = LoadTexture("./resources/Blocos/Livre.png");
    Texture2D agua_texture = LoadTexture("./resources/Blocos/Agua.png");
    Texture2D pedra_texture = LoadTexture("./resources/Blocos/Pedra.png");
    Texture2D arvore_texture = LoadTexture("./resources/Blocos/Tree.png");
    Texture2D escada_texture = LoadTexture("./resources/Blocos/Escada.png");
    Texture2D bau_texture = LoadTexture("./resources/Blocos/Bau.png");
    Texture2D bau_aberto_cheio_texture = LoadTexture("./resources/Blocos/Bau_aberto_cheio.png");
    Texture2D bau_aberto_vazio_texture = LoadTexture("./resources/Blocos/Bau_aberto_vazio.png");
    Texture2D porta_fechada_texture = LoadTexture("./resources/Blocos/Porta_fechada.png");
    Texture2D porta_aberta_texture = LoadTexture("./resources/Blocos/Porta_aberta.png");
    Texture2D coracao_texture = LoadTexture("./resources/Blocos/Coracao.png");
    Texture2D lolo_D_texture = LoadTexture("./resources/Lolo/Lolo-D.png");
    Texture2D lolo_R_texture = LoadTexture("./resources/Lolo/Lolo-R.png");
    Texture2D lolo_L_texture = LoadTexture("./resources/Lolo/Lolo-L.png");
    Texture2D lolo_U_texture = LoadTexture("./resources/Lolo/Lolo-U.png");
    Texture2D larva_texture = LoadTexture("./resources/Inimigos/Larva.png");
    Texture2D tiro_L_R_texture = LoadTexture("./resources/Poderes/tiro_L_R.png");
    Texture2D tiro_U_D_texture = LoadTexture("./resources/Poderes/tiro_U_D.png");
    Texture2D ovo_texture = LoadTexture("./resources/Inimigos/ovo.png");
    Texture2D atirador_R_texture = LoadTexture("./resources/Inimigos/atirador_R.png");
    Texture2D chiclete_R_texture = LoadTexture("./resources/Inimigos/chiclete_R.png");
    Texture2D atirador_D_texture = LoadTexture("./resources/Inimigos/atirador_D.png");
    Texture2D chiclete_D_texture = LoadTexture("./resources/Inimigos/chiclete_D.png");
    Texture2D bloco_movel_texture = LoadTexture("./resources/Blocos/Bloco-Movel.png");

    // Variáveis de posicionamento
    int BordaMapax = ((screen_width - mapa_vazio_texture.width)/2) + 45;
    int BordaMapay = ((screen_height - mapa_vazio_texture.height)/2) + 96;
    int ponto_x_lolo_menu = (screen_width - menu_texture.width)/2 + 45;
    int ponto_x_lolo_load = (screen_width - menu_texture.width)/2 + 25;
    int ponto_y_inic_lolo_menu = (screen_height - menu_texture.height)/2 + 85;
    int ponto_y_inic_lolo_load = (screen_height - menu_texture.height)/2 + 105;
    int desloc_y_lolo_menu = 60;
    int desloc_y_lolo_load = 42;
    
    PONTO lolo_sel_ponto_menu = {ponto_x_lolo_menu, ponto_y_inic_lolo_menu};
    PONTO lolo_sel_ponto_load = {ponto_x_lolo_menu, ponto_y_inic_lolo_load};
    PONTO lolo_sel_ponto_quit = {ponto_x_lolo_menu + 30, ponto_y_inic_lolo_load};
    
    Vector2 position1 = {275, 265};
    Vector2 position2 = {250, 450};
    Vector2 position3 = {210, 500};
    Vector2 position5 = {210, 250};
    Rectangle textBox = {255, 385, 300, 50};
    Vector2 position4 = {textBox.x + 5, textBox.y + 8};
    Vector2 position6 = {215, 300};
    Vector2 position7 = {ponto_x_lolo_menu + 100, ponto_y_inic_lolo_load};
    Vector2 position8 = {ponto_x_lolo_menu + 100, ponto_y_inic_lolo_load + desloc_y_lolo_load};
    Vector2 position9 = {210, 530};
    Vector2 position10 = {210, 300};
    Vector2 position11 = {210, 530};
    Vector2 position12 = {260, 320};
    Vector2 position_text_load = {ponto_x_lolo_menu, ponto_y_inic_lolo_load - 30};
    Vector2 position_num_especiais = {BordaMapax + 585, 300};
    Vector2 position_num_vidas = {BordaMapax + 585, 210};
    Vector2 position_num_fase = {BordaMapax + 120, BordaMapay - 140};
    Vector2 position_num_fase_letra = {BordaMapax - 20, BordaMapay - 140};
    Vector2 position_pontuacao = {BordaMapax + 440, BordaMapay - 140};
    Vector2 position_pontuacao_letra = {BordaMapax + 240, BordaMapay - 140};

    // Declaração das variáveis da caixa de texto com o nome do jogador
    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;
    int nome_n_unico = 0;

    // Declaração de gravação, fase e lolo atuais
    GRAVACAO jogo_atual;
    FASE fase_atual;
    LOLO lolo_atual;
    
    // Variáveis para iteração
    int i, j;
    
    // Variáveis estilo booleanas
    char bau_cheio = 'S';
    
    // Tiro
    TIRO tiro_atual;
    tiro_atual.mostrar = 'N';
    PONTO pos_tiro_bateu_atual;
    
    // Gravações jogos antigos
    GRAVACAO gravacoes_salvas[5];
    int numero_de_gravacoes = numero_gravacoes(ARQ_GRAVACAO);
    
    // Contadores de frames
    int framesCounter = 0;
    int framesCounter2 = 0;
    
    // Variáveis auxiliares
    char vidas_novas[3];
    
    // Main game loop
    while (!WindowShouldClose() && strcmp(status_jogo, "SAIR") != 0)
    {
        UpdateMusicStream(music); //Tocar a musica quando abre o menu
        
        /*
        Nessa parte do código é definido a parte do jogo que deve ser
        exibida na tela. Para isso fazemos comparações com a string que
        quarda a parte do jogo que estamos.
        */
        
        // MENU (Seleção entre demais telas)
        if (strcmp(status_jogo, "MENU") == 0) {
            
           // Detecta se o usuário apertou uma das setas para se mover no menu e muda a posição do lolo (cursor do menu) 
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto_menu.y != ponto_y_inic_lolo_menu) {
                lolo_sel_ponto_menu.y -= desloc_y_lolo_menu;
            }

            if (IsKeyPressed(KEY_DOWN) && lolo_sel_ponto_menu.y != ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                lolo_sel_ponto_menu.y += desloc_y_lolo_menu;
            }
            
            // Detecta se o usuário apertou enter, indicando que ele quer entrar em outra área. Dessa maneira, o direciona baseado na posição do lolo do menu
            if (IsKeyPressed(KEY_ENTER)) {
                if (lolo_sel_ponto_menu.y == ponto_y_inic_lolo_menu) {
                    strcpy(status_jogo, "NAME");
                    numero_de_gravacoes = numero_gravacoes(ARQ_GRAVACAO);
                }
                else if (lolo_sel_ponto_menu.y == ponto_y_inic_lolo_menu + 1*desloc_y_lolo_menu) {
                    numero_de_gravacoes = numero_gravacoes(ARQ_GRAVACAO);
                    if (numero_de_gravacoes == 0) {
                        strcpy(status_jogo, "NULL");
                    }
                    else {
                        for (i = 0; i < numero_de_gravacoes; i++) {
                            gravacoes_salvas[i] = le_gravacao_por_pos(ARQ_GRAVACAO, i);
                        }
                        strcpy(status_jogo, "LOAD");
                    }
                }
                else if (lolo_sel_ponto_menu.y == ponto_y_inic_lolo_menu + 2*desloc_y_lolo_menu) {
                    strcpy(status_jogo, "CRED");
                }
                else if (lolo_sel_ponto_menu.y == ponto_y_inic_lolo_menu + 3*desloc_y_lolo_menu) {
                    strcpy(status_jogo, "SAIR");
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
        if (strcmp(status_jogo, "NAME") == 0) {

            // --> Implementação da caixa de texto para o jogador inserir o nome <-- //
            // Guarda a tecla apertada na variável tecla
            int tecla = GetCharPressed();

            // Checa se mais de um caracter foi pressionado por frame e adiciona o valor ao nome
            while (tecla > 0) {
                if ((tecla >= 32) && (tecla <= 125) && (letterCount < MAX_INPUT_CHARS)) {
                    name[letterCount] = (char)tecla;
                    letterCount++;
                }
                tecla = GetCharPressed();  // Pega o próximo caracter se houver
            }

            // Checa se foi apertado o backspace e nesse caso diminui o tamanho da string
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (letterCount > 0) {
                    letterCount--;
                }
                name[letterCount] = '\0';
            }

            /* 
            Checa se foi apertado enter, nesse caso salva o nome do jogador se ele tiver
            colocado um nome válido e começa o jogo com parâmetros iniciais, mudando para
            uma tela com o texto inicial da fase.
            */
            if (IsKeyPressed(KEY_ENTER)) {
                if (letterCount > 0 && numero_de_gravacoes < 5) {
                    itoa(numero_gravacoes(ARQ_GRAVACAO), jogo_atual.ident, 10);
                    strcpy(jogo_atual.totalpts, "0");
                    strcpy(jogo_atual.num_ult_fase, "0");
                    strcpy(jogo_atual.vidas, "3");
                    char string_interm[9];
                    strcpy(jogo_atual.nomejogador, string_to_lower(name, string_interm));

                    if (nome_unico(ARQ_GRAVACAO, jogo_atual.nomejogador)) {
                        strcpy(status_jogo, "TEXT");
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
            
            // Checa se foi apertado o delete - indicando que o jogador quer sair desse menu
            if (IsKeyPressed(KEY_DELETE)) {
                strcpy(status_jogo, "MENU");
            }
            
            // Mostra a tela para escrever o nome do jogador
            BeginDrawing();

                ClearBackground(BLACK);

                // Imagem de fundo
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);

                // Texto a cima
                DrawTextEx(Fonte_principal, "Nome do", position1, 50, 1, BLACK);
                DrawTextEx(Fonte_principal, "jogador:", position12, 50, 1, BLACK);

                // Caixa para inserir o nome
                DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, BLACK);
                DrawTextEx(Fonte_principal, name, position4, 40, 1, BLACK);

                // Texto a baixo
                DrawTextEx(Fonte_principal, TextFormat("Caracteres %i de %i", letterCount, MAX_INPUT_CHARS), position2, 25, 1, BLACK);
                if (numero_de_gravacoes >= 5) {
                    DrawTextEx(Fonte_principal, "Maximo de saves atingido!", position3, 23, 1, BLACK);
                    DrawTextEx(Fonte_principal, " Aperte DEL para voltar", position9, 23, 1, BLACK);
                }
                else {
                    if (nome_n_unico) {
                        DrawTextEx(Fonte_principal, "  Insira outro nome", position3, 30, 1, BLACK);
                    }
                    else {
                        DrawTextEx(Fonte_principal, "Pressione Enter para salvar", position3, 19, 1, BLACK);
                    }
                }

            EndDrawing();
        }
        
        // LOAD    
        if (strcmp(status_jogo, "LOAD") == 0) {
            
            // Atualiza a posição do lolo (cursor que indica o save) baseado na entrada do usuário e na posição dele
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto_load.y > ponto_y_inic_lolo_load) {
                lolo_sel_ponto_load.y -= desloc_y_lolo_load;
            }
            if ((IsKeyPressed(KEY_DOWN)) && (lolo_sel_ponto_load.y < ponto_y_inic_lolo_load + ((numero_de_gravacoes - 1) * desloc_y_lolo_load))) {
                lolo_sel_ponto_load.y += desloc_y_lolo_load;
            }
            
            /*
                Se for pressionado enter, indicando que o usuário quer carregar aquele save:
                - Distingue que save o usuário quer baseado na posição do cursor
                - Copia as informações do save para jogo_atual (que é uma GRAVACAO)
                - Lê a fase que estava na gravação lida e armazena em fase_atual (que é uma FASE)
            */
            if (IsKeyPressed(KEY_ENTER)) {
                for (i = 0; i < numero_de_gravacoes; i++) {
                    if ((lolo_sel_ponto_load.y == ponto_y_inic_lolo_load + i*desloc_y_lolo_load)) { // Número_arquivos
                        
                        carregar_gravacao(&jogo_atual, &fase_atual, gravacoes_salvas[i]);
                      
                        lolo_atual.posicao.x = fase_atual.pos_i_jogador.x*48;
                        lolo_atual.posicao.y = fase_atual.pos_i_jogador.y*48;
                        lolo_atual.direcao = 'D';
                        tiro_atual.mostrar = 'N';
                        
                        strcpy(status_jogo, "TEXT");
                    }
                }
            }
            
            /* 
            Se o usuário pressionou delete, indicando que ele quer deletar aquele save, 
            copia o save deletado para gravações_salvas[0] e troca o status do jogo para
            CONF (Tela para confirmar se deseja apagar o save).
            */
            if (IsKeyPressed(KEY_DELETE)) {
                for (i = 0; i < numero_de_gravacoes; i++) {
                    if ((lolo_sel_ponto_load.y == ponto_y_inic_lolo_load + i*desloc_y_lolo_load)) { // Número_arquivos
                        copiar_gravacao(gravacoes_salvas, gravacoes_salvas + i);
                        strcpy(status_jogo, "CONF");
                    }
                }
            }
            
            // Se o usuário pressinar a tecla 'Q' -> Retorna ao menu
            if (IsKeyPressed(KEY_Q)) {
                strcpy(status_jogo, "MENU");
            }
            
            BeginDrawing();
                ClearBackground(BLACK);
                
                // Mostra uma tela com fundo padrão, o texto solicitando que o usuário escolha um save e uma descrição do que representa cada coluna
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                DrawTextEx(Fonte_principal, "Escolha um jogo salvo", position5, 24, 2, BLACK);
                DrawTextEx(Fonte_principal, "     Jogador     F    V    P", position_text_load, 24, 2, BLACK);
                
                if (numero_de_gravacoes <= 0) {
                    DrawTextEx(Fonte_principal, "  Sem jogos\n   para\n   mostrar", position10, 40, 1, MAROON);
                }
                else {
                    DrawTextEx(Fonte_principal, "Aperte DEL para apagar", position11, 24, 1, BLACK);
                }
                
                // Imprime o lolo (cursor que indica o save) na posição atualizada
                DrawTexture(lolo_texture, ponto_x_lolo_load, lolo_sel_ponto_load.y, WHITE);
                
                // Imprime as gravações do vetor de gravações salvas
                for (i = 0; i < numero_de_gravacoes; i++) {
                    DrawText(gravacoes_salvas[i].nomejogador, ponto_x_lolo_load + 65, ponto_y_inic_lolo_load + desloc_y_lolo_load*i, 35, BLACK);
                    DrawText(gravacoes_salvas[i].num_ult_fase, ponto_x_lolo_load + 240, ponto_y_inic_lolo_load + desloc_y_lolo_load*i + 10, 25, BLACK);
                    DrawText(gravacoes_salvas[i].vidas, ponto_x_lolo_load + 290, ponto_y_inic_lolo_load + desloc_y_lolo_load*i + 10, 25, BLACK);
                    DrawText(gravacoes_salvas[i].totalpts, ponto_x_lolo_load + 340, ponto_y_inic_lolo_load + desloc_y_lolo_load*i + 10, 25, BLACK);
                }
                
            EndDrawing();
        }
        
        // TEXT (Tela que exibe o texto inicial do jogo)
        if (strcmp(status_jogo, "TEXT") == 0) {
            
            // Faz a contagem do número de frames, para depois de 3 segundo sair da tela de texto
            framesCounter2++;
            if (framesCounter2/60 > 3) {
                framesCounter2 = 0;
                strcpy(status_jogo, "GAME");
            }
            
            // Mostra a tela com o fundo padrão e a mensagem de texto que está em fase_atual.texto_inic
            BeginDrawing();

                ClearBackground(BLACK);
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);

                // Imprime o texto da fase atual
                DrawTextEx(Fonte_principal, fase_atual.texto_inic, position6, 30, 1, BLACK);

            EndDrawing();
        }

        // GAME (Jogo em si)
        if (strcmp(status_jogo, "GAME") == 0) {
            
            // Atualiza a posição do lolo baseado na sua posição atual, na tecla que o usuário pressiona e nos blocos a sua volta
            atualiza_pos_lolo(&lolo_atual, &fase_atual);
            
            // Se o tiro estiver sendo mostrado -> atualiza a posição do tiro até ele bater
            if (tiro_atual.mostrar == 'S') {
                if (atualiza_pos_tiro(&tiro_atual, &fase_atual) == 'B') {
                    /* 
                        Quando o tiro bate, para de mostrar o tiro e pega a posição que o tiro bateu,
                        se a posição não for uma lateral ou um bloco imóvel, mas for um inimigo imóvel,
                        transforma o inimigo em bola. Se a posição em que o tiro bateu conter um inimigo
                        móvel, mata o inimigo.
                    */
                    tiro_atual.mostrar = 'N';
                    pos_tiro_bateu_atual = pos_tiro_bateu(&tiro_atual, &fase_atual);
                    if (pos_tiro_bateu_atual.x != -1) {
                        for (i = 0; i < fase_atual.num_inimigos; i++) {
                            if (pos_tiro_bateu_atual.x - fase_atual.inimigos[i].posicao.x > 0 &&  pos_tiro_bateu_atual.x - fase_atual.inimigos[i].posicao.x < 48 &&
                                pos_tiro_bateu_atual.y - fase_atual.inimigos[i].posicao.y > 0 &&  pos_tiro_bateu_atual.y - fase_atual.inimigos[i].posicao.y < 48) {
                                if (fase_atual.inimigos[i].bola == 'N' && fase_atual.inimigos[i].morto == 'N') {
                                    fase_atual.inimigos[i].bola = 'S';
                                }
                                else if (fase_atual.inimigos[i].bola == 'S' && fase_atual.inimigos[i].morto == 'N' && fase_atual.inimigos[i].tipo != 'M') {
                                    fase_atual.inimigos[i].morto = 'S';
                                    itoa(atoi(jogo_atual.totalpts) + 1, jogo_atual.totalpts, 10);
                                    if (atoi(jogo_atual.totalpts) % 10 == 0) {
                                        itoa(atoi(jogo_atual.vidas) + 1, jogo_atual.vidas, 10);
                                    }
                                }
                            }
                        }
                    }
                }
            }        
            
            /*
                Compara se a posição de alguma das bordas do lolo é a mesma de um coração,
                para que ele poça pegar o coração. Se ele pegar o coração, tranforma o bloco
                de 'C' para 'L'.
            */
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
            
            /* 
                Compara se alguma das bordas do lolo está dentro de um bloco
                que tem um bau aberto e cheio. Nesse caso, esvazia o bau, abre
                a porta e mata todos os inimigos.
            */
            if (fase_atual.num_coracoes == 0 && bau_cheio == 'S') {
                if (fase_atual.elementos[lolo_atual.posicao.y/48][lolo_atual.posicao.x/48] == 'B' ||
                    fase_atual.elementos[(lolo_atual.posicao.y + 47)/48][lolo_atual.posicao.x/48] == 'B' ||
                    fase_atual.elementos[lolo_atual.posicao.y/48][(lolo_atual.posicao.x + 47)/48] == 'B' ||
                    fase_atual.elementos[(lolo_atual.posicao.y + 47)/48][(lolo_atual.posicao.x + 47)/48] == 'B') {

                    if (bau_cheio == 'S') {
                        bau_cheio = 'N';
                        fase_atual.porta_estado = 'A';
                        for (i = 0; i < fase_atual.num_inimigos; i++) {
                            fase_atual.inimigos[i].morto = 'S';
                        }
                    }
                }
            }
            
            /*
                Se o jogador chegar a uma posição y menor do que -24 pixels em relação a borda
                (o que só pode acontecer se ele passou pela porta) aumenta a contagem da fase no
                jogo atual e lê a nova fase, salvando em fase_atual.
            */
            if (lolo_atual.posicao.y < -24) {
                strcpy(status_jogo, "MENU");
                bau_cheio = 'S';
                itoa(atoi(jogo_atual.num_ult_fase) + 1, jogo_atual.num_ult_fase, 10);
                
                substitui_gravacao(ARQ_GRAVACAO, &jogo_atual);
                strcpy(status_jogo, "TEXT");
                fase_atual = le_fase_por_pos(ARQ_FASE , atoi(jogo_atual.num_ult_fase));
                lolo_atual.posicao.x = fase_atual.pos_i_jogador.x*48;
                lolo_atual.posicao.y = fase_atual.pos_i_jogador.y*48;
                lolo_atual.direcao = 'D';
                tiro_atual.mostrar = 'N';
            }
            
            /*
                Observa se a tecla F foi pressionada, indicando que o jogador quer lançar um tiro.
                Se for possível atirar, faz o tiro ser mostrado em uma posição baseado na direção
                que o lolo está olhando e na posição atual do lolo.
            */
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
            
            // Se o jogador apertar Q, muda a janela que está sendo mostrada para QUIT
            if (IsKeyPressed(KEY_Q)) {
                strcpy(status_jogo, "QUIT");
            }
            
            /*
                Se o jogador apertar S, reinicia a fase, lendo a última gravação desse jogador,
                diminuindo uma vida e (dependendo do número de vidas) apaga a gravação ou lê a
                fase do arquivo de fases novamente.
            */
            if (IsKeyPressed(KEY_S)) {
                itoa(atoi(jogo_atual.vidas) - 1, vidas_novas, 10);
                
                carregar_gravacao(&jogo_atual, &fase_atual, le_gravacao_por_pos(ARQ_GRAVACAO, pos_por_nomejogador(ARQ_GRAVACAO, jogo_atual.nomejogador)));
                
                strcpy(jogo_atual.vidas, vidas_novas);
                
                lolo_atual.posicao.x = fase_atual.pos_i_jogador.x*48;
                lolo_atual.posicao.y = fase_atual.pos_i_jogador.y*48;
                lolo_atual.direcao = 'D';
                tiro_atual.mostrar = 'N';
                bau_cheio = 'S';
                                
                if (atoi(jogo_atual.vidas) <= 0) {
                    strcpy(status_jogo, "LOSE");
                    apagar_gravacao(ARQ_GRAVACAO, jogo_atual.ident);
                }
                else {
                    strcpy(status_jogo, "TEXT");
                    fase_atual = le_fase_por_pos(ARQ_FASE , atoi(jogo_atual.num_ult_fase));
                }
            }
            
            
            /*
                Faz um loop pelo vetor de inimigos e no caso dos inimigos serem do tipo 'R' ou 'D',
                atualiza os tiros lançados por esses inimigos baseado na posição que o lolo ocupa no
                mapa e no fato do tiro estar sendo mostrado ou não. Se o tiro atingir o lolo, diminui
                uma vida do lolo e reinicia a fase, gravando o save atualizado no arquivo de gravações.
            */
            for (i = 0; i < fase_atual.num_inimigos; i++) {
                if (fase_atual.inimigos[i].morto == 'N' && fase_atual.inimigos[i].bola == 'N') {
                    if (fase_atual.inimigos[i].tipo == 'R') {
                        if (fase_atual.inimigos[i].tiro.mostrar == 'N') {
                            if (lolo_atual.posicao.y + 48 >= fase_atual.inimigos[i].posicao.y && lolo_atual.posicao.y + 48 <= fase_atual.inimigos[i].posicao.y + 95 && lolo_atual.posicao.x >=  fase_atual.inimigos[i].posicao.x + 48) {
                                fase_atual.inimigos[i].tiro.posicao.x = fase_atual.inimigos[i].posicao.x + 48;
                                fase_atual.inimigos[i].tiro.posicao.y = fase_atual.inimigos[i].posicao.y + ((48 - chiclete_R_texture.height)/2);
                                fase_atual.inimigos[i].tiro.mostrar = 'S';
                            }
                        }
                        else if (fase_atual.inimigos[i].tiro.mostrar == 'S') {
                            if (atualiza_pos_tiro(&fase_atual.inimigos[i].tiro, &fase_atual) == 'B') {
                                fase_atual.inimigos[i].tiro.mostrar = 'N';
                            }
                            else if (atualiza_pos_chiclete(&fase_atual.inimigos[i].tiro, &lolo_atual) == 'B') {
                                fase_atual.inimigos[i].tiro.mostrar = 'N';
                                itoa(atoi(jogo_atual.vidas) - 1, vidas_novas, 10);
                                
                                lolo_atual.posicao.x = fase_atual.pos_i_jogador.x*48;
                                lolo_atual.posicao.y = fase_atual.pos_i_jogador.y*48;
                                lolo_atual.direcao = 'D';
                                tiro_atual.mostrar = 'N';
                                bau_cheio = 'S';
                                
                                if (atoi(vidas_novas) <= 0) {
                                    strcpy(status_jogo, "LOSE");
                                    apagar_gravacao(ARQ_GRAVACAO, jogo_atual.ident);
                                }
                                else {
                                    strcpy(status_jogo, "TEXT");
                                    carregar_gravacao(&jogo_atual, &fase_atual, le_gravacao_por_pos(ARQ_GRAVACAO, pos_por_nomejogador(ARQ_GRAVACAO, jogo_atual.nomejogador)));
                                    strcpy(jogo_atual.vidas, vidas_novas);
                                    substitui_gravacao(ARQ_GRAVACAO, &jogo_atual);
                                }
                            }
                        }
                    }
                    else if (fase_atual.inimigos[i].tipo == 'D') {
                        if (fase_atual.inimigos[i].tiro.mostrar == 'N') {
                            if (lolo_atual.posicao.x + 47 >= fase_atual.inimigos[i].posicao.x && lolo_atual.posicao.x <= fase_atual.inimigos[i].posicao.x + 47 && lolo_atual.posicao.y >= fase_atual.inimigos[i].posicao.y + 48) {
                                fase_atual.inimigos[i].tiro.posicao.x = fase_atual.inimigos[i].posicao.x + (48 - chiclete_D_texture.width)/2;
                                fase_atual.inimigos[i].tiro.posicao.y = fase_atual.inimigos[i].posicao.y + 48;
                                fase_atual.inimigos[i].tiro.mostrar = 'S';
                            }
                        }
                        else if (fase_atual.inimigos[i].tiro.mostrar == 'S') {
                            if (atualiza_pos_tiro(&fase_atual.inimigos[i].tiro, &fase_atual) == 'B') {
                                fase_atual.inimigos[i].tiro.mostrar = 'N';
                            }
                            else if (atualiza_pos_chiclete(&fase_atual.inimigos[i].tiro, &lolo_atual) == 'B') {
                                fase_atual.inimigos[i].tiro.mostrar = 'N';
                                itoa(atoi(jogo_atual.vidas) - 1, jogo_atual.vidas, 10);
                
                                lolo_atual.posicao.x = fase_atual.pos_i_jogador.x*48;
                                lolo_atual.posicao.y = fase_atual.pos_i_jogador.y*48;
                                lolo_atual.direcao = 'D';
                                tiro_atual.mostrar = 'N';
                                bau_cheio = 'S';
                                
                                if (atoi(jogo_atual.vidas) <= 0) {
                                    strcpy(status_jogo, "LOSE");
                                    apagar_gravacao(ARQ_GRAVACAO, jogo_atual.ident);
                                }
                                else {
                                    strcpy(status_jogo, "TEXT");
                                    carregar_gravacao(&jogo_atual, &fase_atual, le_gravacao_por_pos(ARQ_GRAVACAO, pos_por_nomejogador(ARQ_GRAVACAO, jogo_atual.nomejogador)));
                                    strcpy(jogo_atual.vidas, vidas_novas);
                                    substitui_gravacao(ARQ_GRAVACAO, &jogo_atual);
                                }
                            }
                        }
                    }
                }
            }        
            
            // Mostra a tela do jogo baseado nos blocos contidos em fase_atual.elementos e na posição do lolo
            BeginDrawing();
                
                // Mostra um mapa vazio
                ClearBackground(BLACK);
                DrawTexture(mapa_vazio_texture, (screen_width - mapa_vazio_texture.width)/2, (screen_height - mapa_vazio_texture.height)/2, WHITE);

                // Imprime cada um dos quadrados do mapa, no qual o lolo e os inimigos andarão por cima, que estão em fase_atual.elementos
                for (i = 0; i < 11; i++) {
                    for (j = 0; j < 11; j++) {
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
                            case 'E':
                                DrawTexture(escada_texture, BordaMapax + 48*i, BordaMapay + 48*j, WHITE);
                                break;    
                        }
                    }
                }
                
                /* 
                   Imprime a porta em sua posição definida em fase_atual.pos_porta.
                   Muda a textura da porta mostrada a partir da variável
                   fase_atual.porta_estado -> 'F' = fechada; 'A' = aberta
                */
                if (fase_atual.porta_estado == 'F') {
                    DrawTexture(porta_fechada_texture, BordaMapax + 48*atoi(fase_atual.pos_porta) - 4, BordaMapay - 36, WHITE);
                }
                else if (fase_atual.porta_estado == 'A') {
                    DrawTexture(porta_aberta_texture, BordaMapax + 48*atoi(fase_atual.pos_porta) - 4, BordaMapay - 36, WHITE);
                }
                
                // Imprime o tiro na sua posição e direção atuais.
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
                
                // Imprime o número de vidas e tiros na lateral direita da tela
                DrawTextEx(Fonte_principal, fase_atual.num_especiais, position_num_especiais, 40, 1, WHITE);
                DrawTextEx(Fonte_principal, jogo_atual.vidas, position_num_vidas, 40, 1, WHITE);
                DrawTextEx(Fonte_principal, jogo_atual.num_ult_fase, position_num_fase, 40, 1, WHITE);
                DrawTextEx(Fonte_principal, "Fase", position_num_fase_letra, 40, 1, WHITE);
                DrawTextEx(Fonte_principal, jogo_atual.totalpts, position_pontuacao, 40, 1, WHITE);
                DrawTextEx(Fonte_principal, "Pontos", position_pontuacao_letra, 40, 1, WHITE);

                // Imprime o lolo baseado em sua posição atualizada e na direção que ele está olhando
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
                
                /*
                    Imprime cada um dos inimigos do vetor de inimigos baseado
                    em seu tipo e no fato de estarem formato bola ou não, a não
                    ser o inimigo 'M' (que é um bloco móvel), que é impresso sempre
                    da mesma maneira.
                */
                for (i = 0; i < fase_atual.num_inimigos; i++) {
                    if (fase_atual.inimigos[i].morto == 'N') {
                        if (fase_atual.inimigos[i].tipo == 'M') {
                            DrawTexture(bloco_movel_texture, BordaMapax + fase_atual.inimigos[i].posicao.x, BordaMapay + fase_atual.inimigos[i].posicao.y, WHITE);
                        }
                        else if (fase_atual.inimigos[i].bola == 'S') {
                            DrawTexture(ovo_texture, BordaMapax + fase_atual.inimigos[i].posicao.x, BordaMapay + fase_atual.inimigos[i].posicao.y, WHITE);
                        }
                        else {
                            switch (fase_atual.inimigos[i].tipo) {
                            case 'L':
                                DrawTexture(larva_texture, BordaMapax + fase_atual.inimigos[i].posicao.x, BordaMapay + fase_atual.inimigos[i].posicao.y, WHITE);
                                break;
                            case 'R':
                                DrawTexture(atirador_R_texture, BordaMapax + fase_atual.inimigos[i].posicao.x, BordaMapay + fase_atual.inimigos[i].posicao.y, WHITE);
                                if (fase_atual.inimigos[i].tiro.mostrar == 'S') {
                                    DrawTexture(chiclete_R_texture, BordaMapax + fase_atual.inimigos[i].tiro.posicao.x, BordaMapay + fase_atual.inimigos[i].tiro.posicao.y, WHITE);
                                }
                                break;
                            case 'D':
                                DrawTexture(atirador_D_texture, BordaMapax + fase_atual.inimigos[i].posicao.x, BordaMapay + fase_atual.inimigos[i].posicao.y, WHITE);
                                if (fase_atual.inimigos[i].tiro.mostrar == 'S') {
                                    DrawTexture(chiclete_D_texture, BordaMapax + fase_atual.inimigos[i].tiro.posicao.x, BordaMapay + fase_atual.inimigos[i].tiro.posicao.y, WHITE);
                                }
                                break;    
                            }
                        }
                    }
                }
                
            EndDrawing();
        }
        
        // QUIT (Tela que aparece quando o usuário decidir sair do jogo)
        if (strcmp(status_jogo, "QUIT") == 0) {
            
            // Movimenta o lolo (cursor que indica a ação) com base na entrada do usuário
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto_quit.y > ponto_y_inic_lolo_load) {
                lolo_sel_ponto_quit.y -= desloc_y_lolo_load;
            }
            if ((IsKeyPressed(KEY_DOWN)) && (lolo_sel_ponto_quit.y < ponto_y_inic_lolo_load + (1 * desloc_y_lolo_load))) {
                lolo_sel_ponto_quit.y += desloc_y_lolo_load;
            }
            
            // Observa se o usuário deseja sair ou continuar e muda o estado da variável status_jogo de maneira apropriada
            if (IsKeyPressed(KEY_ENTER) && lolo_sel_ponto_quit.y == ponto_y_inic_lolo_load) {
                strcpy(status_jogo, "GAME");
            }
            else if (IsKeyPressed(KEY_ENTER) && lolo_sel_ponto_quit.y == ponto_y_inic_lolo_load + (1 * desloc_y_lolo_load)) {
                strcpy(status_jogo, "MENU");
                bau_cheio = 'S';
            }
            
            // Mostra a tela para sair baseado na posição do cursor
            BeginDrawing();
            ClearBackground(BLACK);
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                DrawTextEx(Fonte_principal, "Continuar", position7, 36, 2, BLACK);
                DrawTextEx(Fonte_principal, "Sair", position8, 36, 2, BLACK); 
                DrawTexture(lolo_texture, lolo_sel_ponto_quit.x, lolo_sel_ponto_quit.y, WHITE);
            EndDrawing();
        }
        
        // LOSE (Tela que exibe a mensagem que o jogador perdeu o jogo)
        if (strcmp(status_jogo, "LOSE") == 0) {
            
            // Faz a contagem do número de frames, para depois de 3 segundo sair da tela de texto
            framesCounter2++;
            if (framesCounter2/60 > 3) {
                framesCounter2 = 0;
                strcpy(status_jogo, "MENU");
            }
            
            // Mostra a tela com o fundo padrão e a mensagem de que o jogador perdeu
            BeginDrawing();

                ClearBackground(BLACK);
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);

                DrawTextEx(Fonte_principal, "Suas vidas\nacabaram\nVoltando para\n o menu...", position6, 30, 1, BLACK);

            EndDrawing();
        }
        
        // CRED (Créditos do jogo)
        if (strcmp(status_jogo, "CRED") == 0) {
            
            // Se o usuário aperta ENTER, retorna ao menu
            if(IsKeyPressed(KEY_ENTER)){
                framesCounter = 0;
                strcpy(status_jogo,"MENU");
            }
            
            framesCounter++;
            
            // Mostra a tela dos créditos com uma mensagem que fica atualizando com o tempo
            BeginDrawing();

                ClearBackground(BLACK);
                DrawTexture(cred_texture, (screen_width - cred_texture.width)/2, (screen_height - cred_texture.height)/2, WHITE);
                DrawText(TextSubtext(message, 0, framesCounter/5), 230, 550, 20, BLACK);

            EndDrawing();
        }
        
        // NULL (Tela mostrada quando não há nenhum jogo salvo)
        if (strcmp(status_jogo, "NULL") == 0) {
            
            // Se o usuário aperta ENTER, retorna ao menu
            if(IsKeyPressed(KEY_ENTER)){
                framesCounter = 0;
                strcpy(status_jogo,"MENU");
            }
            
            framesCounter++;
            
            // Mostra a tela que indica que não há nenhum jogo salvo com uma mensagem que fica atualizando com o tempo
            BeginDrawing();

                ClearBackground(BLACK);
                DrawTexture(load_vazio_texture, (screen_width - load_vazio_texture.width)/2, (screen_height - load_vazio_texture.height)/2, WHITE);
                DrawText(TextSubtext(message, 0, framesCounter/5), 230, 550, 20, BLACK);

            EndDrawing();
        }
        
        // CONF (Tela para o usuário confirmar que quer apagar a gravação)
        if (strcmp(status_jogo, "CONF") == 0) {
            
            // Movimenta o lolo (cursor que indica a ação) com base na entrada do usuário
            if (IsKeyPressed(KEY_UP) && lolo_sel_ponto_quit.y > ponto_y_inic_lolo_load) {
                lolo_sel_ponto_quit.y -= desloc_y_lolo_load;
            }
            if ((IsKeyPressed(KEY_DOWN)) && (lolo_sel_ponto_quit.y < ponto_y_inic_lolo_load + (1 * desloc_y_lolo_load))) {
                lolo_sel_ponto_quit.y += desloc_y_lolo_load;
            }
            
            // Observa se o usuário deseja apagar ou manter a gravação e apaga se desejado apagar. Após isso, retorna ao menu.
            if (IsKeyPressed(KEY_ENTER) && lolo_sel_ponto_quit.y == ponto_y_inic_lolo_load) {
                apagar_gravacao(ARQ_GRAVACAO, gravacoes_salvas[0].ident);
                strcpy(status_jogo, "MENU");
            }
            else if (IsKeyPressed(KEY_ENTER) && lolo_sel_ponto_quit.y == ponto_y_inic_lolo_load + (1 * desloc_y_lolo_load)) {
                strcpy(status_jogo, "MENU");
            }
            
            // Mostra a tela para confirmar se deseja apagar a gravação
            BeginDrawing();
            ClearBackground(BLACK);
                
                DrawTexture(fundo_texture, (screen_width - fundo_texture.width)/2, (screen_height - fundo_texture.height)/2, WHITE);
                
                DrawTextEx(Fonte_principal, "Deseja apagar o jogo?", position5, 24, 2, BLACK);
                DrawTextEx(Fonte_principal, "APAGAR", position7, 36, 2, BLACK);
                DrawTextEx(Fonte_principal, "MANTER", position8, 36, 2, BLACK); 
                
                DrawTexture(lolo_texture, lolo_sel_ponto_quit.x, lolo_sel_ponto_quit.y, WHITE);
            EndDrawing();
        }
        
    }
    
    
    CloseAudioDevice();
    return 0;
}
