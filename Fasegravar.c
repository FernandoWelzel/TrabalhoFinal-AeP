// -->> Programa auxiliar para gravar as fases no arquivo de fases <<-- // 

// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bibliotecas adicionais
#include "raylib.h"

// Nossas bibliotecas
#include "Headers.h"

// Declaração de constantes
#define ARQ_FASE "fase.bin"

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

int main(void)
{    
    int i;
    
    // Elementos da primeira fase do jogo original.
    FASE nivel1;
    
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
    nivel1.inimigos[0].morto = 'N';
    nivel1.inimigos[0].tipo = 'L';
    nivel1.inimigos[0].posicao.x = 6*48;
    nivel1.inimigos[0].posicao.y = 5*48;
    nivel1.inimigos[0].bola = 'N';
    nivel1.inimigos[0].tiro.posicao.x = 0;
    nivel1.inimigos[0].tiro.posicao.y = 0;
    nivel1.inimigos[0].tiro.mostrar = 'N';
    nivel1.inimigos[0].tiro.direcao = 'D';
    
    nivel1.inimigos[1].morto = 'N';
    nivel1.inimigos[1].tipo = 'M';
    nivel1.inimigos[1].posicao.x = 8*48;
    nivel1.inimigos[1].posicao.y = 10*48;
    nivel1.inimigos[1].bola = 'S';
    nivel1.inimigos[1].tiro.posicao.x = 0;
    nivel1.inimigos[1].tiro.posicao.y = 0;
    nivel1.inimigos[1].tiro.mostrar = 'N';
    nivel1.inimigos[1].tiro.direcao = 'D';
    
    nivel1.num_inimigos = 2; 
    
    escreve_fase(ARQ_FASE, &nivel1);
    
    // Elementos da segunda fase do jogo modificado
    FASE nivel2;
    
    strcpy(nivel2.num, "02");
    strcpy(nivel2.nome, "Segunda fase");
    
    char texto_inicial_fase2[200] = "Na segunda fase,\nfaca o que for\npreciso para\nvencer!!!\n";
    strcpy(nivel2.texto_inic, texto_inicial_fase2);
    
    strcpy(nivel2.num_especiais, "0\0");
    PONTO pos_jogador_fase2 = {2, 2};
    nivel2.porta_estado = 'F';
    nivel2.num_coracoes = 3;
    strcpy(nivel2.pos_porta, "08");
    nivel2.pos_i_jogador = pos_jogador_fase2;
    
    char elementos_fase2[11][12] = {"PTTTTTPLLLL",
                                    "PLLTTTPPPPL",
                                    "PLLLTLLLLLL",
                                    "PPLLTCAPLLL",
                                    "TPLLTPAPPLT",
                                    "TTLLPPAAPLT",
                                    "TTTLTLAACLL",
                                    "TLLLLLLAALL",
                                    "TTPPPLLAALL",
                                    "CLLLLLAALLL",
                                    "TTPBTLLLLLL"};
                                    
    for (i = 0; i < 11; i++) {
        strcpy(nivel2.elementos[i], elementos_fase2[i]);
    }
    
    // Inimigos
    nivel2.inimigos[0].morto = 'N';
    nivel2.inimigos[0].tipo = 'L';
    nivel2.inimigos[0].posicao.x = 8*48;
    nivel2.inimigos[0].posicao.y = 10*48;
    nivel2.inimigos[0].bola = 'N';
    nivel2.inimigos[0].tiro.posicao.x = 0;
    nivel2.inimigos[0].tiro.posicao.y = 0;
    nivel2.inimigos[0].tiro.mostrar = 'N';
    nivel2.inimigos[0].tiro.direcao = 'D';
    
    nivel2.num_inimigos = 1; 
    
    escreve_fase(ARQ_FASE, &nivel2);
    
    return 0;
}