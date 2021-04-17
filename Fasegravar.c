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
    char elementos[11][12] = {"PPPPPPLPPTT",
                              "PTTPCLLPPTT",
                              "LTTPPPLPPPT",
                              "LLTTPPLPPPT",
                              "LLLLPPLPPTC",
                              "LLLLLLILPLL",
                              "LTTLLLLLLLL",
                              "TTTTLLLTTLL",
                              "TTTTLLLTTTL",
                              "PTTPBLLLTTL",
                              "PPPPPPLLLLL"};
    
                              
    char texto_inicial[200] = "Ao longo\nda primeira fase,\nmate os inimigos\ne passe pela\nporta.";
    PONTO pos_jogador = {1, 5};
    
    FASE nivel1;
    strcpy(nivel1.num, "01");
    strcpy(nivel1.nome, "Fase inicial");
    nivel1.porta_estado = 'F';
    nivel1.num_coracoes = 2;
    
    for (i = 0; i < 11; i++) {
        strcpy(nivel1.elementos[i], elementos[i]);
    }
    
    strcpy(nivel1.pos_porta, "06");
    nivel1.pos_i_jogador = pos_jogador;
    strcpy(nivel1.texto_inic, texto_inicial);
    
    escreve_fase(ARQ_FASE, &nivel1);
    
    return 0;
}