// -->> Programa auxiliar para gravar as fases no arquivo de fases <<-- // 

// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bibliotecas adicionais
#include "../include/raylib.h"

// Nossas bibliotecas
#include "../include/headers.h"

// Declaração de constantes
#define ARQ_FASE "../records/fase.bin"

void escreve_fase(char * nome_arquivo, pFASE fase) {
    FILE * arquivo;
    if (!(arquivo = fopen(nome_arquivo, "a+b"))) {
        printf("Erro ao abrir o arquivo de fases");
    }
    else {
        fwrite(fase, sizeof(FASE), 1, arquivo);
        fclose(arquivo);
    }
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

void gravar_inimigo(pFASE nivel, int pos, char tipo, int x, int y) {
    nivel->inimigos[pos].morto = 'N';
    nivel->inimigos[pos].tipo = tipo;
    nivel->inimigos[pos].posicao.x = x*48;
    nivel->inimigos[pos].posicao.y = y*48;
    
    if (tipo == 'M') {
        nivel->inimigos[pos].bola = 'S';
    }
    else {
        nivel->inimigos[pos].bola = 'N';
    }
    
    nivel->inimigos[pos].tiro.posicao.x = 0;
    nivel->inimigos[pos].tiro.posicao.y = 0;
    nivel->inimigos[pos].tiro.mostrar = 'N';
    
    switch (tipo) {
        case 'D': 
            nivel->inimigos[pos].tiro.direcao = 'D';
            break;
        case 'R':
            nivel->inimigos[pos].tiro.direcao = 'R';
            break;
        default:
            nivel->inimigos[pos].tiro.direcao = 'L';
            break;
    }
}

int main(void)
{    
    int i;
    
    // Elementos da primeira fase do jogo original.
    FASE nivel1;
    
    // Esse "if" foi colocado a seguir apenas para poder minimizar seu conteúdo no Notepad++. A mesma coisa foi feita para as demais fases.
    if (0 == 0) {
       strcpy(nivel1.num, "01");
        strcpy(nivel1.nome, "Fase inicial");
    
        char texto_inicial_fase1[200] = "Ao longo\nda primeira fase,\nabra o bau e\npasse pela\nporta.";
        strcpy(nivel1.texto_inic, texto_inicial_fase1);
    
        strcpy(nivel1.num_especiais, "0\0");
        PONTO pos_jogador_fase1 = {5, 6};
        nivel1.porta_estado = 'F';
        nivel1.num_coracoes = 2;
        strcpy(nivel1.pos_porta, "06");
        nivel1.pos_i_jogador = pos_jogador_fase1;
    
        char elementos_fase1[11][12] = {"PPPPPPLPPTT",
                                        "PTTPCLLPPTT",
                                        "LTTPPPLPPPT",
                                        "LLTTPPLPPPT",
                                        "LLLLPPLPPTC",
                                        "LLLLLLLLPLL",
                                        "LTTLLLLLLLL",
                                        "TTTTLLLTTLL",
                                        "TTTTLLLTTTL",
                                        "PTTPBLLLTTL",
                                        "PPPPPPLLLLL"};
                                    
        for (i = 0; i < 11; i++) {
            strcpy(nivel1.elementos[i], elementos_fase1[i]);
        }
    
        // Inimigos
        //            (nivel, pos, tipo, x, y)
        gravar_inimigo(&nivel1, 0, 'L', 6, 5);
    
        nivel1.num_inimigos = 1; 
    
        escreve_fase(ARQ_FASE, &nivel1); 
    }

    // Elementos da segunda fase do jogo modificado
    FASE nivel2;
    
    if (0 == 0) {
        strcpy(nivel2.num, "02");
        strcpy(nivel2.nome, "Segunda fase");
    
        char texto_inicial_fase2[200] = "Na segunda fase,\nfaca o que for\npreciso para\nvencer!!!\n";
        strcpy(nivel2.texto_inic, texto_inicial_fase2);
    
        strcpy(nivel2.num_especiais, "0\0");
        PONTO pos_jogador_fase2 = {2, 2};
        nivel2.porta_estado = 'F';
        nivel2.num_coracoes = 3;
        strcpy(nivel2.pos_porta, "05");
        nivel2.pos_i_jogador = pos_jogador_fase2;
    
        char elementos_fase2[11][12] = {"PTTTTLLLLLL",
                                        "PLLTTLPPPPL",
                                        "PLLLTLLLLLL",
                                        "PPLLTCAPLLL",
                                        "TPLLTPAPLLT",
                                        "TTLLPPAPPLT",
                                        "TTTLTLAACLL",
                                        "TLLLLLAALLL",
                                        "TTPPPLAALLL",
                                        "CLLLLLAALLL",
                                        "TTPBTLLLLLL"};
                                    
        for (i = 0; i < 11; i++) {
            strcpy(nivel2.elementos[i], elementos_fase2[i]);
        }
    
        // Inimigos
        //            (nivel, pos, tipo, x, y)
        gravar_inimigo(&nivel2, 0, 'L', 8, 10);
        gravar_inimigo(&nivel2, 1, 'R', 6, 2);
        gravar_inimigo(&nivel2, 2, 'M', 8, 3);
    
        nivel2.num_inimigos = 3; 
    
        escreve_fase(ARQ_FASE, &nivel2);
    }
    
    // Elementos da terceira fase do jogo modificado
    FASE nivel3;
    
    if (0 == 0) {
        strcpy(nivel3.num, "03");
        strcpy(nivel3.nome, "Terceira fase");
    
        char texto_inicial_fase2[200] = " Na terceira fase,\nse mantenha vivo!!!";
        strcpy(nivel3.texto_inic, texto_inicial_fase2);
    
        strcpy(nivel3.num_especiais, "0\0");
        PONTO pos_jogador_fase2 = {2, 1};
        nivel3.porta_estado = 'F';
        nivel3.num_coracoes = 3;
        strcpy(nivel3.pos_porta, "09");
        nivel3.pos_i_jogador = pos_jogador_fase2;
    
        char elementos_fase2[11][12] = {"PPPPPPPPPLL",
                                        "PLLLLLLPPPL",
                                        "PLLLLLLLPPL",
                                        "LLLLLLLLLPL",
                                        "CLLLLLLPPPL",
                                        "TLLLLLLLLPL",
                                        "TTTTLLLPCPL",
                                        "TCLTTLLPPPL",
                                        "TTLLLLLPPLL",
                                        "BLLLLLPPLLL",
                                        "TTTTTLLLLLL"};
                                    
        for (i = 0; i < 11; i++) {
            strcpy(nivel3.elementos[i], elementos_fase2[i]);
        }
    
        // Inimigos
        //            (nivel, pos, tipo, x, y)
        gravar_inimigo(&nivel3, 0, 'R', 1, 3);
        gravar_inimigo(&nivel3, 1, 'R', 1, 4);
        gravar_inimigo(&nivel3, 2, 'M', 4, 2);
        gravar_inimigo(&nivel3, 3, 'M', 5, 2);
        gravar_inimigo(&nivel3, 4, 'L', 8, 10);
    
        nivel3.num_inimigos = 5; 
    
        escreve_fase(ARQ_FASE, &nivel3);
    }
    
    // Elementos da quarta fase do jogo modificado
    FASE nivel4;
    
    if (0 == 0) {
        strcpy(nivel4.num, "04");
        strcpy(nivel4.nome, "Quarta fase");
    
        char texto_inicial_fase4[200] = "     Na quarta fase,\n       descubra o\n         caminho...";
        strcpy(nivel4.texto_inic, texto_inicial_fase4);
    
        strcpy(nivel4.num_especiais, "0\0");
        PONTO pos_jogador_fase4 = {8, 10};
        nivel4.porta_estado = 'F';
        nivel4.num_coracoes = 6;
        strcpy(nivel4.pos_porta, "00");
        nivel4.pos_i_jogador = pos_jogador_fase4;
    
        char elementos_fase2[11][12] = {"LPLLLLLLLLL",
                                        "LLLLPLTCTLT",
                                        "CPPPPLTTTLT",
                                        "PPPLLLPPPLT",
                                        "PBLLLPPPPLT",
                                        "PLLLPPCLLLT",
                                        "AAAEAAAAAEA",
                                        "PPPLTCLLLLL",
                                        "PCPLTTTTCTL",
                                        "PLLLLLLTTTL",
                                        "TTTLLLLLLLL"};
                                    
        for (i = 0; i < 11; i++) {
            strcpy(nivel4.elementos[i], elementos_fase2[i]);
        }
    
        // Inimigos
        //            (nivel, pos, tipo, x, y)
        gravar_inimigo(&nivel4, 0, 'D', 9, 2);
        gravar_inimigo(&nivel4, 1, 'L', 1, 9);
        gravar_inimigo(&nivel4, 2, 'L', 4, 0);
    
        nivel4.num_inimigos = 3; 
    
        escreve_fase(ARQ_FASE, &nivel4);
    }
    
    return 0;
}